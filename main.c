#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "include/mem_address.h"
#include "include/inverted.h"
#include "include/dense.h"
#include "include/hier2.h"
#include "include/hier3.h"


FILE *file;
int page_size;

int main (int argc, char* argv[]){
    
    srand(time(NULL));

    char* mode = argv[1];
    char* file_name = argv[2];
    page_size = atoi(argv[3])*1024;
    int mem_size = atoi(argv[4])*1024;
    char* table_type = argv[5];
    char* debug_flag = argv[6];

    int num_pages = mem_size / page_size;
    
    int dense_size = power(2, (32-find_s(page_size)));

    file = fopen(file_name, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    switch (table_type[0]){
    case 'i':
        switch (mode[0]){
        case 'l':
            inverted_lru(num_pages, page_size, file);
            break;
        case 'f':
            inverted_fifo(num_pages, page_size, file);
            break;
        case 'r':
            inverted_random(num_pages, page_size, file);
            break;
        case '2':
            inverted_2a(num_pages, page_size, file);
            break;
        }
        break;

    case 'd':
        switch (mode[0]){
        case 'l':
            dense_lru(num_pages, page_size, file, dense_size);
            break;
        case 'f':   
            dense_fifo(num_pages, page_size, file, dense_size);
            break;
        case 'r':
            dense_random(num_pages, page_size, file, dense_size);
            break;
        case '2':
            dense_2a(num_pages, page_size, file, dense_size);
            break;
        }
        break;

    case '2':
        switch (mode[0]){
        case 'l':
            hier2_lru(num_pages, page_size, file);
            break;
        case 'f':   
            hier2_fifo(num_pages, page_size, file);
            break;
        case 'r':
            hier2_random(num_pages, page_size, file);
            break;
        case '2':
            hier2_2a(num_pages, page_size, file);
            break;
        }
        break;

    case '3':
        switch (mode[0]){
        case 'l':
            // hier3_lru(num_pages, page_size, file);
            break;
        case 'f':   
            hier3_fifo(num_pages, page_size, file);
            break;
        case 'r':
            // hier3_random(num_pages, page_size, file);
            break;
        case '2':
            // hier3_2a(num_pages, page_size, file);
            break;
        }
        break;
    } 

    fclose(file);

    return (0);
}