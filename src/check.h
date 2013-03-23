#ifndef __CHECK_H_
#define __CHECK_H_

#include "config.h"

typedef enum
{
    RUN_STANDALONE,
} runtype_t;

typedef struct
{
    /*
     * Arguments to child process
     *
     * Note that argv must be NULL-terminated e.g.
     *  .argv = { "one", "two", NULL }
     */
    char *argv[100];

    char allow_stdio;
    char *allowed_files[100];

    long memory_limit;
    long cpu_limit;

} config_t;

typedef struct
{
    char            terminated;
    double          cpu_time;
    unsigned long   memory;
} result_t;

int
run(runtype_t type, config_t *config, result_t *result);

#endif

