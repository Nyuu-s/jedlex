#ifndef JEDLEX_H
#define JEDLEX_H
//* temp
#define UNUSED(value) (void)(value)
#define TODO(message) do { fprintf(stderr, "%s:%d: TODO: %s\n", __FILE__, __LINE__, message); exit(1); } while(0)
//***************** */
#define JEDLEX_REGISTER(type, ...)\
__jedlex_register(type, ((const char*[]){__VA_ARGS__}), (sizeof((const char*[]){__VA_ARGS__})/sizeof(const char*)))

//*********** 
// TODO make private with opaque/void pointers and such
typedef struct JedToken{
    char* value;
    void* type;
} JedToken;

typedef struct Jedlexer {
    char* buffer;           //let user manage IO, and buffer alloc
    unsigned long buffer_ptr;
    JedToken* current;
    JedToken* next; 
} Jedlexer ;
//*********** */

Jedlexer jedlex_init(const char* in_buffer);
void __jedlex_register(void* type, const char* values[], unsigned long amount);

void jedlex_read_next_token();
void jedlex_peek_next_token();
void __jedlex_getchar();


#ifdef JEDLEX_IMPLEMENTATION
void __jedlex_register(void* type, const char* values[], unsigned long amount){
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
#endif
#endif
