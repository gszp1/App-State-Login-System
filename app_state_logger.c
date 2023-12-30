#include "app_state_logger.h"

volatile atomic_int priority_level;

volatile atomic_int login_status;

int initialize_logger() {
    atomic_store(&priority_level, STANDARD);
    atomic_store(&login_status, ON);
    sigset_t signal_set;
    sigemptyset(&signal_set);
    sigaddset(&signal_set, SIGRTMIN);
    sigaddset(&signal_set, SIGRTMIN + 1);
    sigaddset(&signal_set, SIGRTMIN + 2);
    pthread_sigmask(SIG_SETMASK, &signal_set, NULL);
}

int change_login_priority(int new_priority) {
    if (new_priority < MIN || new_priority > MAX) {
        return -1;
    }
    atomic_store(&priority_level, new_priority);
    return 0;
}
