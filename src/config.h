#ifndef __CONFIG_H_
#define __CONFIG_H_

#define VERSION "0.0.1"

struct version
{
    int major;
    int minor;
    int patch;
};

static const struct version version = {
    .major = 0,
    .minor = 0,
    .patch = 1
};

#define STATUS_FILE "status.json"

/*
#define DEBUG
*/

#endif

