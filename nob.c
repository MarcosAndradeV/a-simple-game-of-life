#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

int main(int argc, char** argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    if(!mkdir_if_not_exists("build")) return 1;

    Cmd cmd = {0};
    nob_cc(&cmd);
    nob_cc_inputs(&cmd, "jewel.c");
    nob_cc_output(&cmd, "build/jewel");
    nob_cc_flags(&cmd);
    cmd_append(&cmd, "-lraylib", "-lGL", "-lm", "-lpthread", "-ldl", "-lrt", "-lX11");
    if(!cmd_run_sync_and_reset(&cmd)) return 1;

    if(argc <= 1) return 0;

    const char* arg = shift(argv, argc);

    if(strcmp(arg, "run")) {
        cmd_append(&cmd, "build/jewel");
        if(!cmd_run_sync_and_reset(&cmd)) return 1;
    }

    return 0;
}
