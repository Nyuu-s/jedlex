#ifndef JEDLEX_H
#define JEDLEX_H
//* temp
#include "stdio.h"
#include "stdlib.h"
#include <stdint.h>
#include <time.h>
#define UNUSED(value) (void)(value)
#define TODO(message) do {  fprintf(stderr, "%s:%d: TODO: %s\n", __FILE__, __LINE__, message); } while(0)
#define FATAL_TODO(message) do {  fprintf(stderr, "%s:%d: TODO: %s\n", __FILE__, __LINE__, message);exit(0); } while(0)
//***************** */
#define JEDLEX_REGISTER_VALUES(type, ...)\
__jedlex_register_values(type, ((const char*[]){__VA_ARGS__}), (sizeof((const char*[]){__VA_ARGS__})/sizeof(const char*)))

// define init function sub-behaviour
typedef unsigned int jedlexInitFlags;
#define FLG_USE_DEFAULTS    (1 << 0)
#define FLG_FSM_DISABLED    (1 << 1)
#define FLG_FSM_MINIMAL     (1 << 2)

#define JEDLEX_MAX_STATE    64
#define JEDLEX_HANDLER_EMIT 1
#define JEDLEX_HANDLER_CONTINUE 0
#define JEDLEX_HANDLER_ERROR -1

// CLASSIFIER API
#define JEDLEX_CLS_REGISTER(ds) TODO("Register user datastructure as classifier")

#define JEDLEX_CLS_INSERT(type, string, length) TODO("Implement default insert classifier")
#define JEDLEX_CLS_CLASSIFY(type, string, length) TODO("Implement default lookup classifier")
#define JEDLEX_CLS_FREE  (type, string, length) TODO("Implement default free classifier")



typedef unsigned long long   uint64;
typedef unsigned int         uint32;
typedef unsigned char        uint8;
typedef signed char          int8;
typedef short                int16;
typedef int                  int32;
typedef long long            int64;

typedef char bool;

typedef enum EJedCoreMode{
    COREMODE_SWITCH,
    COREMODE_HANDLERS,
    COREMODE_FSM_CLASSIC,
    COREMODE_FSM_MINIMAL,
    COREMODE_COUNT
} EJedCoreMode;

typedef enum EJedSwitchTokenKind{
    TOKKIND_IDENTIFIER,
    TOKKIND_NUMBER,
    TOKKIND_SYMBOL,
    TOKKIND_WHITESPACE,
    TOKKIND_UNKNOWN,
    TOKKIND_COUNT
} EJedSwitchTokenKind;

typedef enum EJedSwitchState{
    JEDSTATE_START,
    JEDSTATE_ERROR,
    JEDSTATE_IDENTIFIER,
    JEDSTATE_NUMBER,
    JEDSTATE_HEX,
    JEDSTATE_SYMBOL,
    JEDSTATE_WS,
    JEDSTATE_COUNT
} EJedSwitchState;

/*
    Token acumulation strategies:
    Dynamic array
    span / 2 ptrs start-end
*/
typedef struct JedLexToken{
    uint8* start;
    uint8* end;
    uint32 kind;
}  JedLexToken;

typedef struct JedlexCtx  JedlexCtx;
typedef int(*jedlexHandler)(JedlexCtx* ctx, JedLexToken* token, uint8 byte);

struct JedlexCtx {
    // Lexer -- zone
    const uint8* in_buffer;           //let user manage IO, and buffer alloc
    uint64 in_buffer_offset;
    uint64 inbuffer_size;

    //get next token dispatcher
    bool (*func_next_token)(JedlexCtx* ctx, JedLexToken* token);

    //switch zone
    uint32 current_state;
    //handlers zone
    jedlexHandler* state_handlers_table;
    uint64 state_handlers_count;
    bool using_defaut_handlers;
    uint64 fallback_id;
    
    
};


void jedlex_init_handlers(JedlexCtx *ctx, uint8 *input, uint64 buffer_size, EJedCoreMode core_mode);
void jedlex_init(JedlexCtx* ctx, const uint8* input, uint64 buffer_size, EJedCoreMode core_mode);
bool get_next_token(JedlexCtx* ctx, JedLexToken* token);
bool switch_get_next_token(JedlexCtx *ctx, JedLexToken* token);
bool handlers_get_next_token(JedlexCtx *ctx, JedLexToken* token);
uint8 peek_byte(JedlexCtx* ctx, uint64 offset);
uint8 get_byte(JedlexCtx* ctx, uint64 step);
bool is_alphanum(int c);
bool is_num(int c);
bool is_hex(int c);
bool is_alpha(int c);
bool is_symbol(int c);
bool is_whitespace(int c);
void add_current_byte_to_token(JedlexCtx* ctx, JedLexToken* tok);

