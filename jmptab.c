
#include "jedlex.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

void mystart(JedlexCtx* ctx, JedLexToken* token, uint8 byte){
    return;
}


int main(){
    uint8* input = (uint8*)"hello == _world 88 (0xFF)"; // mimic a fdopen + fread into char buffer

    JedlexCtx ctx = {0}; // user handle all memory allocs
    JedLexToken token = {0};
    
    // jedlexHandler* handlers = (jedlexHandler*) malloc(sizeof(jedlexHandler) * JEDSTATE_COUNT);
    // index = state, value = function to handle state
    jedlexHandler handlers[] = {
        mystart
    };

    
    TODO("Setup all handlers");
    handlers[JEDSTATE_START] = mystart;

    jedlex_config_start(&ctx, input, strlen((char*)input), COREMODE_JUMP_TABLE); // start config
    jedlex_config_states_handlers(&ctx, handlers, 1); // overwrite default states with user states
    jedlex_config_end(&ctx);


    
    // when nothing ovrd: use full default 
    // when only states are overidden expect states[CURRENT_STATE] ==> function_pointer(ctx, token, char current_char)

    // jedlex_init(&ctx, input, strlen((char*)input), COREMODE_JUMP_TABLE);
    
    
    while (get_next_token(&ctx, &token)) {    
        printf("Token: %.*s | %s \n", (int)(token.end - token.start), token.start, token_kind_name(token.kind));
    }

    printf("Last Token: %.*s\n", (int)(token.end - token.start), token.start);

    return 0;
}
