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
    JEDSTATE_EMIT,
    JEDSTATE_ERROR,
    JEDSTATE_IN_IDENT,
    JEDSTATE_IN_WS,
    JEDSTATE_COUNT
} EJedSwitchState;

typedef struct JedlexCtx {
    // Lexer -- zone
    const uint8* in_buffer;           //let user manage IO, and buffer alloc
    uint64 istream_offset;

    //switch zone
    EJedSwitchState current_state;

} JedlexCtx;

typedef struct JedLexToken{} JedLexToken;
JedLexToken jedlex_get_next_token(JedlexCtx* ctx);

void jedlex_init(JedlexCtx* ctx, const uint8* input, EJedCoreMode core_mode);

// #ifdef JEDLEX_IMPLEMENTATION

inline void jedlex_init(JedlexCtx* ctx, const uint8* in_buffer, EJedCoreMode core_mode){
    ctx->in_buffer = in_buffer;
    ctx->istream_offset = 0;
    switch (core_mode) {
        case COREMODE_SWITCH: TODO("switch mode (basic switchs with possible hook ovveride)");break;
        case COREMODE_JUMP_TABLE: FATAL_TODO("Jump table mode (switch with full states & actions based on a vtable)"); break;
        case COREMODE_FSM_CLASSIC: FATAL_TODO("FSM classic mode (struct & array)"); break;
        case COREMODE_FSM_MINIMAL: FATAL_TODO("FSM minimal mode (row compression, 1D array & offsets)"); break;
        default:FATAL_TODO("Unsuported core mode!");break;
    }
    TODO("complete ctx init with full ctx in the future");
    TODO("Make core mode compile time switch rather than runtime");
}

inline JedLexToken jedlex_get_next_token(JedlexCtx *ctx){
    ctx->current_state = JEDSTATE_START;
    TODO("finish switch states");
    while (ctx->in_buffer[ctx->istream_offset] != '\0') {
        switch (ctx->current_state) {
            case JEDSTATE_START:{
                TODO("start state ifs logic");
                ctx->current_state = JEDSTATE_ERROR;
                break;
            }
            case JEDSTATE_IN_IDENT:{
                TODO("in identifier ifs logic");
                ctx->current_state = JEDSTATE_ERROR;
                break;
            }
            case JEDSTATE_ERROR: return (JedLexToken){};
            default: ctx->current_state = JEDSTATE_ERROR; break;
        }
    }
    return (JedLexToken){};
}


#endif
