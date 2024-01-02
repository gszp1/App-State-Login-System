#include "app_state_logger.h"

atomic_int priority_level;

atomic_int login_status;

pthread_mutex_t dump_mutex;

void initialize_logger() {
    atomic_store(&priority_level, STANDARD);
    atomic_store(&login_status, ON);
    sigset_t signal_set;
    sigemptyset(&signal_set);
    pthread_sigmask(SIG_SETMASK, &signal_set, NULL);
    add_handlers();

    pthread_mutex_init(&dump_mutex, NULL);
}

void add_handlers() {
    struct sigaction sa;
    sigfillset(&(sa.sa_mask));
    sa.sa_sigaction = handler_priority_toggle_signal;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGRTMIN + 2, &sa, NULL);

    sa.sa_handler = handler_toggle_login_signal;
    sa.sa_flags = 0;
    sigaction(SIGRTMIN + 1, &sa, NULL);
}

void write_to_login_file(const char* message, int priority_level) {
    if ((message == NULL) || (priority_level < MIN) || (priority_level > MAX)) {
        return;
    }
    if (priority_level < atomic_load(&priority_level)) {
        return;
    }
    FILE* log_file = fopen("logs.txt", "a");
    if (log_file == NULL) {
        return;
    }
    fprintf(log_file, "%s\n", message);
    fclose(log_file);
}

// handlers //

void handler_priority_toggle_signal(int signo, siginfo_t* info, void* context) {
    int new_priority_level = info->si_value.sival_int;
    if (new_priority_level < MIN || new_priority_level > MAX) {
        return;
    }
    printf("Received priority level: %d\n", new_priority_level);
    atomic_store(&priority_level, new_priority_level);
}   

void handler_toggle_login_signal(int signo) {
    int new_login_status = (atomic_load(&login_status) + 1) % 2;
    atomic_store(&login_status, new_login_status);
    printf("Toggled logins status to: %d\n", new_login_status);
}
