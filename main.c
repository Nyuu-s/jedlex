
#include <stdlib.h>
#include <stdio.h>
#define JEDLEX_IMPLEMENTATION
#include "jedlex.h"
int main(){
    jedlex_init(NULL);
    JEDLEX_REGISTER(0, "if", "else", "then");
    return 0;
}