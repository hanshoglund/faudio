
#include <fa/signal.h>
#include <fa/midi/message.h>
#include <fa/util.h>

#include <fa/action.h>
#include "../shared/signal.h"

#include <fluidsynth.h>

typedef struct fluid_context {
    fluid_synth_t *synth;
    fa_string_t name;
} fluid_context;

fa_pair_t fa_signal_dls(fa_string_t name)
{
    assert(false && "Not available on this platform");
}


fa_ptr_t before_(fa_ptr_t x, int count, fa_signal_state_t *state)
{
    fluid_context *context = x;
    fluid_synth_t *synth = context->synth;
    fluid_synth_set_sample_rate(synth, state->rate);
    return NULL;
}

fa_ptr_t after_(fa_ptr_t x, int count, fa_signal_state_t *state)
{
    // Nothing
}

// TODO not reentrant
static float left[kMaxVectorSize];
static float right[kMaxVectorSize];


fa_ptr_t render_(fa_ptr_t x, int offset, int count, fa_signal_state_t *state)
{
    fluid_context *context = x;
    fluid_synth_t *synth = context->synth;

    float *left2 = left;
    float *right2 = right;

    if (!kVectorMode) {
        if (FLUID_OK != fluid_synth_nwrite_float(
                    synth,
                    1,
                    &left2,
                    &right2,
                    NULL,
                    NULL
                )) {
            fa_warn(fa_string("Fluidsynth: Could not render"));
        }

        state->buffer[(offset + 0)*kMaxVectorSize] = left[0];
        state->buffer[(offset + 1)*kMaxVectorSize] = right[0];
        return x;
    } else {
        if (FLUID_OK != fluid_synth_nwrite_float(
                    synth,
                    count, // TODO
                    &left2,
                    &right2,
                    NULL,
                    NULL
                )) {
            fa_warn(fa_string("Fluidsynth: Could not render"));
        }

        for (int i = 0; i < count; ++i) {
            state->buffer[(offset + 0)*kMaxVectorSize + i] = left[i];
            state->buffer[(offset + 1)*kMaxVectorSize + i] = right[i];
        }

        return x;
    }

    return NULL;
}

fa_ptr_t receive_(fa_ptr_t x, fa_signal_name_t n, fa_signal_message_t msg)
{
    fluid_context *context = x;
    fluid_synth_t *synth = context->synth;


    // printf("System time (early): %lld\n", fa_clock_milliseconds(fa_clock_standard()));

    if (fa_equal(n, context->name)) {

        if (!fa_midi_message_is_simple(msg)) {
			fa_warn(fa_string("SYSEX message to Fluidsynth"));
			//if (FLUID_OK != fluid_synth_sysex(synth, msg->sysex->data, msg->sysex->size, NULL, 0, NULL, 0)) {
			//	fa_warn(fa_string("Fluidsynth: Could not send SYSEX message"));
			//}
			
        } else {


            uint8_t status_channel, data1, data2;
            fa_midi_message_decons(msg, &status_channel, &data1, &data2);

            int channel = status_channel        & 0x0f;
            int status  = (status_channel >> 4) & 0x0f;

            // printf("%p %d %d %d %d\n", synth, status, channel, data1, data2);
            switch (status) {
            case 0x8: // off
                if (FLUID_OK != fluid_synth_noteoff(synth, channel, data1)) {
                    if (channel == 9) {
                        // Don't warn for noteoff on the percussion channel
                    } else {
                        fa_slog_warning("Fluidsynth: Could not send noteoff message ", msg);
                    }
                }

                break;

            case 0x9: // 9 on
                if (FLUID_OK != fluid_synth_noteon(synth, channel, data1, data2)) {
                    if ((channel == 9) && (data2 == 0)) {
                        // Don't warn for noteoff on the percussion channel
                    } else {
                        fa_slog_warning("Fluidsynth: Could not send noteon message ", msg);
                    }
                }

                break;

            case 0xa: // 10 polyp after
                fa_warn(fa_string("Fluidsynth: Polyphonic key pressure not supported"));
                break;

            case 0xb: // 11 control
                if (FLUID_OK != fluid_synth_cc(synth, channel, data1, data2)) {
                    fa_slog_warning("Fluidsynth: Could not send control message ", msg);
                }

                break;

            case 0xc: // 12 program
                if (FLUID_OK != fluid_synth_program_change(synth, channel, data1)) {
                    fa_warn(fa_string_format_integral("Fluidsynth: Could not send program change message %u", data1));
                }

                break;

            case 0xd: // 13 channel press
                if (FLUID_OK != fluid_synth_channel_pressure(synth, channel, data1)) {
                    fa_warn(fa_string("Fluidsynth: Could not send channel pressure message"));
                }

                break;

            case 0xe: { // 14 pitch wheel
                // TODO do we need to to call pitch_wheel_sens etc?
                unsigned short bend;
                bend = (unsigned short)data2;
                bend <<= 7;
                bend |= (unsigned short)data1;

                if (FLUID_OK != fluid_synth_pitch_bend(synth, channel, bend)) {
                    fa_warn(fa_string("Fluidsynth: Could not send pitch bend message"));
                }

                break;
            }

            default: {
                if ((status_channel == 0xff) && (data1 == 0x2f)) {
                    // EndOfTrack, ignore
                } else {
                    fa_slog_warning("Unknown MIDI message to Fluidsynth: ", msg);
                    // assert(false && "Unknown MIDI message to Fluidsynth");
                }
            }
            }

        }
    }

    return NULL;
}