void jedlex_set_handlers_table(JedlexCtx* ctx, jedlexHandler* handlers ,uint64 handlers_amount, uint64 fallback_id);
void jedlex_set_handler_for_state(JedlexCtx* ctx, uint32 state, jedlexHandler handler );

int32 dh_start(JedlexCtx* ctx, JedLexToken* token, uint8 byte);
int32 dh_error(JedlexCtx* ctx, JedLexToken* token, uint8 byte);
int32 dh_identifier(JedlexCtx* ctx, JedLexToken* token, uint8 byte);
int32 dh_number(JedlexCtx* ctx, JedLexToken* token, uint8 byte);
int32 dh_hex(JedlexCtx* ctx, JedLexToken* token, uint8 byte);
int32 dh_symbol(JedlexCtx* ctx, JedLexToken* token, uint8 byte);
int32 dh_whitespace(JedlexCtx* ctx, JedLexToken* token, uint8 byte);
// #ifdef JEDLEX_IMPLEMENTATION

// #############################
// #		COREMODE: SWITCH
// #############################
inline void jedlex_init(JedlexCtx* ctx, const uint8* in_buffer, uint64 buffer_size, EJedCoreMode core_mode){
    ctx->in_buffer = in_buffer;
    ctx->in_buffer_offset = 0;
    ctx->inbuffer_size = buffer_size;
    switch (core_mode) {
        case COREMODE_SWITCH:{
            ctx->func_next_token = switch_get_next_token;
            break;
        }
        case COREMODE_HANDLERS: {
            ctx->func_next_token = handlers_get_next_token;
            break;
        }
        case COREMODE_FSM_CLASSIC: FATAL_TODO("FSM classic mode (struct & array)"); break;
        case COREMODE_FSM_MINIMAL: FATAL_TODO("FSM minimal mode (row compression, 1D array & offsets)"); break;
        default:FATAL_TODO("Unsuported core mode!");break;
    }
    TODO("complete ctx init with full ctx in the future");
    TODO("Make core mode compile time switch rather than runtime");
}

const char *state_name(uint32 s) {
    switch (s) {
        case JEDSTATE_START:        return "STATE_START";
        case JEDSTATE_IDENTIFIER:   return "STATE_IDENTIFIER";
        case JEDSTATE_NUMBER:       return "STATE_NUMBER";
        case JEDSTATE_HEX:          return "STATE_HEXADECIMAL";
        case JEDSTATE_WS:           return "STATE_WHITESPACE";
        case JEDSTATE_ERROR:        return "STATE_ERROR";
        default:                    return "?UNKNOWN?";
    }
}
const char *token_kind_name(EJedSwitchTokenKind s) {
    switch (s) {
        case TOKKIND_IDENTIFIER:        return "IDENTIFIER";
        case TOKKIND_NUMBER:            return "NUMBER";
        case TOKKIND_SYMBOL:            return "SYMBOL";
        case TOKKIND_WHITESPACE:        return "WHITESPACE";
        case TOKKIND_UNKNOWN:           return "UNKNOWN";
        default:               return "?ERROR?";
    }
}
// dispatcher
inline bool get_next_token(JedlexCtx *ctx, JedLexToken* token){
    //dispatch to the get_next_token function depending on ctx init
   return ctx->func_next_token(ctx, token);
}

inline uint8 get_byte(JedlexCtx* ctx, uint64 relative_offset){
    ctx->in_buffer_offset += relative_offset;
    return peek_byte(ctx, 0);
}
inline void add_current_byte_to_token(JedlexCtx* ctx, JedLexToken* tok){
    if(ctx->inbuffer_size < ctx->in_buffer_offset+1)
        return;
    if(tok->end == NULL || tok->start == NULL){
        //first time calling it on this token
        tok->start = tok->end = (uint8*) &ctx->in_buffer[ctx->in_buffer_offset];
        return;
    }
    tok->end++;
}

