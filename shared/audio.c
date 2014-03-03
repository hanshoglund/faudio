
/*
    faudio

    Copyright (c) DoReMIR Music Research 2012-2013
    All rights reserved.

 */

#include <fa/audio.h>

#include <fa/atomic.h>
#include <fa/dynamic.h>
#include <fa/atomic/queue.h>
#include <fa/pair/left.h>
#include <fa/priority_queue.h>
#include <fa/signal.h>
#include <fa/thread.h>
#include <fa/util.h>
#include <fa/time.h>

#include <portaudio.h>
#include "signal.h"
#include "signal_internal.h"
#include "action_internal.h"

/*
    Notes:
        * Name mixup. What to call incoming/outcoming to make it unambigous.
            * to_audio, from_audio or something?
        * FIXME endSession must check for running streams, or we get consistency errors
            * Either abort streams or refuse to close session
 */

typedef fa_audio_proc_t             proc_t;
typedef fa_audio_device_t           device_t;
typedef fa_audio_stream_t           stream_t;
typedef fa_audio_session_t          session_t;
typedef fa_audio_stream_callback_t  stream_callback_t;
typedef fa_audio_session_callback_t session_callback_t;
typedef fa_audio_status_callback_t  status_callback_t;
typedef fa_signal_custom_processor_t   *custom_proc_t;

typedef PaDeviceIndex native_index_t;
typedef PaStream     *native_stream_t;

#define kMaxSignals             32
#define kMaxMessageCallbacks    64
#define kMaxStatusCallbacks     64

struct _fa_audio_session_t {

    impl_t              impl;               // Dispatcher
    system_time_t       acquired;           // Time of acquisition (not used at the moment)

    list_t              devices;            // Cached device list

    device_t            def_input;          // Default devices, both possibly null
    device_t            def_output;         // If present, these are also in the above list

    list_t              streams;            // All streams started on this sessiuon (list of stream_t)

    struct {
        double          sample_rate;
        double          latency;
        int             vector_size;
    }                   parameters;         // Parameters, which may be updated by set_parameters

    struct {
        int                         count;
        struct {
            fa_nullary_t            function;
            fa_ptr_t                data;
        }                           elements[kMaxStatusCallbacks];
    }                               callbacks;          // Status callbacks

    pair_t                          status_closure;
};

struct _fa_audio_device_t {

    impl_t              impl;               // Dispatcher
    native_index_t      index;              // Native device index
    session_t           session;            // Underlying session

    string_t            name;               // Cached names
    string_t            host_name;

    // bool                muted;           // Not used at the moment
    // double              volume;
};

struct _fa_audio_stream_t {

    impl_t              impl;               // Dispatcher
    native_stream_t     native;             // Native stream, or NULL if closed
    device_t            input, output;

    unsigned            signal_count;       // Number of signals (same as number of outputs)
    signal_t            signals[kMaxSignals];
    state_t             state;              // DSP state

    unsigned            input_channels, output_channels;
    double              sample_rate;
    long                max_buffer_size;
    PaStreamCallbackFlags pa_flags;         // Potential error messages from PortAudio

    struct {
        thread_t        thread;
        mutex_t         mutex;
        bool            stop;
    }                   controller;         // Controller thread (where scheduling runs)

    atomic_queue_t      in_controls;        // Controls for scheduling, (AtomicQueue SomeAction)
    atomic_queue_t      short_controls;     // Controls for scheduling, (AtomicQueue SomeAction)
    atomic_queue_t      out_controls;       // Controls from audio, (AtomicQueue SomeAction)
    priority_queue_t    controls;           // Scheduled controls (PriorityQueue (Time, Action))

    struct {
        int             count;
        struct {
            fa_binary_t function;
            fa_ptr_t    data;
        }               elements[kMaxMessageCallbacks];
    }                   callbacks;          // Message callbacks
};

static mutex_t   pa_mutex;
static bool      pa_status;
static session_t current_session;

error_t audio_device_error(string_t msg);
error_t audio_device_error_with(string_t msg, int error);
ptr_t audio_session_impl(fa_id_t interface);
ptr_t audio_device_impl(fa_id_t interface);
ptr_t audio_stream_impl(fa_id_t interface);
inline static session_t new_session();
inline static void session_init_devices(session_t session);
inline static void delete_session(session_t session);
inline static device_t new_device(session_t session, native_index_t index);
inline static void delete_device(device_t device);
inline static stream_t new_stream(device_t input, device_t output, double sample_rate, long max_buffer_size);
inline static void delete_stream(stream_t stream);

