#define _POSIX_SOURCE
#define _GNU_SOURCE

#ifndef APP_STATE_LOGGER_H
#define APP_STATE_LOGGER_H

// includes //
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <stdatomic.h>

/* Used signals:
    SIGRTMIN - dump
    SIGRTMIN + 1 - toggle log [0, 1]
    SIGRTMIN + 2 - change login priority [0, 1, 2]
*/

// data types //
typedef enum {
    MIN = 0,
    STANDARD = 1,
    MAX = 2
} priority_level_t;

// structures //


// declarations //

int initialize_logger();

int change_login_priority(int new_priority);


#endif //ifndef APP_STATE_LOGGER_H
