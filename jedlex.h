#ifndef JEDLEX_H
#define JEDLEX_H
//* temp
#include "stdio.h"
#include "stdlib.h"
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

// CLASSIFIER API
#define JEDLEX_CLS_REGISTER(ds) TODO("Register user datastructure as classifier")

#define JEDLEX_CLS_INSERT(type, string, length) TODO("Implement default insert classifier")
#define JEDLEX_CLS_CLASSIFY(type, string, length) TODO("Implement default lookup classifier")
#define JEDLEX_CLS_FREE  (type, string, length) TODO("Implement default free classifier")

#define MAX_STATES 1024


typedef unsigned long long   uint64;
typedef unsigned int         uint32;
typedef unsigned char        uint8;
typedef char bool;

typedef enum EJedCoreMode{
    COREMODE_SWITCH,
    COREMODE_JUMP_TABLE,
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
    EJedSwitchTokenKind kind;
}  JedLexToken;

typedef struct JedlexCtx  JedlexCtx;
typedef bool(*jedlexHandler)(JedlexCtx* ctx, JedLexToken* token, uint8 byte);

struct JedlexCtx {
    // Lexer -- zone
    const uint8* in_buffer;           //let user manage IO, and buffer alloc
    uint64 in_buffer_offset;
    uint64 inbuffer_size;

    //dispatchers
    bool (*func_next_token)(JedlexCtx* ctx, JedLexToken* token);
    //switch zone
    EJedSwitchState current_state;
    //handlers zone
    bool is_config_successfull;
    jedlexHandler* state_handlers_table;
    uint64 state_handlers_count;
    
    
};


void jedlex_config_start(JedlexCtx* ctx, const uint8* input, uint64 buffer_size, EJedCoreMode core_mode);
void jedlex_config_states_handlers(JedlexCtx* ctx,jedlexHandler* handlers, uint32 size);
void jedlex_config_end(JedlexCtx* ctx);

void jedlex_init(JedlexCtx* ctx, const uint8* input, uint64 buffer_size, EJedCoreMode core_mode);
bool get_next_token(JedlexCtx* ctx, JedLexToken* token);
bool switch_get_next_token(JedlexCtx *ctx, JedLexToken* token);
bool jmptab_get_next_token(JedlexCtx *ctx, JedLexToken* token);
uint8 peek_char(JedlexCtx* ctx, uint64 offset);
bool is_alphanum(int c);
bool is_num(int c);
bool is_hex(int c);
bool is_alpha(int c);
bool is_symbol(int c);
bool is_whitespace(int c);
void advance_char(JedlexCtx* ctx, uint64 step);
void add_char_to_token(JedlexCtx* ctx, JedLexToken* tok);
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
        case COREMODE_JUMP_TABLE: {
            TODO("Jump table mode (switch with full states & actions based on a vtable)");
            ctx->func_next_token = jmptab_get_next_token;
            ctx->is_config_successfull = 0;
            break;
        }
        case COREMODE_FSM_CLASSIC: FATAL_TODO("FSM classic mode (struct & array)"); break;
        case COREMODE_FSM_MINIMAL: FATAL_TODO("FSM minimal mode (row compression, 1D array & offsets)"); break;
        default:FATAL_TODO("Unsuported core mode!");break;
    }
    TODO("complete ctx init with full ctx in the future");
    TODO("Make core mode compile time switch rather than runtime");
}