void before_processing(stream_t stream);
void after_processing(stream_t stream);
void during_processing(stream_t stream, unsigned count, float **input, float **output);

static int native_audio_callback(const void *input_ptr,
                                 void *output_ptr,
                                 unsigned long frame_count,
                                 const PaStreamCallbackTimeInfo *time_info,
                                 PaStreamCallbackFlags flags,
                                 void *data);

static void native_finished_callback(void *data);

// --------------------------------------------------------------------------------

ptr_t _status_callback(ptr_t x)
{
    session_t session = (session_t) x;

    int n = session->callbacks.count;

    for (int i = 0; i < n; ++i) {
        nullary_t f = session->callbacks.elements[i].function;
        ptr_t     x = session->callbacks.elements[i].data;
        f(x);
    }

    return x;
}

inline static session_t new_session()
{
    session_t session = fa_new(audio_session);
    session->impl = &audio_session_impl;
    session->callbacks.count = 0;
    return session;
}

inline static void session_init_devices(session_t session)
{
    native_index_t count;
    list_t         devices;

    count   = Pa_GetDeviceCount();
    devices = fa_list_empty();

    for (size_t i = 0; i < count; ++i) {
        device_t device = new_device(session, i);

        if (device) {
            devices = fa_list_dcons(device, devices);
        }
    }

    session->devices      = fa_list_dreverse(devices);
    session->def_input    = new_device(session, Pa_GetDefaultInputDevice());
    session->def_output   = new_device(session, Pa_GetDefaultOutputDevice());
    session->streams      = empty();

    session->parameters.sample_rate = kDefSampleRate;
    session->parameters.vector_size = kDefVectorSize;
    session->parameters.latency     = kDefLatency;
}

inline static void delete_session(session_t session)
{
    // TODO free device list
    fa_delete(session);
}

inline static device_t new_device(session_t session, native_index_t index)
{
    if (index == paNoDevice) {
        return NULL;
    }

    device_t device = fa_new(audio_device);
    device->impl    = &audio_device_impl;

    const PaDeviceInfo  *info      = Pa_GetDeviceInfo(index);
    const PaHostApiInfo *host_info = Pa_GetHostApiInfo(info->hostApi);

    device->index       = index;
    device->session     = session;

    /* 
        PortAudio aspire to return UTF-8 on all platforms but this is not always the case
        Assume native encodings for now.
    
        See also #96
     */
#ifdef _WIN32
    device->name        = fa_string_from_cp1252((char *) info->name);       // const cast
    device->host_name   = fa_string_from_cp1252((char *) host_info->name);
#else
    device->name        = fa_string_from_utf8((char *) info->name);         // const cast
    device->host_name   = fa_string_from_utf8((char *) host_info->name);
#endif

    return device;
}

inline static void delete_device(device_t device)
{
    fa_destroy(device->name);
    fa_destroy(device->host_name);
    fa_delete(device);
}

inline static stream_t new_stream(device_t input, device_t output, double sample_rate, long max_buffer_size)
{
    stream_t stream         = fa_new(audio_stream);

    stream->impl            = &audio_stream_impl;

    stream->input           = input;
    stream->output          = output;
    stream->input_channels  = (!input) ? 0 : fa_audio_input_channels(input);
    stream->output_channels = (!output) ? 0 : fa_audio_output_channels(output);

    stream->sample_rate     = sample_rate;
    stream->max_buffer_size = max_buffer_size;

    stream->signal_count    = 0;
    stream->pa_flags        = 0;

    stream->in_controls     = atomic_queue();
    stream->short_controls  = atomic_queue();
    stream->controls        = priority_queue();

    stream->out_controls    = atomic_queue();

    stream->callbacks.count = 0;

    return stream;
}

inline static void delete_stream(stream_t stream)
{
    fa_destroy(stream->in_controls);
    fa_destroy(stream->out_controls);
    fa_destroy(stream->short_controls);
    fa_destroy(stream->controls);
    fa_delete(stream);
}


// --------------------------------------------------------------------------------