fa_ptr_t destroy_(fa_ptr_t x)
{
    fa_inform(fa_string("        Destroying FluidSynth instance"));
    fluid_context *context = x;
    fluid_synth_t *synth = context->synth;
    fluid_settings_t *settings = fluid_synth_get_settings(synth);

    delete_fluid_synth(synth);
    delete_fluid_settings(settings);
    fa_free(context);
}

fa_pair_t fa_signal_synth(fa_string_t name, fa_string_t path2)
{
    // FIXME: name is currently ignored and hardcoded to "fluid" in receive_
    
    // create synth
    fluid_synth_t *synth = NULL;
    {
        fluid_settings_t *settings = new_fluid_settings();

        // fluid_settings_setstr(settings, "name", value);
        // fluid_settings_setsint(settings, "name", value);
        // fluid_settings_setsnum(settings, "name", value);

        fluid_settings_setnum(settings, "synth.gain", 0.6);
        fluid_settings_setint(settings, "synth.threadsafe-api", 0);
        fluid_settings_setint(settings, "synth.verbose", 0);

        fa_inform(fa_string("Creating FluidSynth instance"));
        synth = new_fluid_synth(settings);

        fa_inform(fa_string_dappend(fa_string("    Loading sound font "), fa_copy(path2)));
        char *path = fa_unstring(path2);

        if (FLUID_FAILED == fluid_synth_sfload(synth, path, true)) {
            fa_warn(fa_string("Fluidsynth: Could not load sound font"));
            return NULL; // TODO error
        }

        // Sample rate is set later

        printf("%p\n", synth);
    }

    fluid_context *context = fa_new_struct(fluid_context);
    context->synth = synth;
    context->name  = fa_copy(name);

    fa_signal_custom_processor_t *proc = fa_malloc(sizeof(fa_signal_custom_processor_t));
    proc->before  = before_;
    proc->after   = after_;
    proc->render  = render_;
    proc->receive = receive_;
    proc->send    = NULL;
    proc->destroy = destroy_;
    proc->data    = context;

    fa_signal_t left  = fa_signal_input_with_custom(proc, 0);
    fa_signal_t right = fa_signal_input_with_custom(proc, 1);

    // Return stereo output, embedding the custom proc (so it is actually run)
    return fa_pair_create(fa_signal_custom(proc, left), right);
}
