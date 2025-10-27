
#include <stdlib.h>
#include <stdio.h>
#define JEDLEX_IMPLEMENTATION
#include "jedlex.h"
int main(){
    jedlex_init(NULL);
    JEDLEX_REGISTER(TTYPE_KEYWORD, "if", "else", "then");
    return 0;
}