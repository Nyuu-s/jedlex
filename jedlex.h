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


typedef struct JedLexToken{}  JedLexToken;
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
    
    ctx->current_state = JEDSTATE_START;
    TODO("finish switch states");
    while (ctx->in_buffer[ctx->in_buffer_offset] != '\0') {
        char current_char = peek_char(ctx, 0);
        printf("cur char = %c\n", current_char);

        TODO("Callback check ON_STATE_ENTER");
        switch (ctx->current_state) {
            case JEDSTATE_START:{
                if(is_num(current_char)) ctx->current_state = JEDSTATE_NUMBER;
                else if(is_alpha(current_char)) ctx->current_state = JEDSTATE_IDENTIFIER;
                else ctx->current_state = JEDSTATE_ERROR;
 
                printf("state: %s\n", state_name(ctx->current_state));
                break;
            }
            case JEDSTATE_IDENTIFIER:{
                // TODO("in identifier ifs logic");
                printf("state: %s, char: %c\n", state_name(ctx->current_state), current_char);
                advance_char(ctx, 1);
                break;
            }
            case JEDSTATE_NUMBER:{
                printf("state: %s, char: %c\n", state_name(ctx->current_state), current_char);
                if(is_num(current_char)) advance_char(ctx, 1);
                // is in hex form 0x/0X
                else if(current_char == 'x' || current_char == 'X') advance_char(ctx, 1);
                else ctx->current_state = JEDSTATE_ERROR;
                break;
            }
            case JEDSTATE_ERROR: return (JedLexToken){};
            default: ctx->current_state = JEDSTATE_ERROR; break;
        }
    }
    return (JedLexToken){};
}


#endif
