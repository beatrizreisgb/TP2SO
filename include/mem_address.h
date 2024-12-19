#ifndef MEM_ADDRESS_H
#define MEM_ADDRESS_H

#include <stdio.h>
#include <stdlib.h>

extern int fifo_first;
extern int hit;
extern int miss;
extern int global_time;
extern int written;
extern int second_chance_idx;
extern int pages_count;
extern unsigned int addr;
extern char rw;

struct mem_address{
    unsigned addr;
    unsigned first;
    unsigned second;
    unsigned third;
    unsigned time;
    unsigned dirty;
    char rw;
};

struct page_time{
    unsigned page;
    unsigned first;
    unsigned second;
    unsigned third;
    unsigned time;
};

int power(int base, int exp);

unsigned find_s(int page_size);

void print_result();

unsigned find_row(unsigned num, int shift);

unsigned find_column(unsigned num, int shift);

#endif // MEM_ADDRESS_H