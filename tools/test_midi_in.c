
#include <fa/fa.h>
#include <fa/util.h>
#include "common.h"

/*
    This program listens for incoming MIDI notes on the default MIDI input device:

        * Prints incoming notes on the standard output

        * Echoes incoming notes to the standard MIDI output device

        * Echoes incoming notes to the standard audio output device
 */

#define kEchoPrint 0
#define kEchoMIDI 1
#define kEchoSynth 2

#define kModeOfEchoing kEchoMIDI

fa_ptr_t just_print(fa_ptr_t _, fa_ptr_t timeMessage)
{
    // fa_print_ln(fa_string_show(timeMessage));
    fa_time_t time = fa_pair_first(timeMessage);
    fa_time_t msg  = fa_pair_second(timeMessage);
    printf("%lld ", fa_time_to_milliseconds(time));
    fa_print("%s\n", msg);

    fa_destroy(fa_pair_first(timeMessage));
    fa_destroy(fa_pair_second(timeMessage));
    fa_destroy(timeMessage);
    return 0;
}

fa_ptr_t print_and_echo_midi(fa_ptr_t x, fa_ptr_t timeMessage)
{
    fa_midi_stream_t out_stream = x;

    // fa_print_ln(fa_string_show(timeMessage));
    fa_time_t time = fa_pair_first(timeMessage);
    fa_time_t msg  = fa_pair_second(timeMessage);
    printf("%lld ", fa_time_to_milliseconds(time));
    fa_print("%s\n", msg);

    // printf("System time (early): %lld\n", fa_clock_milliseconds(fa_clock_standard()));
    fa_midi_schedule_relative(fa_milliseconds(0), fa_action_send(fa_string(""), msg), out_stream);

    fa_destroy(fa_pair_first(timeMessage));
    fa_destroy(timeMessage);
    return 0;
}


static long start_millis = -1;

fa_ptr_t print_and_echo_dls(fa_ptr_t x, fa_ptr_t timeMessage)
{
    fa_print_ln(fa_thread_current());

    fa_audio_stream_t out_stream = x;

    // fa_print_ln(fa_string_show(timeMessage));
    fa_time_t time = fa_pair_first(timeMessage);
    fa_time_t msg  = fa_pair_second(timeMessage);

    long millis = fa_time_to_milliseconds(time);

    if (start_millis == (-1)) {
        start_millis = millis;
    }

    millis -= start_millis;

    printf("%-7ld ", millis);

    fa_print("%s\n", msg);

    // printf("System time (early): %lld\n", fa_clock_milliseconds(fa_clock_standard()));
    fa_audio_schedule_relative(fa_milliseconds(0), fa_action_send(fa_string("dls"), msg), out_stream);
    // fa_mark_used(out_stream);

    fa_destroy(fa_pair_first(timeMessage));
    fa_destroy(timeMessage);
    return 0;

    fa_mark_used(time);
}

fa_ptr_t times2(fa_ptr_t _, fa_ptr_t x)
{
    return fa_multiply(x, fa_signal_constant(2.0));
}

inline static
fa_ptr_t louder(fa_ptr_t data, fa_ptr_t x)
{
    // return x*1.6;
    return fa_multiply(fa_signal_constant(9), x);
}
void run_midi()
{
    fa_midi_session_t s     = fa_midi_begin_session();
    fa_audio_session_t as   = fa_audio_begin_session();
    assert(s  && "No MIDI session");
    assert(as && "No audio session");

    fa_midi_device_t i  = fa_midi_default_input(s);
    fa_midi_device_t o  = fa_midi_default_output(s);
    assert(i && "No MIDI input");
    assert(o && "No MIDI output");

    fa_audio_device_t ai   = fa_audio_default_input(as);
    fa_audio_device_t ao   = fa_audio_default_output(as);
    assert(ai && "No audio input");
    assert(ao && "No audio output");

    fa_midi_stream_t ist = fa_midi_open_stream(i);
    fa_midi_stream_t ost = fa_midi_open_stream(o);

#ifndef _WIN32
    fa_pair_t synth = fa_signal_dls(fa_string("dls"));
#else
    fa_pair_t synth = fa_signal_synth(fa_string("fluid"), fa_string("C:\\sf.sf2"));
#endif
    fa_list_t out              = fa_list_map(louder, NULL, fa_pair_to_list(synth));
    fa_audio_set_parameter(fa_string("sample-rate"), fa_f64(48000), as);
    fa_audio_set_parameter(fa_string("vector-size"), fa_f64(512), as);
    fa_audio_stream_t aost  = fa_audio_open_stream(ai, ao, just, out);

    switch (kModeOfEchoing) {

    case kEchoPrint:
        fa_midi_add_message_callback(just_print, NULL, ist);
        break;

    case kEchoMIDI:
        printf("Echoing via MIDI\n");
        fa_midi_add_message_callback(print_and_echo_midi, ost, ist);
        break;

    case kEchoSynth:
        printf("Echoing via internal synth\n");
        fa_midi_add_message_callback(print_and_echo_dls, aost, ist);
        break;
    }

    if (fa_check(ist)) {
        fa_error_log(ist, NULL);
    }

    while (1) {
        fa_thread_sleep(1000);
        // fa_audio_schedule_relative(hms(0,0,0), fa_action_send(fa_string("dls"), fa_midi_message_create_simple(0x90, 64 + ((0 % 12) * 3), 90)), aost);
    }

    fa_destroy(ist);
    fa_destroy(s);
}

int main(int argc, char const *argv[])
{
    fa_set_log_tool();

    printf("Main thread: ");
    fa_print_ln(fa_thread_current());

    fa_with_faudio() {
        run_midi();
    }
}
