#define _POSIX_SOURCE
#define _GNU_SOURCE

#ifndef APP_STATE_LOGGER_H
#define APP_STATE_LOGGER_H

// Includes //
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <stdatomic.h>
#include <semaphore.h>
#include <errno.h>

/* Used signals:
    SIGRTMIN - dump
    SIGRTMIN + 1 - toggle log [0, 1]
    SIGRTMIN + 2 - change login priority [0, 1, 2]
*/

// Data types //

// Enumerate for levels of priority.
typedef enum {
    MIN = 0,
    STANDARD = 1,
    MAX = 2
} priority_level_t;

// Enumerate for login status
typedef enum {
    OFF = 0,
    ON = 1
} login_status_t;

// Structures //

// Structure for data saved in dump file.
typedef struct {
    void* dump_area;
    long size;
} dump_data_t;

// Function declarations //

// Function for logger initializer.
void initialize_logger();

// Function for adding handlers.
void add_handlers();

// Function for writing logs into log file.
void write_to_login_file(const char* message, int priority);

// Function for changing dump contents.
void change_dump_data(void* data, long size);

// Function for thread creating dump files.
void* dump_area(void* arg);

// Function for freeing all allocated resources.
void destroy_logger();

// Signal handlers declarations //

// Handler for changing priority (SIGRTMIN + 2)
void handler_toggle_login_signal(int signo);

// Handler for toggling login (SIGRTMIN + 1)
void handler_priority_toggle_signal(int signo, siginfo_t* info, void* context);

// Handler for dump creation signal (SIGRTMIN)
void handler_create_dump_file_signal(int signo);

#endif // ifndef APP_STATE_LOGGER_H
