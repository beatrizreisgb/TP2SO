#include <stdio.h>
#include <stdlib.h>

int fifo_first;
int hit;
int miss;
int global_time;
int written;
int second_chance_idx;
int pages_count;
unsigned addr;
char rw;

struct mem_address{
    unsigned addr;
    unsigned time;
    char rw;
};

int power(int base, int exp);

unsigned find_s(int page_size);

void print_result();