//TODO: replace size_t with owned type
//TODO: define uint8
#ifndef JEDLEX_H
#define JEDLEX_H
#include <stddef.h>


#if defined(LEX_ENCODING_UTF8)
    #define LEX_DECODE(p, out_cp) utf8_decode(p, out_cp)
#elif defined(LEX_ENCODING_UTF16_LE)
    #define LEX_DECODE(p, out_cp) utf16le_decode(p, out_cp)
#elif defined(LEX_ENCODING_UTF16_BE)
    #define LEX_DECODE(p, out_cp) utf16be_decode(p, out_cp)
#elif defined(LEX_ENCODING_UTF32_LE)
    #define LEX_DECODE(p, out_cp) utf32le_decode(p, out_cp)
#elif defined(LEX_ENCODING_UTF32_BE)
    #define LEX_DECODE(p, out_cp) utf32be_decode(p, out_cp)
#elif defined(LEX_ENCODING_EBCDIC_BE)
    #define LEX_DECODE(p, out_cp) ebcdicbe_decode(p, out_cp)
#elif defined(LEX_ENCODING_EBCDIC_LE)
    #define LEX_DECODE(p, out_cp) ebcdicle_decode(p, out_cp)
#else
    #define LEX_DECODE(p, out_cp) utf8_decode(p, out_cp)
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

Elex_status utf8_decode(JLexer* lex, JCodePoint* cp);

// #### codepoint classifier | merge directly in decoder, might change in the future
// #### sequence classifier (fsm)
int get_cp_len(codepoint_t cp){
  if(cp >= 0xC0 && cp <= 0xDF)      return 2;
  else if(cp >= 0xE0 && cp <= 0xEF) return 3;
  else if(cp >= 0xF0 && cp <= 0xF7) return 4;
  else return -1;
}

Elex_status jlex_get_char(JLexer* lex, JCodePoint* cp){
  LEX_DECODE(lex, cp);
  return lex->status;
}

Elex_status utf8_decode(JLexer* lex, JCodePoint* cp){
  if(lex->status != JED_READY) { lex->status = JED_ERROR; return lex->status;}
  
  uint8* cur = lex->buff + lex->b_offset;

  if(lex->b_offset >= lex->buff_size){
    lex->status = JED_EOF;
    return lex->status;
  }

  if(*cur < 0x80) {
    cp->value = *lex->buff;
    lex->b_offset++;
    lex->c_offset++;
    return lex->status;
  }
  printf("hello there\n");

  int len = get_cp_len(*cur);
  if(len <= 0 ) {
    lex->status = JED_ERROR; 
    return lex->status;
  }

  if(lex->b_offset + len > lex->buff_size){
    lex->status = JED_EOF;
    // might span on multiple buffers
  }
  // 1100 0000 if 1 since 1 bit header 7 payload
  // 1110 0000 if 2 since 3 bit header 5 payload
  // 1111 0000 if 3 since 4 bit header 4 payload
  // 1111 1100 if 4 since 5 bit header 3 payload
  // using len * (len != 1) so shift by 7 when len=1 otherwise shift by 7-(2;4)
  uint8 first_byte_mask = ~(0xFF << (7 - (len * (len != 1))));


  // 0000 0000 | (cur & fb_mask) << (6 * (1;4) 
  // zero out the header bits in current byte (& operation) 
  // then move them to the left enough so there is space for len-1 payload
  // then read len-1 bytes and remove their header and slide them left in the result in their corresponding slot
  cp->value |= ((*cur & first_byte_mask) << (6 * (len-1)));
  for(int i = 1; i < len; i++){
    cp->value |= (( *(cur+i) & 0x3F) << (6 * (len-i-1)));
  }

  //move byte offset and codepoint offset;
  lex->b_offset += len;
  lex->c_offset++;
  return lex->status;

}
void get_next_token();
void get_state_from_cp(JCodePoint cp);


void init_lexer();
void reset_lexer();

// output token



#endif
