#include "app_state_logger.h"

volatile atomic_int priority_level;

volatile atomic_int login_status;

void initialize_logger() {
    atomic_store(&priority_level, STANDARD);
    atomic_store(&login_status, ON);
    sigset_t signal_set;
    sigemptyset(&signal_set);
    sigaddset(&signal_set, SIGRTMIN);
    sigaddset(&signal_set, SIGRTMIN + 1);
    sigaddset(&signal_set, SIGRTMIN + 2);
    pthread_sigmask(SIG_SETMASK, &signal_set, NULL);
}

// handlers //

void handler_priority_toggle_signal(int signo, siginfo_t* info, void* context) {
    int new_priority_level = info->si_value.sival_int;
    if (new_priority_level < MIN || new_priority_level > MAX) {
        return;
    }
    atomic_store(&priority_level, new_priority_level);
}   
