#include "app_state_logger.h"

atomic_int priority_level;

atomic_int login_status;

atomic_int thread_stop;

sem_t dump_semaphore;

pthread_mutex_t data_modification_mutex;

pthread_mutex_t log_file_modification_mutex;

dump_data_t dump_data;

pthread_t thread;

// Functions //

//Function for logger initializer.
void initialize_logger() {
    atomic_store(&priority_level, STANDARD);
    atomic_store(&login_status, ON);
    sigset_t signal_set;
    sigemptyset(&signal_set);
    for (int i = 0; i < 3; ++i) {
       sigdelset(&signal_set, SIGRTMIN + i);
    }
    pthread_sigmask(SIG_UNBLOCK, &signal_set, NULL);
    add_handlers();

    sem_init(&dump_semaphore, 0, 0);
    pthread_mutex_init(&data_modification_mutex, NULL);
    pthread_mutex_init(&log_file_modification_mutex, NULL);
    dump_data.dump_area = NULL;
    dump_data.size = 0;

    atomic_store(&thread_stop, 0);
    pthread_create(&thread, NULL, dump_area, (void*)(&dump_data));
}

// Function for adding handlers.
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

// Function for writing logs into log file.
void write_to_login_file(const char* message, int priority) {
    if ((message == NULL) || (priority < MIN) || (priority > MAX)) {
        return;
    }
    if ((priority < atomic_load(&priority_level)) || (atomic_load(&login_status) == OFF)) {
        return;
    }
    pthread_mutex_lock(&log_file_modification_mutex);
    FILE* log_file = fopen("logs.txt", "a");
    if (log_file == NULL) {
        return;
    }
    fprintf(log_file, "%s\n", message);
    fclose(log_file);
    pthread_mutex_unlock(&log_file_modification_mutex);
}

// Function for changing dump contents.
void change_dump_data(void* data, long size) {
    pthread_mutex_lock(&data_modification_mutex);
    dump_data.dump_area = data;
    dump_data.size = size;
    pthread_mutex_unlock(&data_modification_mutex);
}

// Function for thread creating dump files.
void* dump_area(void* arg) {
    sigset_t mask;
    sigfillset(&mask);
    pthread_sigmask(SIG_BLOCK, &mask, NULL);
    dump_data_t* data = (dump_data_t*)arg;
    char file_name[128] = {};
    char* write_ptr;
    while(atomic_load(&thread_stop) != 1) {
        sem_wait(&dump_semaphore);
        if (data->size == 0) {
            continue;
        }
        pthread_mutex_lock(&data_modification_mutex);
        // Check for file with largest number at the end
        int largest_suffix = 1;
        while (1) {
            sprintf(file_name, "%s%d", "dump", largest_suffix);
            FILE* file = fopen(file_name, "r");
            if (file == NULL) {
                break;
            }
            fclose(file);
            ++largest_suffix;
        }
        write_ptr = data->dump_area;
        FILE* dump_file = fopen(file_name, "w");
        if (dump_file == NULL) {
            pthread_mutex_unlock(&data_modification_mutex);
            continue;
        }
        int counter = 0;
        while (counter < data->size) {
            fputc(*(write_ptr + counter), dump_file);
            ++counter;
        }
        fclose(dump_file);
        pthread_mutex_unlock(&data_modification_mutex);
    }
}

// function for freeing all allocated resources.
void destroy_logger() {
    atomic_store(&thread_stop, 1);
    pthread_cancel(thread);
    sem_destroy(&dump_semaphore);
    pthread_mutex_destroy(&data_modification_mutex);
}


// handlers //

//handler for changing priority (SIGRTMIN + 2)
void handler_priority_toggle_signal(int signo, siginfo_t* info, void* context) {
    int new_priority_level = info->si_value.sival_int;
    if (new_priority_level < MIN || new_priority_level > MAX) {
        return;
    }
    atomic_store(&priority_level, new_priority_level);
}

//handler for toggling login (SIGRTMIN + 1)
void handler_toggle_login_signal(int signo) {
    int new_login_status = (atomic_load(&login_status) + 1) % 2;
    atomic_store(&login_status, new_login_status);
}

// handler for dump creation signal (SIGRTMIN)
void handler_create_dump_file_signal(int signo) {
    sem_post(&dump_semaphore);
}