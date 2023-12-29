#include "app_state_logger.h"

atomic_int priority_level;

int initialize_logger() {
    
}

int change_login_priority(int new_priority) {
    if (new_priority < MIN || new_priority > MAX) {
        return -1;
    }
    atomic_store(&priority_level, new_priority);
    return 0;
}
