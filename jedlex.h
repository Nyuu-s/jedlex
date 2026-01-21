#ifndef JEDLEX_H
#define JEDLEX_H
//* temp
#include "stdio.h"
#include "stdlib.h"
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

typedef unsigned long u64;
typedef unsigned int u32;

typedef struct JedlexToken{
    char* value;
    void* type;
} JedToken;

typedef struct JedlexDFSMState{
    u32 id;


}JedlexDFSMState;

typedef struct JedlexCtx {
    // Lexer -- zone
    const char* in_stream;           //let user manage IO, and buffer alloc
    u64 istream_offset;

} JedlexCtx ;


void jedlex_init(JedlexCtx* ctx, const char* input, jedlexInitFlags flags);

// #ifdef JEDLEX_IMPLEMENTATION

inline void jedlex_init(JedlexCtx* ctx, const char* in_buffer, jedlexInitFlags flags){
    ctx->in_stream = in_buffer;
    ctx->istream_offset = 0;
    if (flags & FLG_FSM_MINIMAL) {
        FATAL_TODO("OPTIMIZE STATEMACHINE MEMORY TO MINIMAL");
    }
    if(flags & FLG_USE_DEFAULTS){
        TODO("CHOSE TO USE DEFAULT FSM IF NOT DISABLED OR DEFAULT SWITCH LIKE FUNC");
        if(flags & FLG_FSM_DISABLED){
            FATAL_TODO("DEFAULT SWITCH LIKE FUNC!");
        }else {
            TODO("DEFAULT FSM !");
        }
    }
    if(flags & FLG_FSM_DISABLED){
        FATAL_TODO("DISABLE STATMACHINES !");
    }
    TODO("complete ctx init with full ctx in the future");
    TODO("change input buffer stream type to represent an abstract byte stream");
}



#endif
