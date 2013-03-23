#include <stdio.h>

#include "config.h"
#include "utils.h"
#include "check.h"

static config_t config = 
{
    .argv = { "./under", "./under", NULL },
    .cpu_limit    = 1000,
    .memory_limit = 256 * 1024,
};

int
main(int argc, char **argv)
{
    result_t result;
    int out;

    init_utils(argc, argv);
    printf("bound/" VERSION "\n");
    
    out = run(RUN_STANDALONE, &config, &result);

    step("result.cpu_time = %lf s.", result.cpu_time);
    step("result.memory   = %lu kb", result.memory);

    return 0;
}
