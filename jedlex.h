#ifndef JEDLEX_H
#define JEDLEX_H
//* temp
#define UNUSED(value) (void)(value)
#define TODO(message) do { fprintf(stderr, "%s:%d: TODO: %s\n", __FILE__, __LINE__, message); exit(1); } while(0)
//***************** */
#define JEDLEX_REGISTER_VALUES(type, ...)\
__jedlex_register_values(type, ((const char*[]){__VA_ARGS__}), (sizeof((const char*[]){__VA_ARGS__})/sizeof(const char*)))

// CLASSIFIER API
#define JEDLEX_CLS_REGISTER(ds) TODO("Register user datastructure as classifier")

#define JEDLEX_CLS_INSERT(type, string, length) TODO("Implement default insert classifier")
#define JEDLEX_CLS_CLASSIFY(type, string, length) TODO("Implement default lookup classifier")
#define JEDLEX_CLS_FREE  (type, string, length) TODO("Implement default free classifier")

#define MAX_STATES 1024

//*********** 

// DFA
typedef unsigned int JedStateID;
typedef struct {
    char key;
    // int priority; ?
    // restrictions; ?
    JedStateID target;
} JedTransition;

typedef struct {
    unsigned int count;
    JedStateID fallback;
    const char* name;
} JedState;

JedState states[1024];













// lexer / tokens structs
typedef int JedTokenType;

typedef enum JedDefaultTokenTypes{
    TTYPE_EOF,
    TTYPE_PUNCTUATION,
    TTYPE_SYMBOL,
    TTYPE_IDENTIFIER,
    TTYPE_KEYWORD,
    TTYPE_COUNT
} JedDefaultTokenTypes;


typedef struct JedToken{
    char* value;
    void* type;
} JedToken;

typedef struct Jedlexer {
    // Lexer -- zone
    char* buffer;           //let user manage IO, and buffer alloc
    unsigned long buffer_index;
    void* token_classifier; //Identifier map pointer, void* to let user completely overide default
    JedToken* current;
    JedToken* next; 

    // -- DFA Zone
} Jedlexer ;
//*********** */

Jedlexer jedlex_init(const char* in_buffer);

void jedlex_read_next_token();
void jedlex_peek_next_token();
void __jedlex_getchar();


// #ifdef JEDLEX_IMPLEMENTATION
void __jedlex_register_values(JedTokenType type, const char* values[], unsigned long amount){
    for (int i =0; i<amount; i++) {
        printf("%s\n", values[i]);
    }
}
Jedlexer jedlex_init(const char* in_buffer){
    TODO("Implement function: jedlex_init");
}
void jedlex_read_next_token(){
    TODO("Implement function: jedlex_read_next_token");
};
void jedlex_peek_next_token(){
    TODO("Implement function: jedlex_peek_next_token");
};
void __jedlex_getchar(){
    TODO("Implement function: __jedlex_getchar");
};
// #endif
#endif
