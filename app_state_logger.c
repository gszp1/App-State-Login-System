#include "app_state_logger.h"

volatile atomic_int priority_level;

volatile atomic_int login_status;

void initialize_logger() {
    atomic_store(&priority_level, STANDARD);
    atomic_store(&login_status, ON);
    sigset_t signal_set;
    sigemptyset(&signal_set);
    pthread_sigmask(SIG_SETMASK, &signal_set, NULL);
}

void add_handlers() {
    struct sigaction sa;
    sa.sa_sigaction = handler_priority_toggle_signal;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGRTMIN + 1, &sa, NULL);
}

// handlers //

void handler_priority_toggle_signal(int signo, siginfo_t* info, void* context) {
    int new_priority_level = info->si_value.sival_int;
    if (new_priority_level < MIN || new_priority_level > MAX) {
        return;
    }
    atomic_store(&priority_level, new_priority_level);
}   
