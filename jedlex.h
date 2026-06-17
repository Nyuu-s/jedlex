//TODO: replace size_t with owned type
//TODO: define uint8
#ifndef JEDLEX_H
#define JEDLEX_H
#include <stddef.h>

#if defined(LEX_ENCODING_UTF8)
    #define LEX_DECODE(p, out_cp, out_len) utf8_decode(p, out_cp, out_len)
#elif defined(LEX_ENCODING_UTF16_LE)
    #define LEX_DECODE(p, out_cp, out_len) utf16le_decode(p, out_cp, out_len)
#elif defined(LEX_ENCODING_UTF16_BE)
    #define LEX_DECODE(p, out_cp, out_len) utf16be_decode(p, out_cp, out_len)
#elif defined(LEX_ENCODING_UTF32_LE)
    #define LEX_DECODE(p, out_cp, out_len) utf32le_decode(p, out_cp, out_len)
#elif defined(LEX_ENCODING_UTF32_BE)
    #define LEX_DECODE(p, out_cp, out_len) utf32be_decode(p, out_cp, out_len)
#elif defined(LEX_ENCODING_EBCDIC_BE)
    #define LEX_DECODE(p, out_cp, out_len) ebcdicbe_decode(p, out_cp, out_len)
#elif defined(LEX_ENCODING_EBCDIC_LE)
    #define LEX_DECODE(p, out_cp, out_len) ebcdicle_decode(p, out_cp, out_len)
#else
    #define LEX_DECODE(p, out_cp, out_len) utf8_decode(p, out_cp, out_len)
#endif

// #### GLOBALS
typedef unsigned char uint8;
typedef unsigned int uint32;

typedef enum {
  JED_READY,
  JED_EOF,
  JED_ERROR,

  JED_STATUS_COUNT
} Elex_status;

typedef enum{
  JED_IDENTIFIER,
  JED_PUNCTUATION,

  /*USER EXTENDABLE*/
  JED_TOKENKIND_COUNT
} Etoken_kind;
typedef struct {
  uint8* buff;
  size_t buff_size;
  size_t b_offset; //offset in bytes
  size_t c_offset; //offset in "characters"/codepoints
  size_t line;

  Elex_status status;
} JLexer;

typedef struct {
  uint8* start; size_t length;
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
  codepoint_t value;
  Ecp_category category;
  // uint8 length; just compute it from cp  
} JCodePoint;


// #### codepoint classifier | merge directly in decoder, might change in the future
// #### sequence classifier (fsm)
int get_cp_len(codepoint_t cp){
  if(cp >= 0xC0 && cp <= 0xDF)      return 2;
  else if(cp >= 0xE0 && cp <= 0xEF) return 3;
  else if(cp >= 0xF0 && cp <= 0xF7) return 4;
  else return -1;
}

int  decode_and_classify_uni(JLexer* lex, JCodePoint* cp){
  if(lex->status != JED_READY) { return -1;}
  
  if(*lex->buff < 0x80) {cp->value = *lex->buff; return 1;}

  uint8* p = lex->buff;
  int len = get_cp_len(*p);
  if(len <= 0 || lex->b_offset + len > lex->buff_size) { lex->status = JED_ERROR; return -1;}

  uint8 first_byte_mask = ~(0xFF << (7 - len));

  cp->value |= ((*p & first_byte_mask) << (6 * (len-1)));
  for(int i = 1; i < len; i++){
    cp->value |= (( *(p+i) & 0x3F) << (6 * (len-i-1)));
  }

}
void get_next_token();
void get_state_from_cp(JCodePoint cp);


void init_lexer();
void reset_lexer();

// output token



#endif
