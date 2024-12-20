#include "../include/mem_address.h"

int fifo_first = 0;
int hit = 0;
int miss = 0;
int global_time = 0;
int written = 0;
int second_chance_idx = 0;
int pages_count = 0;
unsigned addr;
char rw;

struct mem_address* create_table(int size){    
    struct mem_address* table;
    table = (struct mem_address*) malloc(size * sizeof(struct mem_address));

    return table;
}

struct page_time* create_time_table(int size){
    struct page_time* time_table;
    time_table = (struct page_time*) malloc(size * sizeof(struct page_time));

    return time_table;
}

struct mem_address** create_2aTable(int rows, int columns){
    struct mem_address** table = (struct mem_address**)malloc(rows * sizeof(struct mem_address*));

    for (int i = 0; i < rows; i++) {
       table[i] = (struct mem_address*)malloc(columns * sizeof(struct mem_address));
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            table[i][j].addr = -1;
        }
    }

    return table;
}

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
    printf("Páginas lidas: %d\n", miss);
    printf("Páginas escritas: %d\n", written);
}

unsigned find_row(unsigned page, int shift){
    return page >> shift;
}

unsigned find_column(unsigned num, int shift) {
    unsigned mask = (1U << shift) - 1; 
    return num & mask;  
}