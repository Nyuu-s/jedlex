
#include <stdlib.h>
#include <stdio.h>
#define JEDLEX_IMPLEMENTATION
#include "jedlex.h"
int main(){
    jedlex_init(NULL);
    char* arr[5];
    JEDLEX_REGISTER_VALUES(TTYPE_KEYWORD, "if", "else", "then");
    JEDLEX_REGISTER_VALUES(TTYPE_KEYWORD, "top");
    JEDLEX_REGISTER_VALUES(TTYPE_IDENTIFIER, "hello", "world");
    // JEDLEX_CLS_REGISTER(&arr);

    // add_rule(Matcher, Event, Restriction, priority)

    return 0;
}