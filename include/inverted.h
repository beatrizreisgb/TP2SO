#include <stdlib.h>
#include <stdio.h>

int get_address(int page_size);

void inverted_fifo(int num_pages, int page_size, FILE* file);

void inverted_lru(int num_pages, int page_size, FILE* file);

void inverted_random(int num_pages, int page_size, FILE* file);

void inverted_2a(int num_pages, int page_size, FILE* file);