

#include "jedlex.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

void print_token(JedLexTokenFSM* token){
    // if(token->type == 0){
    //     printf("Token: %.*s | %s \n", (int)(token->u.end - token->u.start)+1, token->u.start, token_kind_name(token->kind));
    // }
    // else{

    printf("Token: %s | %s \n", token->buffer, token_kind_name(token->kind));
    // }
}

int main(){
    JedlexCtx ctx = {0};

    uint8* input = (uint8*)"'hel\\\\lo' == _world 88 (0xFF)"; // mimic a fdopen + fread into char buffer
    for(int i = 0; i<strlen((char*)input); i++){
        printf("%c\n", *input);
        input++;
    }
    JedLexTokenFSM token = {0};
    // jedlex_init_fsm(&ctx, input, sizeof(input), COREMODE_FSM_CLASSIC);
    printf("res: %d", (uint8)'z'-(uint8)'a');
    
    int32 STRING = 99;

    FSMState start = {0};
    FSMState ident = {.id = 1};
    FSMState string_lit = {.id = 2};
    FSMState string_esc = {.id = 3};

    jedlex_init_fsm(&ctx, input, strlen((char*)input), COREMODE_FSM_CLASSIC);
    jedlex_add_state(&ctx, &start, 10, -1);
    jedlex_add_state(&ctx, &ident, 10, TOKKIND_NUMBER);
    jedlex_add_state(&ctx, &string_lit, 10, STRING);
    jedlex_add_state(&ctx, &string_esc, 10, -1);

    jedlex_add_single_transition(&ctx, &start, &ident, '[', 1);
    jedlex_add_single_transition(&ctx, &start, &string_lit, '\'', 0);


    jedlex_add_single_range_transition(&ctx, &ident, &ident, 'a', 'z', 0);
    jedlex_add_single_range_transition(&ctx, &string_lit, &string_lit, 'a', 'z', 0);
    jedlex_add_single_transition(&ctx, &string_lit, &string_esc, '\\', 1);
    jedlex_add_single_transition(&ctx, &string_esc, &string_lit, '\\', 0); // <-- add more
    jedlex_add_single_transition(&ctx, &string_esc, &string_lit, '"', 0); // <-- add more
    jedlex_add_single_transition(&ctx, &string_esc, &string_lit, '\'', 0); // <-- add more
    jedlex_add_single_transition(&ctx, &string_lit, &string_lit, '\'', 0); // <-- add more
    
    while(get_next_token_fsm(&ctx, &token)){
        // printf("Token: %.*s | %s \n", (int)(token.end - token.start)+1, token.start, token_kind_name(token.kind));
        print_token(&token);
    }
    print_token(&token);
    // printf("Last Token: %.*s\n", (int)(token.end - token.start)+1, token.start);
    return 0;
}