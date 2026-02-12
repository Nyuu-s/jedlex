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
    return 0;
}