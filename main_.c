#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "include/inverted.h"
#include "include/mem_address.h"

#define INF 1000000000

FILE *file;
int page_size;

// void dense_fifo(struct mem_address* dense_table, int num_pages, int dense_size){
//     while(fscanf(file, "%x %c", &addr, &rw) == 2){
//         struct mem_address pg;
//         pg.addr = get_address();
//         pg.rw = rw;
//         int page = pg.addr;
//         char rw = pg.rw;
//         int min_time = INF;
//         int fifo_first = 0;

//         if (dense_table[page].addr == 1) {
//             hit++;
//             global_time++;
//             continue;
//         }
        
//         miss++;

//         if (pages_count < num_pages){
//             dense_table[page].addr = 1;
//             dense_table[page].rw = rw;
//             dense_table[page].time = global_time++;
//             pages_count++;
//             continue;
//         }

//         for (int i = 0; i < dense_size; i++) {
//             if (dense_table[i].time < min_time) {
//                 min_time = dense_table[i].time;
//                 fifo_first = i;
//             }
//         }

//         if (dense_table[fifo_first].rw == 'W'){
//             written++;
//         }

//         dense_table[fifo_first].addr = -1;
//         dense_table[fifo_first].time = INF;
//         dense_table[fifo_first].rw = ' ';
//         dense_table[page].addr = 1;
//         dense_table[page].time = global_time++;
//         dense_table[page].rw = rw;

//         continue;
//     }
// }

// void dense_lru(struct mem_address* dense_table, int num_pages, int dense_size){
//     while(fscanf(file, "%x %c", &addr, &rw) == 2){
//         struct mem_address pg;
//         pg.addr = get_address();
//         pg.rw = rw;
//         int page = pg.addr;
//         char rw = pg.rw;
//         int min_time = INF;
//         int lru = 0;

//         if (dense_table[page].addr == 1) {
//             dense_table[page].time = global_time++;
//             hit++;
//             continue;
//         }
        
//         miss++;

//         if (pages_count < num_pages){
//             dense_table[page].addr = 1;
//             dense_table[page].rw = rw;
//             dense_table[page].time = global_time++;
//             pages_count++;
//             continue;
//         }

//         for (int i = 0; i < dense_size; i++) {
//             if (dense_table[i].time < min_time) {
//                 min_time = dense_table[i].time;
//                 lru = i;
//             }
//         }

//         if (dense_table[lru].rw == 'W'){
//             written++;
//         }
//         dense_table[lru].addr = -1;
//         dense_table[lru].time = INF;
//         dense_table[lru].rw = ' ';
//         dense_table[page].addr = 1;
//         dense_table[page].time = global_time++;
//         dense_table[page].rw = rw;

//         continue;
//     }
// }

// void dense_random(struct mem_address* dense_table, int num_pages, int dense_size){
//     while(fscanf(file, "%x %c", &addr, &rw) == 2){
//         struct mem_address pg;
//         pg.addr = get_address();
//         pg.rw = rw;
//         int page = pg.addr;
//         char rw = pg.rw;
//         int rand_idx;

//         if (dense_table[page].addr == 1) {
//             hit++;
//             continue;
//         }
        
//         miss++;

//         if (pages_count < num_pages){
//             dense_table[page].addr = 1;
//             dense_table[page].rw = rw;
//             pages_count++;
//             continue;
//         }

//         do{
//             rand_idx = rand() % dense_size;
//         }while(dense_table[rand_idx].addr != 1);
        
//         if (dense_table[rand_idx].rw == 'W'){
//             written++;
//         }
//         printf("rand idx %d \n", rand_idx);

//         dense_table[rand_idx].addr = -1;
//         dense_table[rand_idx].rw = ' ';
//         dense_table[page].addr = 1;
//         dense_table[page].rw = rw;

//         continue;
//     }
// }

int main (int argc, char* argv[]){
    
    srand(time(NULL));

    char* mode = argv[1];
    char* file_name = argv[2];
    page_size = atoi(argv[3])*1024;
    int mem_size = atoi(argv[4])*1024;
    char* table_type = argv[5];
    char* debug_flag = argv[6];

    int num_pages = mem_size / page_size;

    file = fopen(file_name, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    int dense_size = mem_size*1024;

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

    // case 'd':
    //     // table = (struct mem_address*) malloc(dense_size * sizeof(struct mem_address)); 
    //     // for (int i = 0; i < dense_size; i++){
    //     //     table[i].addr = -1;
    //     //     table[i].time = INF;
    //     // }

    //     switch (mode[0]){
    //     case 'l':
    //         dense_lru(table, num_pages, dense_size);
    //         break;
    //     case 'f':
    //         dense_fifo(table, num_pages, dense_size);
    //         break;
    //     case 'r':
    //         dense_random(table, num_pages, dense_size);
    //         break;
    //     case '2':
    //         //dense_2a(table, num_pages, dense_size, second_chance);
    //         break;
    //     }
    //     break;
    } 

    fclose(file);

    return (0);
}