void fa_audio_initialize()
{
    pa_mutex        = fa_thread_create_mutex();
    pa_status       = false;
    current_session = NULL;

    if (kVectorMode) {
        inform(string("    Using vector processing"));
    } else {
        inform(string("    Using single-step processing"));
    }
}

void fa_audio_terminate()
{
    fa_thread_destroy_mutex(pa_mutex);
}

// --------------------------------------------------------------------------------

void add_audio_status_listener(fa_pair_t closure);
void remove_audio_status_listener(fa_pair_t closure);

session_t fa_audio_begin_session()
{
    if (!pa_mutex) {
        assert(false && "Module not initalized");
    }

    inform(string("Initializing real-time audio session"));

    session_t session;
    fa_with_lock(pa_mutex) {
        if (pa_status) {
            session = (session_t) audio_device_error(string("Overlapping real-time audio sessions"));
        } else {
            inform(string("Starting up PortAudio"));
            Pa_Initialize();
            pa_status = true;
            inform(string("Started PortAudio"));

            session = new_session();
            session_init_devices(session);

            current_session = session;
        }
    }
    // FIXME cache pair
    session->status_closure = pair(_status_callback, session);
    add_audio_status_listener(session->status_closure);

    inform(string("Finished initializing session"));
    return session;
}

void fa_audio_end_session(session_t session)
{
    if (!pa_mutex) {
        assert(false && "Module not initalized");
    }

    inform(string("Terminating real-time audio session"));

    fa_with_lock(pa_mutex) {
        fa_for_each(stream, session->streams) {
            // It is OK if the stream is already closed
            fa_audio_close_stream(stream);
            delete_stream(stream);
        }

        if (pa_status) {
            Pa_Terminate();
            pa_status = false;
        }

        current_session = NULL;
    }
    remove_audio_status_listener(session->status_closure);

    inform(string("Finished terminating session"));
    delete_session(session);
}

void fa_audio_with_session(session_callback_t session_callback,
                           ptr_t                           session_data,
                           error_callback_t                error_callback,
                           ptr_t                           error_data
                          )
{
    session_t session = fa_audio_begin_session();

    if (fa_check(session)) {
        error_callback(error_data, (error_t) session);
    } else {
        session_callback(session_data, session);
    }

    fa_audio_end_session(session);
}

void fa_audio_set_parameter(string_t name,
                            ptr_t value,
                            session_t session)
{
    if (fa_equal(name, string("sample-rate"))) {
        double x;

        switch (fa_dynamic_get_type(value)) {
        case i32_type_repr:
            x = fa_peek_int32(value);
            break;

        case f32_type_repr:
            x = fa_peek_float(value);
            break;

        case f64_type_repr:
            x = fa_peek_double(value);
            break;

        default:
            warn(string("Wrong type"));
            return;
        }

        session->parameters.sample_rate = x;
    }

    if (fa_equal(name, string("latency"))) {
        double x;

        switch (fa_dynamic_get_type(value)) {
        case i32_type_repr:
            x = fa_peek_int32(value);
            break;

        case f32_type_repr:
            x = fa_peek_float(value);
            break;

        case f64_type_repr:
            x = fa_peek_double(value);
            break;

        default:
            warn(string("Wrong type"));
            return;
        }

        session->parameters.latency = x;
    }

    if (fa_equal(name, string("vector-size"))) {
        int x;

        switch (fa_dynamic_get_type(value)) {
        case i32_type_repr:
            x = fa_peek_int32(value);
            break;

        case f32_type_repr:
            x = fa_peek_float(value);
            break;

        case f64_type_repr:
            x = fa_peek_double(value);
            break;

        default:
            warn(string("Wrong type"));
            return;
        }

        if (x <= kMaxVectorSize) {
            session->parameters.vector_size = x;
        } else {
            warn(fa_string_format_integral("Vector size %d too large, ignoring parameter.", x));
        }
    }
}

fa_list_t fa_audio_current_sessions()
{
    if (!current_session) {
        return empty();
    } else {
        return list(current_session);
    }
}

fa_ptr_t fa_audio_end_all_sessions()
{
    fa_dfor_each(x, fa_audio_current_sessions()) {
        fa_audio_end_session(x);
    }
    return NULL;
}

fa_list_t fa_audio_all(session_t session)
{
    return fa_copy(session->devices);
}

