#include "app_state_logger.h"

atomic_int priority_level;

atomic_int login_status;

sem_t dump_semaphore;

pthread_mutex_t data_modification_mutex;

dump_data_t dump_data;

void initialize_logger() {
    atomic_store(&priority_level, STANDARD);
    atomic_store(&login_status, ON);
    sigset_t signal_set;
    sigemptyset(&signal_set);
    pthread_sigmask(SIG_SETMASK, &signal_set, NULL);
    add_handlers();

    sem_init(&dump_semaphore, 0, 0);
    pthread_mutex_init(&data_modification_mutex, NULL);

    dump_data.dump_area = NULL;
    dump_data.size = 0;

    pthread_t thread;
    pthread_create(&thread, NULL, dump_area, (void*)(&dump_data));
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

    sa.sa_handler = handler_create_dump_file_signal;
    sigaction(SIGRTMIN, &sa, NULL);
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

void change_dump_data(void* data, long size) {
    pthread_mutex_lock(&data_modification_mutex);
    dump_data.dump_area = data;
    dump_data.size = size;
    pthread_mutex_unlock(&data_modification_mutex);
}

void* dump_area(void* arg) {
    dump_data_t* data = (dump_data_t*)arg;
    char file_name[128] = {};
    char* write_ptr;
    while(1) {
        sem_wait(&dump_semaphore);
        if (data->size == 0) {
            continue;
        }
        pthread_mutex_lock(&data_modification_mutex);
        sprintf(file_name, "dump");
        write_ptr = data->dump_area;
        FILE* dump_file = fopen(file_name, "w");
        if (dump_file == NULL) {
            pthread_mutex_unlock(&data_modification_mutex);
            continue;
        }
        int counter = 0;
        while (counter < data->size) {
            fputc(*(write_ptr + counter), dump_file);
        }
        fclose(dump_file);
        pthread_mutex_unlock(&data_modification_mutex);
    }

}

// handlers //

void handler_priority_toggle_signal(int signo, siginfo_t* info, void* context) {
    int new_priority_level = info->si_value.sival_int;
    if (new_priority_level < MIN || new_priority_level > MAX) {
        return;
    }
    atomic_store(&priority_level, new_priority_level);
}   

void handler_toggle_login_signal(int signo) {
    int new_login_status = (atomic_load(&login_status) + 1) % 2;
    atomic_store(&login_status, new_login_status);
}

void handler_create_dump_file_signal(int signo) {
    sem_post(&dump_semaphore);
}