inline bool is_num(int c){
    return c >= '0' && c <= '9';
}
inline bool is_hex(int c){
    return (
        is_num(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')
    );
}
inline bool is_alpha(int c){
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
inline bool isalphanum(int c){
    return is_alpha(c) || is_num(c);
}
inline bool is_whitespace(int c){
    return c == ' ' || c == '\t' || c == '\n';
}

inline bool is_symbol(int c){
    return (
         c == '?' ||
         c == ',' ||
         c == ';' ||
         c == '.' ||
         c == ':' ||
         c == '/' ||
         c == '!' ||
         c == ']' ||
         c == '+' ||
         c == '%' ||
         c == '$' ||
         c == '[' ||
         c == '=' ||
         c == '}' ||
         c == ')' ||
         c == '(' ||
         c == '{' ||
         c == '|' ||
         c == '-' ||
         c == '&' 
    );
}

inline uint8 peek_byte(JedlexCtx* ctx, uint64 relative_offset){
    if(!ctx->in_buffer) return '\0'; //or eof;
    if(!(ctx->in_buffer_offset + relative_offset >= 0) || !(ctx->in_buffer_offset+relative_offset < ctx->inbuffer_size)) return '\0';

    return ctx->in_buffer[ctx->in_buffer_offset+relative_offset];
}




inline bool switch_get_next_token(JedlexCtx *ctx, JedLexToken *token){
    token->end = 0;
    token->start = 0;
    ctx->current_state = JEDSTATE_START;
    uint32 state_counters[JEDSTATE_COUNT];
    while (ctx->in_buffer[ctx->in_buffer_offset] != '\0') {

        uint8 current_byte = peek_byte(ctx, 0);
        
        printf("state: %s, char: %c\n", state_name(ctx->current_state), current_byte);
        switch (ctx->current_state) {
            case JEDSTATE_START:{
                token->start    =   NULL; //(uint8*) ctx->in_buffer + ctx->in_buffer_offset;
                token->end      =   NULL; //(uint8*) ctx->in_buffer + ctx->in_buffer_offset;
                if(is_num(current_byte)) {
                    if(current_byte == '0'){
                        add_current_byte_to_token(ctx, token);
                        ctx->in_buffer_offset++;
                        //hex format handling
                        uint8 next = peek_byte(ctx, 0);
                        if (next == 'x' || next == 'X') {
                            add_current_byte_to_token(ctx, token);
                            ctx->in_buffer_offset++;
                            ctx->current_state = JEDSTATE_HEX;
                            break;
                        }
                    }
                    ctx->current_state = JEDSTATE_NUMBER;
                    break;
                }
                else if(is_alpha(current_byte) || current_byte == '_') {
                    add_current_byte_to_token(ctx, token); ctx->current_state = JEDSTATE_IDENTIFIER;
                    ctx->in_buffer_offset++;
                }
                else if(is_whitespace(current_byte)) {
                    ctx->in_buffer_offset++;
                    ctx->current_state = JEDSTATE_WS;
                }       
                else if(is_symbol(current_byte)) {
                    add_current_byte_to_token(ctx, token);
                    ctx->in_buffer_offset++;
                    ctx->current_state = JEDSTATE_SYMBOL;
                }
                else ctx->current_state = JEDSTATE_ERROR;
                break;
            }
            case JEDSTATE_IDENTIFIER:{
                if(is_alpha(current_byte) || is_num(current_byte) ||current_byte == '_'){
                    add_current_byte_to_token(ctx, token);
                    ctx->in_buffer_offset++;
                }
                else {
                    token->kind = TOKKIND_IDENTIFIER;
                    return 1;
                }
                break;
            }
            case JEDSTATE_NUMBER:{
                if(is_num(current_byte)) {
                    add_current_byte_to_token(ctx, token);
                    ctx->in_buffer_offset++;
                }
                else{
                    token->kind = TOKKIND_NUMBER;
                    return 1;
                }
                break;
            }
            case JEDSTATE_WS:{
                if (is_whitespace(current_byte)) {
                    ctx->in_buffer_offset++;
                }else {
                    ctx->current_state = JEDSTATE_START;
                }
                break;
            }
            case JEDSTATE_SYMBOL:{
                if (is_symbol(current_byte )
                && current_byte != '(' && current_byte != ')'
                && current_byte != '{' && current_byte != '}'
                && current_byte != '[' && current_byte != ']'  ) {
                    add_current_byte_to_token(ctx, token);
                    ctx->in_buffer_offset++;
                }
                else {
                    token->kind = TOKKIND_SYMBOL;
                    return 1;
                }
                break;
            }
            case JEDSTATE_HEX: {
                if(is_hex(current_byte)){
                    add_current_byte_to_token(ctx, token);
                    ctx->in_buffer_offset++;
                }
                else {
                    token->kind = TOKKIND_NUMBER;
                    return 1;
                }
                break;
            }
            case JEDSTATE_ERROR: {
                token->kind = TOKKIND_UNKNOWN;
                return 0;
            }
            default: ctx->current_state = JEDSTATE_ERROR; break;
        }
    }
    return 0;
}

// #############################
// #		COREMODE HANDLERS
// #############################

//only in C99+
static jedlexHandler default_handlers[JEDLEX_MAX_STATE] = {
    // [JEDSTATE_START]         = dh_start,  
    // [JEDSTATE_ERROR]         = dh_start,
    // [JEDSTATE_IDENTIFIER]    = dh_start,
    // [JEDSTATE_NUMBER]        = dh_start,
    // [JEDSTATE_HEX]           = dh_start,
    // [JEDSTATE_SYMBOL]        = dh_start,
    // [JEDSTATE_WS]            = dh_start,
};
/*************** DEFAULT HANDLERS */
inline int32 dh_identifier(JedlexCtx* ctx, JedLexToken* token, uint8 byte){
    if(is_alpha(byte) || is_num(byte) ||byte == '_'){
        add_current_byte_to_token(ctx, token);
        ctx->in_buffer_offset++;
    }
    else {
        token->kind = TOKKIND_IDENTIFIER;
        ctx->current_state = JEDSTATE_START;
        return 1;
    }
    return 0;
}

inline int32 dh_number(JedlexCtx* ctx, JedLexToken* token, uint8 byte){
    if(is_num(byte)) {
        add_current_byte_to_token(ctx, token);
        ctx->in_buffer_offset++;
    }
    else {
        token->kind = TOKKIND_NUMBER;
        ctx->current_state = JEDSTATE_START; // for coremode switch
        return 1; // for coremode handler table
    }
    return 0;
}
inline int32 dh_start(JedlexCtx* ctx, JedLexToken* token, uint8 byte){
    token->start    =   0; 
    token->end      =   0; 
    if(is_num(byte)) {
        if(byte == '0'){
            add_current_byte_to_token(ctx, token);
            ctx->in_buffer_offset++;
            //hex format handling
            uint8 next = peek_byte(ctx, 0);
            if (next == 'x' || next == 'X') {
                add_current_byte_to_token(ctx, token);
                ctx->in_buffer_offset++;
                ctx->current_state = JEDSTATE_HEX;
                return JEDLEX_HANDLER_CONTINUE;

            }
        }
        ctx->current_state = JEDSTATE_NUMBER;
    }
    else if(is_alpha(byte) || byte == '_') {
        add_current_byte_to_token(ctx, token); 
        ctx->current_state = JEDSTATE_IDENTIFIER;
        ctx->in_buffer_offset++;
    }
    else if(is_whitespace(byte)) {
        ctx->in_buffer_offset++;
        ctx->current_state = JEDSTATE_WS;
    }       
    else if(is_symbol(byte)) {
        add_current_byte_to_token(ctx, token);
        ctx->in_buffer_offset++;
        ctx->current_state = JEDSTATE_SYMBOL;
    }
    else ctx->current_state = JEDSTATE_ERROR;

    return JEDLEX_HANDLER_CONTINUE;

}

inline int32 dh_whitespace(JedlexCtx* ctx, JedLexToken* token, uint8 byte){
    if (is_whitespace(byte)) {
        ctx->in_buffer_offset++;
    }else {
        ctx->current_state = JEDSTATE_START;
    }
    return JEDLEX_HANDLER_CONTINUE;
}

inline int32 dh_symbol(JedlexCtx* ctx, JedLexToken* token, uint8 byte){
    if (is_symbol(byte) 
        && byte != '(' && byte != ')'
        && byte != '{' && byte != '}'
        && byte != '[' && byte != ']'  ) {
        add_current_byte_to_token(ctx, token);
        ctx->in_buffer_offset++;
    }
    else {
        token->kind = TOKKIND_SYMBOL;
        ctx->current_state = JEDSTATE_START;
        return JEDLEX_HANDLER_EMIT;
    }
    return JEDLEX_HANDLER_CONTINUE;
}

inline int32 dh_error(JedlexCtx* ctx, JedLexToken* token, uint8 byte){
    token->kind = TOKKIND_UNKNOWN;
    return JEDLEX_HANDLER_ERROR;
}

inline int32 dh_hex(JedlexCtx* ctx, JedLexToken* token, uint8 byte){
    if(is_hex(byte)){
        add_current_byte_to_token(ctx, token);
        ctx->in_buffer_offset++;
    }
    else {
        token->kind = TOKKIND_NUMBER;
        ctx->current_state = JEDSTATE_START;
        return JEDLEX_HANDLER_EMIT;

    }
    return JEDLEX_HANDLER_CONTINUE;
}
/******************************** */
inline void jedlex_set_handler_for_state(JedlexCtx* ctx, uint32 state, jedlexHandler handler ){
    if(ctx->using_defaut_handlers){
        if(state < 0 || state >= JEDLEX_MAX_STATE){  
            FATAL_TODO("state value cannot be bigger than JEDLEX_MAX_STATE nor less than 0");
            return;
        }
        if(state >= ctx->state_handlers_count){
            ctx->state_handlers_count = state;
        }
    }
    else {
        if(state < 0 || state >= ctx->state_handlers_count){ 
            FATAL_TODO("state value has to be a valid index");
            return;
        }
    }

    ctx->state_handlers_table[state] = handler;
} 
inline void jedlex_set_handlers_table(JedlexCtx* ctx, jedlexHandler* handlers ,uint64 handlers_amount, uint64 fallback_id){ 
    if(handlers == NULL || handlers_amount <= 0){
        //switch back to default
        ctx->state_handlers_count = JEDSTATE_COUNT;
        ctx->state_handlers_table = (jedlexHandler*) default_handlers;
        ctx->using_defaut_handlers = 1;
        ctx->fallback_id = 0;
        return;
    }
    if(fallback_id < 0 || fallback_id >= handlers_amount){
        ctx->fallback_id = 0;
    }else {
        ctx->fallback_id = fallback_id;
    }
    ctx->state_handlers_table = handlers;
    ctx->state_handlers_count = handlers_amount;
    ctx->using_defaut_handlers = 0;
};
inline void jedlex_init_handlers(
    JedlexCtx* ctx,
    uint8* input,
    uint64 buffer_size, 
    EJedCoreMode core_mode
){
    jedlex_init(ctx, input, buffer_size, core_mode);
    default_handlers[JEDSTATE_START]         = dh_start;
    default_handlers[JEDSTATE_ERROR]         = dh_error;
    default_handlers[JEDSTATE_IDENTIFIER]    = dh_identifier;
    default_handlers[JEDSTATE_NUMBER]        = dh_number;
    default_handlers[JEDSTATE_HEX]           = dh_hex;
    default_handlers[JEDSTATE_SYMBOL]        = dh_symbol;
    default_handlers[JEDSTATE_WS]            = dh_whitespace;
    ctx->state_handlers_table = (jedlexHandler*) default_handlers;
    ctx->state_handlers_count = JEDSTATE_COUNT;
    ctx->using_defaut_handlers = 1;
    return;
}


inline bool handlers_get_next_token(JedlexCtx *ctx, JedLexToken *token){
    if( ctx->in_buffer_offset >= ctx->inbuffer_size ){
        return 0;
    } 
    int32 response = JEDLEX_HANDLER_CONTINUE;
    token->start = 0;
    token->end = 0;
    token->kind = TOKKIND_UNKNOWN;
    do {
        if(ctx->current_state < 0 || ctx->current_state >= ctx->state_handlers_count) return 0;
        jedlexHandler handler = ctx->state_handlers_table[ctx->current_state];
        if(!handler){
            ctx->current_state = ctx->fallback_id;
            return JEDLEX_HANDLER_EMIT;
        }
        response = handler(ctx, token, peek_byte(ctx, 0));
    } while (ctx->in_buffer_offset < ctx->inbuffer_size && response == JEDLEX_HANDLER_CONTINUE);


    return response > JEDLEX_HANDLER_CONTINUE;
}


#endif