#define fail_if_no_input(type) \
    if (!session->def_input) { \
        return (type) audio_device_error(string("No input device available")); \
    }
#define fail_if_no_output(type) \
    if (!session->def_output) { \
        return (type) audio_device_error(string("No output device available")); \
    }

fa_pair_t fa_audio_default(session_t session)
{
    fail_if_no_input(fa_pair_t);
    fail_if_no_output(fa_pair_t);
    return pair(session->def_input, session->def_output);
}

device_t fa_audio_default_input(session_t session)
{
    fail_if_no_input(device_t);
    return session->def_input;
}

device_t fa_audio_default_output(session_t session)
{
    fail_if_no_output(device_t);
    return session->def_output;
}

session_t fa_audio_session(device_t device)
{
    return device->session;
}

fa_string_t fa_audio_name(device_t device)
{
    return fa_copy(device->name);
}

fa_string_t fa_audio_host_name(device_t device)
{
    return fa_copy(device->host_name);
}

fa_string_t fa_audio_full_name(device_t device)
{
    string_t str = string("");
    fa_write_string(str, fa_audio_host_name(device));
    fa_write_string(str, string(" "));
    fa_write_string(str, fa_audio_name(device));
    fa_write_string(str, string(" "));
    fa_write_string(str, fa_string_format_integral("(%d in, ", fa_audio_input_channels(device)));
    fa_write_string(str, fa_string_format_integral("%d out)", fa_audio_output_channels(device)));
    return str;
}

int fa_audio_input_channels(device_t device)
{
    const PaDeviceInfo *info = Pa_GetDeviceInfo(device->index);
    return info->maxInputChannels;
}

int fa_audio_output_channels(device_t device)
{
    const PaDeviceInfo *info = Pa_GetDeviceInfo(device->index);
    return info->maxOutputChannels;
}

bool fa_audio_has_input(device_t device)
{
    return fa_audio_input_channels(device) != 0;
}
bool fa_audio_has_output(device_t device)
{
    return fa_audio_output_channels(device) != 0;
}


// TODO add/remove session status listener


void fa_audio_add_status_callback(status_callback_t function,
                                  ptr_t             data,
                                  session_t         session)
{
    int n = session->callbacks.count++;
    assert(n < kMaxStatusCallbacks);

    session->callbacks.elements[n].function = function;
    session->callbacks.elements[n].data     = data;
}

double fa_audio_current_sample_rate(fa_audio_device_t device)
{
    const PaDeviceInfo *info = Pa_GetDeviceInfo(device->index);
    return info->defaultSampleRate;
}

double fa_audio_default_sample_rate(fa_audio_device_t device)
{
    return fa_audio_current_sample_rate(device);
}


// --------------------------------------------------------------------------------

inline static
void print_audio_info(device_t input, device_t output)
{
    inform(string("Opening real-time audio stream"));
    inform(string_dappend(string("    Input:         "), input ? fa_audio_full_name(input) : string("-")));
    inform(string_dappend(string("    Output:        "), output ? fa_audio_full_name(output) : string("-")));

    fa_let(session, input ? input->session : output->session) {
        inform(fa_string_format_floating("    Sample Rate:   %2f", session->parameters.sample_rate));
        inform(fa_string_format_floating("    Latency:       %3f", session->parameters.latency));
        inform(fa_string_format_integral("    Vector Size:   %d",  session->parameters.vector_size));
    }
}

inline static
void print_signal_tree(ptr_t x)
{
    inform(fa_string_show(x));
}

inline static
list_t apply_processor(proc_t proc, ptr_t proc_data, list_t inputs)
{
    if (proc) {
        return proc(proc_data, inputs);
    } else {
        // TODO check number of channels is < kMaxSignals
        warn(string("Audio.openStream: Assuming stereo output"));
        return list(fa_signal_constant(0), fa_signal_constant(0));
    }
}

