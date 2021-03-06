
#include <fa/fa.h>
#include <fa/util.h>
#include "common.h"

/*
    This program prints the currently connected audio devices.

    The output consists of device declarations separated by blank lines.

    An example device declaration:

        Name:           Built-in Microphone
        Host:           Core Audio
        Input:          2
        Output:         0

 */
void print_device(fa_audio_device_t x)
{
    fa_print("Name: %s\n", fa_audio_name(x));
    // fa_print("Name length: %d\n", fa_string_length(fa_audio_name(x)));
    fa_print("In:   %s\n", fa_i16(fa_audio_input_channels(x)));
    fa_print("Out:  %s\n", fa_i16(fa_audio_output_channels(x)));
    fa_print("Host: %s\n", fa_audio_host_name(x));
    fa_print("Rate: %s\n", fa_f64(fa_audio_default_sample_rate(x)));
    fa_print_ln(fa_string(""));
}

fa_audio_session_t print_audio_devices(fa_ptr_t _, fa_audio_session_t session)
{
    fa_for_each(x, fa_audio_all(session)) {
        if (!fa_check(x)) {
            print_device(x);
        }
    }

#define compose(F,X,N) F(X(N))
#define default_input_name(x) compose(fa_audio_name, fa_audio_default_input, x)


    if (!fa_check(fa_audio_default_input(session))) {
        fa_print("Default input: %s\n", fa_string_to_string(default_input_name(session)));
    } else {
        fa_print("No default input\n", NULL);
    }

    if (!fa_check(fa_audio_default_output(session))) {
        fa_print("Default output: %s\n", fa_string_to_string(fa_audio_name(fa_audio_default_output(session))));
    } else {
        fa_print("No default output\n", NULL);
    }

    return session;
}

int main(int argc, char const *argv[])
{
    fa_set_log_tool();

    fa_with_faudio() {
        fa_audio_with_session(
            print_audio_devices, NULL,
            fa_error_log, NULL);
    }
}

