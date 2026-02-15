

#include "jedlex.h"
#include "stdlib.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
int main(){

    JedlexCtx ctx = {0};
    uint8* input = (uint8*)"hello == _world 88 (0xFF)"; // mimic a fdopen + fread into char buffer
    
    // jedlex_init_fsm(&ctx, input, sizeof(input), COREMODE_FSM_CLASSIC);
    printf("res: %d", (uint8)'z'-(uint8)'a');

    FSMState s1 = {0};
    FSMState s2 = {0};


    jedlex_add_state(&ctx, &s1, 10);
    jedlex_add_state(&ctx, &s2, 10);

    jedlex_add_single_range_transition(&ctx, &s1, &s2, 'a', 'z');
    jedlex_add_single_range_transition(&ctx, &s2, &s2, 'a', 'z');

    
    return 0;
}