stream_t fa_audio_open_stream(device_t input,
                              device_t output,
                              proc_t proc,
                              ptr_t proc_data
                             )
{
    PaError         status;

    if (!input && !output) {
        return (stream_t) audio_device_error_with(
                   string("Can not open a stream with no devices"), 0);
    }

    if (input && output && (input->session != output->session)) {
        return (stream_t) audio_device_error_with(
                   string("Can not open a stream on devices from different sessions"), 0);
    }

    unsigned long   buffer_size = (input ? input : output)->session->parameters.vector_size;
    double          sample_rate = (input ? input : output)->session->parameters.sample_rate;
    stream_t        stream = new_stream(input, output, sample_rate, buffer_size);

    {
        // TODO number of inputs
        list_t inputs = list(fa_signal_input(kInputOffset + 0), fa_signal_input(kInputOffset + 1));
        list_t signals = apply_processor(proc, proc_data, inputs);

        stream->signal_count = fa_list_length(signals);

        for (int i = 0; i < stream->signal_count; ++i) {
            stream->signals[i] = fa_list_index(i, signals);
        }
    }
    {
        /*
            Print info messages.
         */
        print_audio_info(input, output);
    }
    fa_let(session, input ? input->session : output->session) {
        /*
            Open and set native stream.
         */
        PaStreamParameters input_stream_parameters = {
            .suggestedLatency           = session->parameters.latency,
            .hostApiSpecificStreamInfo  = NULL,
            .device                     = (input ? input->index : 0),
            .sampleFormat               = (paFloat32 | paNonInterleaved),
            .channelCount               = stream->input_channels
        };

        PaStreamParameters output_stream_parameters = {
            .suggestedLatency           = session->parameters.latency,
            .hostApiSpecificStreamInfo  = NULL,
            .device                     = (output ? output->index : 0),
            .sampleFormat               = (paFloat32 | paNonInterleaved),
            .channelCount               = stream->output_channels
        };

        PaStreamFlags    flags    = paNoFlag;
        PaStreamCallback *callback = native_audio_callback;
        ptr_t            data     = stream;

        status = Pa_OpenStream(
                     &stream->native,
                     input ? &input_stream_parameters : NULL,
                     output ? &output_stream_parameters : NULL,
                     sample_rate, buffer_size, flags,
                     callback, data
                 );

        if (status != paNoError) {
            return (stream_t) audio_device_error_with(string("Could not start stream"), status);
        }

        status = Pa_SetStreamFinishedCallback(stream->native, native_finished_callback);

        if (status != paNoError) {
            return (stream_t) audio_device_error_with(string("Could not start stream"), status);
        }
    }
    {
        /*
            Prepare and launch DSP thread.
         */
        before_processing(stream);

        status = Pa_StartStream(stream->native);

        if (status != paNoError) {
            return (stream_t) audio_device_error_with(string("Could not start stream"), status);
        }
    }
    {
        /*
            Launch control thread.
         */
        ptr_t audio_control_thread(ptr_t data);
        stream->controller.thread = fa_thread_create(audio_control_thread, stream);
        stream->controller.mutex  = fa_thread_create_mutex();
        stream->controller.stop   = false;
    }


    fa_let(session, input ? input->session : output->session) {
        fa_push_list(stream, session->streams);
    }
    return stream;
}

void fa_audio_close_stream(stream_t stream)
{
    inform(string("Closing real-time audio stream"));
    inform(fa_string_format_integral("  Stream: %p \n", (long) stream));

    {
        // TODO need atomic
        native_stream_t native = stream->native;

        if (native) {
            inform(string("    Closing native stream"));
            stream->native = NULL;

            PaError error;

            if ((error = Pa_StopStream(native)) != paNoError) {
                warn(string("Could not stop stream: "));
                warn(string((char *) Pa_GetErrorText(error)));
            }

            if ((error = Pa_CloseStream(native)) != paNoError) {
                warn(string("Could not close stream"));
                warn(string((char *) Pa_GetErrorText(error)));
            }

            // after_processing will be called after this

            inform(string("    Native stream closed"));
            inform(string("    Stopping stream controller"));

            stream->controller.stop = true;
            fa_thread_join(stream->controller.thread);
            fa_thread_destroy_mutex(stream->controller.mutex);

            inform(string("    Stream controller thread stopped"));
        }
    }

    // Not deleted until sesion is gone
}

void fa_audio_with_stream(device_t            input,
                          device_t            output,
                          proc_t              proc,
                          ptr_t               proc_data,
                          stream_callback_t   stream_callback,
                          ptr_t               stream_data,
                          error_callback_t    error_callback,
                          ptr_t               error_data)
{
    stream_t stream = fa_audio_open_stream(input, output, proc, proc_data);

    if (fa_check(stream)) {
        error_callback(error_data, (error_t) stream);
    } else {
        stream_callback(stream_data, stream);
    }

    fa_audio_close_stream(stream);
}

