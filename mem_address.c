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

int power(int base, int exp) {
    int resultado = 1;
    for (int i = 0; i < exp; i++) {
        resultado *= base;
    }
    return resultado;
}

unsigned find_s(int page_size){
    unsigned s, tmp;
    // Derivar o valor de s: 
    tmp = page_size;
    s = 0;
    while (tmp>1) {
        tmp = tmp>>1;
        s++;
    }
    return s;
}

void print_result(){
    printf("Hits: %d\n", hit);
    printf("Misses/Page faults: %d\n", miss);
    printf("Writes: %d\n", written);
}

unsigned find_row(unsigned page, int shift){
    return page >> shift;
}

unsigned find_column(unsigned num, int shift) {
    unsigned mask = (1U << shift) - 1; 
    return num & mask;  
}