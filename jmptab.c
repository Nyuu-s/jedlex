
#include "jedlex.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>


int32 mystart(JedlexCtx* ctx, JedLexToken* token, uint8 byte){
    for (int i = 0; i<7; i++) {
        add_current_byte_to_token(ctx, token);
        ctx->in_buffer_offset++;
    }
    return JEDLEX_HANDLER_EMIT;
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

    //Init a default handler table 
    jedlex_init_handlers(&ctx, input, strlen((char*)input), COREMODE_HANDLERS); // start config
    // jedlex_set_handlers_table(&ctx, handlers, 1, 0);
    
    /*
        Option A replace default array with user array
            jedlex_set_handlers_table(&ctx, handlers, 1 , 0 );
        
        Option B: extends current array (or default one)
            jedlex_set_handler_for_state(&ctx, JEDSTATE_START, mystart);
    
    */ 
    
    while (get_next_token(&ctx, &token)) {    
        printf("Token: %.*s | %s \n", (int)(token.end - token.start)+1, token.start, token_kind_name(token.kind));
    }

    printf("Last Token: %.*s\n", (int)(token.end - token.start)+1, token.start);

    return 0;
}
