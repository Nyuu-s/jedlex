//TODO: replace size_t with owned type
//TODO: define uint8
#ifndef JEDLEX_H
#define JEDLEX_H

// #### GLOBALS

typedef enum {
  JED_READY,
  JED_EOF,

  JED_STATUS_COUNT
} Elex_status;

typedef enum{
  JED_IDENTIFIER,
  JED_PUNCTUATION

  /*USER EXTENDABLE*/
  JED_TOKENKIND_COUNT
} Etoken_kind;

typedef struct {
  uint8* buff;
  size_t end;
  size_t b_offset; //offset in bytes
  size_t c_offset; //offset in "characters"/codepoints
  size_t line;
} JLexer;

typedef struct {
  uint8* start;
  size_t length;
  size_t col;
  size_t line;
  Etoken_kind kind;
} JToken;

typedef uint32 codepoint_t;

// #### UNICODE DECODING
typedef enum {  
  JED_CPCAT_LETTER,
  JED_CPCAT_NUMBER,
  JED_CPCAT_EMOTE,

  /*USER EXTENDABLE*/
  JED_CPCAT_COUNT
} Ecp_category;

typedef struct {
  codepoint_t cp;
  Ecp_category category;
  // uint8 length; just compute it from cp  
} JCodePoint;

JCodePoint decode_and_classify_uni(JLexer* lex);

// #### codepoint classifier | merge directly in decoder, might change in the future
// #### sequence classifier (fsm)
void get_next_token();
void get_state_from_cp(JcodePoint cp);


void init_lexer();
void reset_lexer();

// output token



#endif
