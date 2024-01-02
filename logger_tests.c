#include "app_state_logger.h"

int main() {
    initialize_logger();

    union sigval sv;
    sv.sival_int = STANDARD;
    sigqueue(getpid(), SIGRTMIN + 2, sv);
    write_to_login_file("Hello worldMAX!", MAX);
    write_to_login_file("Hello worldMIN!", MIN);
    write_to_login_file("Hello worldSTANDARD!", STANDARD);
    sv.sival_int = 0;
    // sigqueue(getpid(), SIGRTMIN + 1, sv);
    
    // sigqueue(getpid(), SIGRTMIN, sv);

    // char* data = calloc(sizeof(char), 12);
    // sprintf(data, "Hello world");
    // change_dump_data(data, 12);
    sigqueue(getpid(), SIGRTMIN, sv);
    // sigqueue(getpid(), SIGRTMIN, sv);
    // sigqueue(getpid(), SIGRTMIN, sv);
    destroy_logger();
}