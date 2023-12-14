#include "logger_commons.h"

int main(int argc, char** argv) {
    sigset_t signal_set;
    sigfillset(&signal_set);
    pthread_sigmask(SIG_SETMASK, &signal_set, NULL);
}