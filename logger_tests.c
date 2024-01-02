#include "app_state_logger.h"

int main() {
    initialize_logger();

    union sigval sv;
    sv.sival_int = 1;
    sigqueue(getpid(), SIGRTMIN + 1, sv);
    printf("Signal SIGRTMIN+1, with int value of 1 sent.");
    
}