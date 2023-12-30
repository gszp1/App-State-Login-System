#include "app_state_logger.h"

volatile atomic_int priority_level;

volatile atomic_int login_status;

int initialize_logger() {
    atomic_store(&priority_level, STANDARD);
    atomic_store(&login_status, ON);
    
}

int change_login_priority(int new_priority) {
    if (new_priority < MIN || new_priority > MAX) {
        return -1;
    }
    atomic_store(&priority_level, new_priority);
    return 0;
}
