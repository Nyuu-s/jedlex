
#include "jedlex.h"
#include "stdlib.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#define MAX_TRANSIT 128
typedef unsigned int u32;

typedef struct Transit {
    u32 from_state;
    u32 to_state;
    char event;    
} Transit;

typedef struct State{
    u32 ID;
    u32 transit_count;
    char* name;
    Transit* transits;
} State;


void debug_transits(Transit* ts, u32 count){
    printf("Transits {\n");
    for (u32 i=0; i<count; i++) {
        printf("\t\tfrom_state:%d\n", ts[i].from_state);
        printf("\t\tto_state:%d\n", ts[i].to_state);
        printf("\t\tevent:%c\n", ts[i].event);
        printf("\n");
    }
    printf("\t}\n");
}
void debug_state(State* s){
    printf("State pointer: %p\n", s);
    printf("State {\n");
    printf("\tname:%s\n", s->name);
    printf("\tID:%d\n", s->ID);
    printf("\ttransit_count:%d\n", s->transit_count);
    printf("\tTransits pointer: %p\n", s->transits);
    printf("\t");
    debug_transits(s->transits, s->transit_count);
    printf("}\n");
}


State* make_state(){
    State* s = (State*) malloc(sizeof(State));
    Transit* ts = (Transit*) malloc(sizeof(Transit)* MAX_TRANSIT);
    memset(ts, 0, sizeof(Transit)* MAX_TRANSIT);
    
    s->transit_count = 0;
    s->transits = ts;
    s->ID = rand();
    return s;
}

void add_transit(State* s, char on_event, u32 to_state){
    if(s->transit_count < MAX_TRANSIT){
        s->transits[s->transit_count].event = on_event;
        s->transits[s->transit_count].to_state = to_state;
        s->transits[s->transit_count++].from_state = s->ID;
    }else{
        printf("ERROR: State %s cannot have more transitions !", s->name);
    }
}

void add_transit_range(State* s, char lower, char higher, u32 to_state){
    if(lower < higher){
        for (u32 i = lower; i<=higher; i++) {
            add_transit(s, (char) i, to_state);
        }
    } else {
        printf("ERROR: lower bounds is bigger than higer bounds");
    }    
}



int main(){
    uint8* input = "aaabbbccc"; // mimic a fdopen + fread into char buffer
    JedlexCtx ctx = {0}; // user handle all memory allocs
    
    jedlex_init(&ctx, input, COREMODE_SWITCH);
jedlex_get_next_token(&ctx);
    // State *s1 = make_state();
    // State *s2 = make_state();
    // s1->name = "Start";
    // s2->name = "End";

    // add_transit_range(s1, 'a', 'z', s2->ID);

    // debug_state(s1);
    // debug_state(s2);
    return 0;
}
