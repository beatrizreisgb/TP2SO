#include <stdio.h>
#include <stdlib.h>

int get_address(int page_size);

void hier2_fifo(int num_pages, int page_size, FILE* file, char* debug_flag, FILE* outfile);

void hier2_lru(int num_pages, int page_size, FILE* file, char* debug_flag, FILE* outfile);

void hier2_random(int num_pages, int page_size, FILE* file, char* debug_flag, FILE* outfile);

void hier2_2a(int num_pages, int page_size, FILE* file, char* debug_flag, FILE* outfile);
