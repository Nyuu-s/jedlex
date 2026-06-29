

#include <stdio.h>
#include "jedlex.h"

// CP_USER_CAT expects value between 0;63
#define MY_CUSTOM_CAT CP_USER_CAT(5)

int main(){
  printf("hello world!\n");
  FILE* fd = fopen("./README.md", "r");

  uint8 buff[4096];
  fread(buff, sizeof(buff[0]), sizeof(buff), fd);


  JLexer lexer = {0};
  lexer.buff = buff;

  JCodepoint cp = {0};
  while(lexer.status != JED_EOF){
    jlex_get_char(&lexer, &cp);
  }

  printf("End of prog reached!\n");

  return 0;

}

 
