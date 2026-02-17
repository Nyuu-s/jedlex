

#include "jedlex.h"
#include "stdlib.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
int main(){

    JedlexCtx ctx = {0};
    uint8* input = (uint8*)"hello == _world 88 (0xFF)"; // mimic a fdopen + fread into char buffer
    JedLexToken token = {0};
    // jedlex_init_fsm(&ctx, input, sizeof(input), COREMODE_FSM_CLASSIC);
    printf("res: %d", (uint8)'z'-(uint8)'a');

    FSMState start = {0};
    FSMState s2 = {.id = 1};

    jedlex_init_fsm(&ctx, input, strlen((char*)input), COREMODE_FSM_CLASSIC);
    jedlex_add_state(&ctx, &start, 10);
    jedlex_add_state(&ctx, &s2, 10);

    jedlex_add_single_range_transition(&ctx, &start, &s2, 'a', 'z');
    jedlex_add_single_range_transition(&ctx, &s2, &s2, 'a', 'z');

    while(get_next_token(&ctx, &token)){
        printf("Token: %.*s | %s \n", (int)(token.end - token.start)+1, token.start, token_kind_name(token.kind));
    }

    printf("Last Token: %.*s\n", (int)(token.end - token.start)+1, token.start);
    return 0;
}