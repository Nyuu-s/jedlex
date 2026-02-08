
#include "jedlex.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

bool mystart(JedlexCtx* ctx, JedLexToken* token, uint8 byte){
    for (int i = 0; i<7; i++) {
        add_current_byte_to_token(ctx, token);
        ctx->in_buffer_offset++;
    }
    return 0;
}


int main(){
    uint8* input = (uint8*)"hello == _world 88 (0xFF)"; // mimic a fdopen + fread into char buffer

    JedlexCtx ctx = {0}; // user handle all memory allocs
    JedLexToken token = {0};
    
    // jedlexHandler* handlers = (jedlexHandler*) malloc(sizeof(jedlexHandler) * JEDSTATE_COUNT);
    // index = state, value = function to handle state
    jedlexHandler handlers[JEDSTATE_COUNT];

    
    TODO("Setup all handlers");
    handlers[JEDSTATE_START] = mystart;

    jedlex_init_handlers(&ctx, input, strlen((char*)input), COREMODE_JUMP_TABLE, handlers, 1); // start config


    while (get_next_token(&ctx, &token)) {    
        printf("Token: %.*s | %s \n", (int)(token.end - token.start), token.start, token_kind_name(token.kind));
    }

    printf("Last Token: %.*s\n", (int)(token.end - token.start)+1, token.start);

    return 0;
}
