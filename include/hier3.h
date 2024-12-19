#include <stdio.h>
#include <stdlib.h>

int get_address(int page_size);

void hier3_fifo(int num_pages, int page_size, FILE* file);

void hier3_lru(int num_pages, int page_size, FILE* file);

void hier3_random(int num_pages, int page_size, FILE* file);

void hier3_2a(int num_pages, int page_size, FILE* file);
