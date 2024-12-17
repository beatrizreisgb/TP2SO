#include <stdio.h>
#include <stdlib.h>

int get_address(int page_size);

void dense_fifo(int num_pages, int page_size, FILE* file, int dense_size);

void dense_lru(int num_pages, int page_size, FILE* file, int dense_size);

void dense_random(int num_pages, int page_size, FILE* file, int dense_size);
