
#include "jedlex.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>




int main(){
    uint8* input = (uint8*)"hello == _world 88 (0xFF)"; // mimic a fdopen + fread into char buffer

    JedlexCtx ctx = {0}; // user handle all memory allocs
    JedLexToken token = {0};
    jedlex_init(&ctx, input, strlen((char*)input), COREMODE_JUMP_TABLE);
    
    
    while (get_next_token(&ctx, &token)) {    
        printf("Token: %.*s | %s \n", (int)(token.end - token.start), token.start, token_kind_name(token.kind));
    }

    printf("Last Token: %.*s\n", (int)(token.end - token.start), token.start);

    return 0;
}
