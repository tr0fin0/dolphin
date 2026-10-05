#include "boot.h"
#include "fsm.h"

void boot_run(void) {
    return fsm_transition(FSM_STATE_SAFE);
}