list_t fa_audio_devices(fa_audio_stream_t stream)
{
    if (fa_equal(stream->input, stream->output)) {
        return list(stream->input);
    } else {
        return list(stream->input, stream->output);
    }
}

fa_clock_t fa_audio_stream_clock(fa_audio_stream_t stream)
{
    return (fa_clock_t) stream;
}

fa_clock_t fa_audio_get_clock(fa_audio_stream_t stream)
{
    return (fa_clock_t) stream;
}

void fa_audio_add_message_callback(fa_audio_message_callback_t function,
                                   fa_ptr_t data,
                                   fa_audio_stream_t stream)
{
    int n = stream->callbacks.count++;
    assert(n < kMaxMessageCallbacks);
    stream->callbacks.elements[n].function = function;
    stream->callbacks.elements[n].data     = data;
}

void fa_audio_schedule(fa_time_t time,
                       fa_action_t action,
                       fa_audio_stream_t stream)
{
    pair_left_t pair = pair_left(time, action);
    fa_with_lock(stream->controller.mutex) {
        fa_priority_queue_insert(pair, stream->controls);
    }
}

void fa_audio_schedule_relative(fa_time_t         time,
                                fa_action_t        action,
                                fa_audio_stream_t  stream)
{
    if (fa_equal(time, seconds(0)) && !fa_action_is_compound(action)) {
        fa_atomic_queue_write(stream->short_controls, action);
    } else {
        time_t now = fa_clock_time(fa_audio_stream_clock(stream));
        fa_audio_schedule(fa_add(now, time), action, stream);
    }
}


ptr_t forward_action_to_audio_thread(ptr_t x, ptr_t action)
{
    stream_t stream = x;
    fa_atomic_queue_write(stream->in_controls, action);
    return NULL;
}
ptr_t audio_control_thread(ptr_t x)
{
    stream_t stream = x;

    inform(string("Audio control thread active"));

    while (true) {
        if (stream->controller.stop) {
            break;
        }

        {
            ptr_t nameValue;
            
            while (0) // FIXME
            // while ((nameValue = fa_atomic_queue_read(stream->out_controls)))
            {
                int n = stream->callbacks.count;

                
                fa_unpair(nameValue, name, value) {

                    // FIXME assure that this copying can not happen after stream has been
                    // stopped
                    string_t name2 = fa_copy(name);
                    ptr_t    value2 = fa_copy(value);
                    // inform(fa_string_show(pair(name2, value2)));

                    for (int j = 0; j < n; ++j) {
                        binary_t cbFunc = stream->callbacks.elements[j].function;
                        ptr_t    cbData = stream->callbacks.elements[j].data;
                        cbFunc(cbData, name2, value2);
                    }         
                    fa_destroy(name2);
                    fa_destroy(value2);
                }
            }
        }

        fa_with_lock(stream->controller.mutex) {
            time_t now = fa_clock_time(fa_audio_stream_clock(stream));
            run_actions(stream->controls,
                        now,
                        forward_action_to_audio_thread,
                        stream
                       );
            fa_destroy(now);

            // Note: thread_sleep may be to inexact.

            // We need to find a more precise way of invoking the scheduler. We could either
            //  * Look for a platform-independent timing library
            //  * Write platform-specific code
            //  * Use notifications from the audio thread (might not work at startup)

            fa_thread_sleep(kAudioSchedulerIntervalMillis);
        }
    }

    inform(string("Audio control thread finished"));
    return NULL;
}

// --------------------------------------------------------------------------------

#define VALS inputs
#define MERGED_SIGNAL signals[0]


