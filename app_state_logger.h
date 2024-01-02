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
#include <semaphore.h>

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

typedef enum {
    OFF = 0,
    ON = 1
} login_status_t;

// structures //

typedef struct {
    void* dump_area;
    long size;
} dump_data_t;

// function declarations //

void initialize_logger();

void add_handlers();

void write_to_login_file(const char* message, int priority_level);

void change_dump_data(void* data, long size);

void* dump_area(void* arg);

// signal handlers declarations //

void handler_toggle_login_signal(int signo);

void handler_priority_toggle_signal(int signo, siginfo_t* info, void* context);

void handler_create_dump_file_signal(int signo);

#endif //ifndef APP_STATE_LOGGER_H
