#include "boot.h"
#include "fsm.h"

void boot_run(void) {
    fsm_transition(STATE_SAFE);
}
