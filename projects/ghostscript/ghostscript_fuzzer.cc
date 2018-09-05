#include <stddef.h>
#include <stdint.h>

#include "iapi.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    void *minst;
    int code = gsapi_new_instance(&minst, NULL);
    if (code < 0) {
        return 0;
    }

    code = gsapi_set_arg_encoding(minst, GS_ARG_ENCODING_UTF8);
    if (code != 0) {
        return 0;
    }


    char *argv[6];
    argv[0] = "gs";
    argv[1] = "-q";
    argv[2] = "-dSAFER";
    argv[3] = "-sDEVICE=ppmraw";
    argv[4] = "-sstdout=/dev/null";
    argv[5] = NULL;
    code = gsapi_init_with_args(minst, 5, argv);
    if (code != 0) {
        return 0;
    }

    int exit_code;
    gsapi_run_string_with_length(
        minst,
        reinterpret_cast<char *>(const_cast<uint8_t *>(data)),
        size,
        0,
        &exit_code
    );
    gsapi_exit(minst);
    gsapi_delete_instance(minst);

    return 0;
}
