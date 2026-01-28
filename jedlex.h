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

typedef enum EJedTokenType{
    TOKTYPE_IDENTIFIER,
    TOKTYPE_PUNCTUATION,
    TOKTYPE_WHITESPACE,
    TOKTYPE_COUNT
} EJedTokenType;

typedef enum EJedSwitchState{
    JEDSTATE_START,
    JEDSTATE_ERROR,
    JEDSTATE_IDENTIFIER,
    JEDSTATE_KEYWORD,
    JEDSTATE_NUMBER,
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
}  JedLexToken;

typedef struct JedlexCtx    JedlexCtx ;

struct JedlexCtx {
    // Lexer -- zone
    const uint8* in_buffer;           //let user manage IO, and buffer alloc
    uint64 in_buffer_offset;
    uint64 inbuffer_size;
    JedLexToken (*func_next_token)(JedlexCtx* ctx);
    //switch zone
    EJedSwitchState current_state;
    
};

void jedlex_init(JedlexCtx* ctx, const uint8* input, uint64 buffer_size, EJedCoreMode core_mode);
JedLexToken get_next_token(JedlexCtx* ctx);
JedLexToken switch_get_next_token(JedlexCtx *ctx);
uint8 peek_char(JedlexCtx* ctx, uint64 offset);
bool is_alphanum(char c);
bool is_num(char c);
bool is_alpha(char c);
void advance_char(JedlexCtx* ctx, uint64 step);
void add_char_to_token(JedlexCtx* ctx, JedLexToken* tok);
// #ifdef JEDLEX_IMPLEMENTATION

inline void jedlex_init(JedlexCtx* ctx, const uint8* in_buffer, uint64 buffer_size, EJedCoreMode core_mode){
    ctx->in_buffer = in_buffer;
    ctx->in_buffer_offset = 0;
    ctx->inbuffer_size = buffer_size;
    switch (core_mode) {
        case COREMODE_SWITCH:{
            TODO("switch mode (basic switchs with possible hook ovveride)");
            ctx->func_next_token = switch_get_next_token;
            break;
        }
        case COREMODE_JUMP_TABLE: FATAL_TODO("Jump table mode (switch with full states & actions based on a vtable)"); break;
        case COREMODE_FSM_CLASSIC: FATAL_TODO("FSM classic mode (struct & array)"); break;
        case COREMODE_FSM_MINIMAL: FATAL_TODO("FSM minimal mode (row compression, 1D array & offsets)"); break;
        default:FATAL_TODO("Unsuported core mode!");break;
    }
    TODO("complete ctx init with full ctx in the future");
    TODO("Make core mode compile time switch rather than runtime");
}

const char *state_name(EJedSwitchState s) {
    switch (s) {
        case JEDSTATE_START:      return "STATE_START";
        case JEDSTATE_IDENTIFIER: return "STATE_IDENTIFIER";
        case JEDSTATE_NUMBER:     return "STATE_NUMBER";
        case JEDSTATE_WS:       return "STATE_WHITESPACE";
        case JEDSTATE_ERROR:      return "STATE_ERROR";
        default:               return "?UNKNOWN?";
    }
}

// dispatcher
inline JedLexToken get_next_token(JedlexCtx *ctx){
    //dispatch to the get_next_token function depending on ctx init
   return ctx->func_next_token(ctx);
}

inline void advance_char(JedlexCtx* ctx, uint64 step){
    ctx->in_buffer_offset += step;
}
inline void add_char_to_token(JedlexCtx* ctx, JedLexToken* tok){
    printf("%llu  <= %llu : %d\n", ctx->inbuffer_size, ctx->in_buffer_offset+1,  ctx->inbuffer_size <= ctx->in_buffer_offset+1);
    if(ctx->inbuffer_size <= ctx->in_buffer_offset+1)
        return;
    ctx->in_buffer_offset += 1;
    tok->end++;

    printf("ptr:%p\n", tok->end);

}

inline bool is_num(char c){
    return c >= '0' && c <= '9';
}
inline bool is_alpha(char c){
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
inline bool isalphanum(char c){
    return is_alpha(c) || is_num(c);
}


inline uint8 peek_char(JedlexCtx* ctx, uint64 offset){
    if(!ctx->in_buffer) return EOF; //or \0;
    if(!(ctx->in_buffer_offset + offset >= 0) || !(ctx->in_buffer_offset+offset < ctx->inbuffer_size)) return EOF;

    return ctx->in_buffer[ctx->in_buffer_offset+offset];
}


inline JedLexToken switch_get_next_token(JedlexCtx *ctx){

    TODO("Callback check ON_CHAR");
    JedLexToken result = {NULL, NULL};
    ctx->current_state = JEDSTATE_START;
    uint32 state_counters[JEDSTATE_COUNT];
    TODO("finish switch states");
    while (ctx->in_buffer[ctx->in_buffer_offset] != '\0') {

        char current_char = peek_char(ctx, 0);
        state_counters[ctx->current_state]++;

        TODO("Callback check ON_STATE_ENTER");
        printf("state: %s, char: %c\n", state_name(ctx->current_state), current_char);
        switch (ctx->current_state) {
            case JEDSTATE_START:{
                for (int i =0; i<JEDSTATE_COUNT; ++i) {
                    state_counters[i] = 0;
                }
                result.start    =   (uint8*) ctx->in_buffer + ctx->in_buffer_offset;
                result.end      =   (uint8*) ctx->in_buffer + ctx->in_buffer_offset;
                
                printf("%p %p\n", result.start, result.end);
                if(is_num(current_char)) ctx->current_state = JEDSTATE_NUMBER;
                else if(is_alpha(current_char) || current_char == '_') ctx->current_state = JEDSTATE_IDENTIFIER;
                else ctx->current_state = JEDSTATE_ERROR;
 
                break;
            }
            case JEDSTATE_IDENTIFIER:{
                if(is_alpha(current_char) || is_num(current_char) ||current_char == '_'){
                    add_char_to_token(ctx, &result);
                }
                else return result;
                break;
            }
            case JEDSTATE_NUMBER:{
                if(current_char == '0'){
                    add_char_to_token(ctx, &result);
                    uint8 next = peek_char(ctx, 0);
                    if (next == 'x' || next == 'X') {
                        add_char_to_token(ctx, &result);
                    }
                }
                else if(is_num(current_char)) {
                    add_char_to_token(ctx, &result);
                    printf("ptr:%p\n", result.end);
                }
                else return result;
                break;
            }
            case JEDSTATE_ERROR: return result;
            default: ctx->current_state = JEDSTATE_ERROR; break;
        }
    }
    return result;

}


#endif