void before_processing(stream_t stream)
{
    session_t session  = stream->input ? stream->input->session : stream->output->session;
    stream->state      = new_state(session->parameters.sample_rate); // FIXME

    signal_t merged = fa_signal_constant(0);

    for (int c = 0; c < stream->signal_count; ++c) {
        signal_t withOutput = fa_signal_output(0, kOutputOffset + c, stream->signals[c]);
        merged = fa_signal_former(merged, withOutput); // Could use any combinator here
    }

    fa_for_each(x, fa_signal_get_procs(merged)) {
        // printf("Adding custom proc %p!\n", x);
        add_custom_proc(x, stream->state);
    }
    stream->MERGED_SIGNAL = fa_signal_simplify(merged);

    print_signal_tree(stream->MERGED_SIGNAL);

    stream->MERGED_SIGNAL = fa_signal_doptimize(stream->MERGED_SIGNAL);
    stream->MERGED_SIGNAL = fa_signal_dverify(stream->MERGED_SIGNAL);

    run_custom_procs(custom_proc_before, 0, stream->state);
}

void after_processing(stream_t stream)
{
    run_custom_procs(custom_proc_after, 0, stream->state);
    delete_state(stream->state);
}

ptr_t run_simple_action2(ptr_t x, ptr_t a)
{
    return run_simple_action(x, a);
}
void handle_outgoing_message(ptr_t x, string_t name, ptr_t value)
{
    stream_t stream = x;
    mark_used(stream);                         
    // FIXME
    // fa_atomic_queue_write(stream->out_controls, pair(name, value));
}

void during_processing(stream_t stream, unsigned count, float **input, float **output)
{
    state_base_t state = (state_base_t) stream->state;
    {
        ptr_t action;

        while ((action = fa_atomic_queue_read(stream->in_controls))) {
            run_simple_action2(stream->state, action);
        }
    }
    {
        ptr_t action;

        while ((action = fa_atomic_queue_read(stream->short_controls))) {
            run_simple_action2(stream->state, action);
        }
    }

    // Outgoing controls
    {
        custom_procs_receive((state_t) state, handle_outgoing_message, stream);
    }

    if (!kVectorMode) {
        for (int i = 0; i < count; ++ i) {
            run_custom_procs(custom_proc_render, count, stream->state);

            if (stream->input) {
                for (int c = 0; c < stream->input_channels; ++c) {
                    state->VALS[(c + kInputOffset) * kMaxVectorSize] = input[c][i];
                }
            }      

            step(stream->MERGED_SIGNAL, stream->state);

            if (stream->output) {
                for (int c = 0; c < stream->output_channels; ++c) {
                    output[c][i] = state->VALS[(c + kOutputOffset) * kMaxVectorSize];
                }
            }

            inc_state1(stream->state);
        }
    } else {
        // assert((count == kMaxVectorSize) && "Wrong vector size");
        // assert((stream->signal_count == 2) && "Wrong number of channels");

        if (stream->input) {
            for (int i = 0; i < count; ++ i) {
                for (int c = 0; c < stream->input_channels; ++c) {
                    state->VALS[(c + kInputOffset) * kMaxVectorSize + i] = input[c][i];
                }
            }
        }   

        {
            double dummy_output[count];
            run_custom_procs(custom_proc_render, count, stream->state);
            step_vector(stream->MERGED_SIGNAL, stream->state, count, dummy_output);
        }

        inc_state(count, stream->state);

        if (stream->output) {
            for (int i = 0; i < count; ++ i) {
                for (int c = 0; c < stream->output_channels; ++c) {
                    output[c][i] = state->VALS[(c + kOutputOffset) * kMaxVectorSize + i];
                }
            }
        }
    }
}


/* The callbacks */

int native_audio_callback(const void                       *input,
                          void                             *output,
                          unsigned long                     count,
                          const PaStreamCallbackTimeInfo   *time_info,
                          PaStreamCallbackFlags             flags,
                          void                             *data)
{
    during_processing(data, count, (float **) input, (float **) output);
    stream_t stream = data;
    stream->pa_flags |= flags;

    return paContinue;
}

void native_finished_callback(void *data)
{
    stream_t stream = data;
    inform(fa_string_format_integral("Stream flag result (0 = ok): %d", stream->pa_flags));

    after_processing(data);
}


// --------------------------------------------------------------------------------

bool audio_session_equal(ptr_t a, ptr_t b)
{
    return a == b;
}

fa_string_t audio_session_show(ptr_t a)
{
    string_t str = string("<AudioSession ");
    str = string_dappend(str, fa_string_format_integral("%p", (long) a));
    str = string_dappend(str, string(">"));
    return str;
}

void audio_session_destroy(ptr_t a)
{
    fa_audio_end_session(a);
}

