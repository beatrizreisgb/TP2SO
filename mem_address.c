#include "include/mem_address.h"

int fifo_first = 0;
int hit = 0;
int miss = 0;
int global_time = 0;
int written = 0;
int second_chance_idx = 0;
int pages_count = 0;
unsigned addr;
char rw;

int get_address(int page_size){
    unsigned s, tmp;
    // Derivar o valor de s: 
    tmp = page_size;
    s = 0;
    while (tmp>1) {
        tmp = tmp>>1;
        s++;
    }

    int p = addr >> s; //end da pagina
    return p;
}

void print_result(){
    printf("Hits: %d\n", hit);
    printf("Misses/Page faults: %d\n", miss);
    printf("Writes: %d\n", written);
}