const char *state_name(EJedSwitchState s) {
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

inline void advance_char(JedlexCtx* ctx, uint64 step){
    ctx->in_buffer_offset += step;
}
inline void add_char_to_token(JedlexCtx* ctx, JedLexToken* tok){
    if(ctx->inbuffer_size < ctx->in_buffer_offset+1)
        return;
    ctx->in_buffer_offset += 1;
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

inline uint8 peek_char(JedlexCtx* ctx, uint64 offset){
    if(!ctx->in_buffer) return '\0'; //or \0;
    if(!(ctx->in_buffer_offset + offset >= 0) || !(ctx->in_buffer_offset+offset < ctx->inbuffer_size)) return EOF;

    return ctx->in_buffer[ctx->in_buffer_offset+offset];
}


inline bool switch_get_next_token(JedlexCtx *ctx, JedLexToken *token){
    token->end = 0;
    token->start = 0;
    ctx->current_state = JEDSTATE_START;
    uint32 state_counters[JEDSTATE_COUNT];
    while (ctx->in_buffer[ctx->in_buffer_offset] != '\0') {

        uint8 current_char = peek_char(ctx, 0);
        state_counters[ctx->current_state]++;

        printf("state: %s, char: %c\n", state_name(ctx->current_state), current_char);
        switch (ctx->current_state) {
            case JEDSTATE_START:{
                for (int i =0; i<JEDSTATE_COUNT; ++i) {
                    state_counters[i] = 0;
                }
                token->start    =   (uint8*) ctx->in_buffer + ctx->in_buffer_offset;
                token->end      =   (uint8*) ctx->in_buffer + ctx->in_buffer_offset;
                
                if(is_num(current_char)) {
                    if(current_char == '0'){
                        add_char_to_token(ctx, token);
                        //hex format handling
                        uint8 next = peek_char(ctx, 0);
                        if (next == 'x' || next == 'X') {
                            add_char_to_token(ctx, token);
                            ctx->current_state = JEDSTATE_HEX;
                            break;
                        }
                    }
                    ctx->current_state = JEDSTATE_NUMBER;
                    break;
                }
                else if(is_alpha(current_char) || current_char == '_') {
                    add_char_to_token(ctx, token); ctx->current_state = JEDSTATE_IDENTIFIER;
                }
                else if(is_whitespace(current_char)) {
                    advance_char(ctx, 1);ctx->current_state = JEDSTATE_WS;
                }       
                else if(is_symbol(current_char)) {
                    add_char_to_token(ctx, token);
                    ctx->current_state = JEDSTATE_SYMBOL;
                }
                else ctx->current_state = JEDSTATE_ERROR;
                break;
            }
            case JEDSTATE_IDENTIFIER:{
                if(is_alpha(current_char) || is_num(current_char) ||current_char == '_'){
                    add_char_to_token(ctx, token);
                }
                else {
                    token->kind = TOKKIND_IDENTIFIER;
                    return 1;
                }
                break;
            }
            case JEDSTATE_NUMBER:{
                if(is_num(current_char)) {
                    add_char_to_token(ctx, token);
                }
                else{
                    token->kind = TOKKIND_NUMBER;
                    return 1;
                }
                break;
            }
            case JEDSTATE_WS:{
                if (is_whitespace(current_char)) {
                    advance_char(ctx, 1);
                }else {
                    ctx->current_state = JEDSTATE_START;
                }
                break;
            }
            case JEDSTATE_SYMBOL:{
                if (is_symbol(current_char) 
                && current_char != '(' && current_char != ')'
                && current_char != '{' && current_char != '}'
                && current_char != '[' && current_char != ']'  ) {
                    add_char_to_token(ctx, token);
                }
                else {
                    token->kind = TOKKIND_SYMBOL;
                    return 1;
                }
                break;
            }
            case JEDSTATE_HEX: {
                if(is_hex(current_char)){
                    add_char_to_token(ctx, token);
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
// typedef bool(*jedlexHandler)(JedlexCtx* ctx, JedLexToken* token, uint8 byte);

inline void jedlex_config_start(JedlexCtx* ctx, const uint8* in_buffer, uint64 buffer_size, EJedCoreMode core_mode){
    ctx->is_config_successfull = 1;
    jedlex_init(ctx, in_buffer, buffer_size, core_mode);
    TODO("jedlex_config_start: maybe remove jedlex_init entirely ?");
}

inline void jedlex_config_states_handlers(JedlexCtx* ctx,jedlexHandler* handlers, uint32 size){
    if(handlers == NULL || size <= 0){
        printf("ERROR: Please provide an array of handlers for each states\n");
        ctx->is_config_successfull = 0;
        return;
    }
    ctx->state_handlers_table = handlers;
    ctx->state_handlers_count = size;
    ctx->is_config_successfull = 1;
}

inline void jedlex_config_end(JedlexCtx* ctx){
    if(!ctx->is_config_successfull){
        FATAL_TODO("PLEASE FIX LIB CONFIGURATION");
    }

}
inline bool jmptab_get_next_token(JedlexCtx *ctx, JedLexToken *token){

    token = NULL;
    bool break_loop = 0;
    uint8 current_byte = peek_char(ctx, 0);
    while (ctx->in_buffer[ctx->in_buffer_offset] != '\0' || !break_loop) {
        if(ctx->current_state >= 0 && ctx->current_state < ctx->state_handlers_count){
            break_loop = ctx->state_handlers_table[ctx->current_state](ctx, token, current_byte);
        }else {
            break_loop = 1;
        }
        advance_char(ctx, 1);
        current_byte = peek_char(ctx, 0);
    }
    return token != NULL;
}


#endif