ptr_t audio_session_impl(fa_id_t interface)
{
    static fa_equal_t audio_session_equal_impl
        = { audio_session_equal };
    static fa_string_show_t audio_session_show_impl
        = { audio_session_show };
    static fa_destroy_t audio_session_destroy_impl
        = { audio_session_destroy };

    switch (interface) {
    case fa_equal_i:
        return &audio_session_equal_impl;

    case fa_string_show_i:
        return &audio_session_show_impl;

    case fa_destroy_i:
        return &audio_session_destroy_impl;

    default:
        return NULL;
    }
}


// --------------------------------------------------------------------------------

bool audio_device_equal(ptr_t a, ptr_t b)
{
    device_t device1 = (device_t) a;
    device_t device2 = (device_t) b;
    // TODO check that session is valid
    return device1->index == device2->index;
}

fa_string_t audio_device_show(ptr_t a)
{
    device_t device = (device_t) a;

    string_t str = string("<AudioDevice ");
    str = string_dappend(str, fa_audio_host_name(device));
    str = string_dappend(str, string(" "));
    str = string_dappend(str, fa_audio_name(device));
    str = string_dappend(str, string(">"));
    return str;
}

ptr_t audio_device_impl(fa_id_t interface)
{
    static fa_equal_t audio_device_equal_impl
        = { audio_device_equal };
    static fa_string_show_t audio_device_show_impl
        = { audio_device_show };

    switch (interface) {
    case fa_equal_i:
        return &audio_device_equal_impl;

    case fa_string_show_i:
        return &audio_device_show_impl;

    default:
        return NULL;
    }
}


// --------------------------------------------------------------------------------

bool audio_stream_equal(ptr_t a, ptr_t b)
{
    return a == b;
}

fa_string_t audio_stream_show(ptr_t a)
{
    string_t str = string("<AudioStream ");
    str = string_dappend(str, fa_string_format_integral(" %p", (long) a));
    str = string_dappend(str, string(">"));
    return str;
}

void audio_stream_destroy(ptr_t a)
{
    fa_audio_close_stream(a);
}

int64_t audio_stream_milliseconds(ptr_t a)
{
    stream_t stream = (stream_t) a;
    state_base_t state = (state_base_t) stream->state;

    return (int64_t)((double) state->count / (double) state->rate * 1000.0);
}

fa_time_t audio_stream_time(ptr_t a)
{
    int64_t ms = audio_stream_milliseconds(a);
    return fa_milliseconds(ms);
}


ptr_t audio_stream_impl(fa_id_t interface)
{
    static fa_string_show_t audio_stream_show_impl
        = { audio_stream_show };
    static fa_clock_interface_t audio_stream_clock_impl
        = { audio_stream_time, audio_stream_milliseconds };
    static fa_equal_t audio_stream_equal_impl
        = { audio_stream_equal };
    static fa_destroy_t audio_stream_destroy_impl
        = { audio_stream_destroy };

    switch (interface) {

    case fa_clock_interface_i:
        return &audio_stream_clock_impl;

    case fa_string_show_i:
        return &audio_stream_show_impl;

    case fa_equal_i:
        return &audio_stream_equal_impl;

    case fa_destroy_i:
        return &audio_stream_destroy_impl;

    default:
        return NULL;
    }
}


// --------------------------------------------------------------------------------

void fa_log_error_from(fa_string_t msg, fa_string_t origin);

error_t audio_device_error(string_t msg)
{
    error_t err = fa_error_create_simple(error,
                                         msg,
                                         string("Doremir.Device.Audio"));
    fa_error_log(NULL, err);
    return err;
}

error_t audio_device_error_with(string_t msg, int code)
{
    string_t pa_error_str = string(code != 0 ? (char *) Pa_GetErrorText(code) : "");

    error_t err = fa_error_create_simple(error,
                                         string_dappend(msg,
                                                        string_dappend(string(": "), pa_error_str)
                                                        // format_integral(" (error code %d)", code)
                                                       ),
                                         string("Doremir.Device.Audio"));
    fa_error_log(NULL, err);
    return err;
}

void audio_device_fatal(string_t msg, int code)
{
    fa_log_error_from(
        string_dappend(msg, format_integral(" (error code %d)", code)),
        string("Doremir.Device.Audio"));

    fa_log_error(string("Terminating Fa"));
    exit(error);
}


// nothing really
