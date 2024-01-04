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
    
    sv.sival_int = MAX;
    sigqueue(getpid(), SIGRTMIN + 2, sv);
    write_to_login_file("HALLO DIE WELT!MA", MAX);
    write_to_login_file("HALLO DIE WELT!ST", STANDARD);
    write_to_login_file("HALLO DIE WELT!MI", MIN);

    sv.sival_int = MAX;
    sigqueue(getpid(), SIGRTMIN + 1, sv);
    write_to_login_file("HALLO DIE WELT!CHIPI", MAX);
    write_to_login_file("HALLO DIE WELT!CHIPI", STANDARD);
    write_to_login_file("HALLO DIE WELT!CHAPA", MIN);

    sv.sival_int = MIN;
    sigqueue(getpid(), SIGRTMIN + 2, sv);
    sigqueue(getpid(), SIGRTMIN + 1, sv);
    write_to_login_file("HALLO DIE WELT!DUBI", MAX);
    write_to_login_file("HALLO DIE WELT!DUBI", STANDARD);
    write_to_login_file("HALLO DIE WELT!LABA", MIN);
        char* data = calloc(12, sizeof(char));  
        sprintf(data, "Hello world");
        change_dump_data(data, 11);
    for (int i = 0; i < 100; ++i) {
        // char* data = calloc(12, sizeof(char));  
        // sprintf(data, "Hello world");
        // change_dump_data(data, 11);
        sigqueue(getpid(), SIGRTMIN, sv);
    }
    int j = 0;
    for (int i = 0; i < 100; ++i) {
        sv.sival_int = MAX;
        sigqueue(getpid(), SIGRTMIN + 2, sv);
        if (j == 0) {
            char* data2 = calloc(12, sizeof(char));
            sprintf(data2, "hallo"); 
            change_dump_data(data2, 5);
            sigqueue(getpid(), SIGRTMIN, sv);
            ++j;
        } else if (j == 1) {
            data = calloc(12, sizeof(char));  
            sprintf(data, "Hello world");
            change_dump_data(data, 11);
            sigqueue(getpid(), SIGRTMIN, sv);
            --j;
        }
        sigqueue(getpid(), SIGRTMIN + 1, sv);
        write_to_login_file("HALLO DIE WELT!DUBI", STANDARD);
         sv.sival_int = MIN;
        sigqueue(getpid(), SIGRTMIN + 2, sv);
            write_to_login_file("HALLO DIE WELT!DUBI", STANDARD);
    }
   
    while(getchar() != 'q');
    destroy_logger();
}