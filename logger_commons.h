#define _POSIX_SOURCE
#define _GNU_SOURCE

#ifndef LOGGER_COMMONS_H
#define LOGGER_COMMONS_H

// includes //
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

// data types //
typedef enum {
    MIN = 0,
    STANDARD = 1,
    MAX = 2
} priority_level_t;

// structures //


// declarations //

int initialize_logger();


#endif //ifndef LOGGER_COMMONS_H
