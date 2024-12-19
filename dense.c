#include "include/dense.h"
#include "include/mem_address.h"

#define INF 1000000000

void dense_fifo(int num_pages, int page_size, FILE* file, int dense_size){
    struct mem_address* table;
    table = (struct mem_address*) malloc(dense_size * sizeof(struct mem_address));
       
    for (int i = 0; i < dense_size; i++){
        table[i].addr = -1;
        table[i].time = INF;
    }

    int* time_table;
    time_table = (int*) malloc(num_pages * sizeof(int));

    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        int s = find_s(page_size);
        pg.addr = addr >> s;
        pg.rw = rw;
        int page = pg.addr;
        char rw = pg.rw;
        int found = 0;
        int min_time = INF;
        int fifo_first = 0;


        if (table[page].addr == 1) {
            hit++;
            global_time++;
            found++;
        }

        if (found) continue;
        
        miss++;

        if (pages_count < num_pages){
            table[page].addr = 1;
            table[page].rw = rw;
            time_table[pages_count] = global_time++;
            pages_count++;
            found++;
        }

        if(found) continue;


        // problema de desempenho
        for (int i = 0; i < num_pages; i++) {
            if (time_table[i] < min_time) {
                min_time = time_table[i];
                fifo_first = i;
            }
        }

        if (table[fifo_first].rw == 'W'){
            written++;
        }

        table[fifo_first].addr = -1;
        table[fifo_first].rw = ' ';
        table[page].addr = 1;
        table[page].rw = rw;

        time_table[fifo_first] = global_time++;

        continue;
    }
    
    print_result();
}

void dense_lru(int num_pages, int page_size, FILE* file, int dense_size){
    struct mem_address* table;
    table = (struct mem_address*) malloc(dense_size * sizeof(struct mem_address));
       
    for (int i = 0; i < dense_size; i++){
        table[i].addr = -1;
        table[i].time = INF;
    }
    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        int s = find_s(page_size);
        pg.addr = addr >> s;
        pg.rw = rw;
        int page = pg.addr;
        char rw = pg.rw;
        int min_time = INF;
        int lru = 0;

        if (table[page].addr == 1) {
            table[page].time = global_time++;
            hit++;
            continue;
        }
        
        miss++;

        if (pages_count < num_pages){
            table[page].addr = 1;
            table[page].rw = rw;
            table[page].time = global_time++;
            pages_count++;
            continue;
        }

        //for (int i = 0; i < dense_size; i++) {
        //   if (table[i].time < min_time) {
        //        min_time = table[i].time;
        //        lru = i;
        //   }
        //}

        if (table[lru].rw == 'W'){
            written++;
        }
        table[lru].addr = -1;
        table[lru].time = INF;
        table[lru].rw = ' ';
        table[page].addr = 1;
        table[page].time = global_time++;
        table[page].rw = rw;

        continue;
    }
    print_result();
}

void dense_random(int num_pages, int page_size, FILE* file, int dense_size){
    struct mem_address* table;
    table = (struct mem_address*) malloc(dense_size * sizeof(struct mem_address));
       
    for (int i = 0; i < dense_size; i++){
        table[i].addr = -1;
        table[i].time = INF;
    }
    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        int s = find_s(page_size);
        pg.addr = addr >> s;
        pg.rw = rw;
        int page = pg.addr;
        char rw = pg.rw;
        int rand_idx;

        if (table[page].addr == 1) {
            hit++;
            continue;
        }
        
        miss++;

        if (pages_count < num_pages){
            table[page].addr = 1;
            table[page].rw = rw;
            pages_count++;
            continue;
        }

        //do{
            rand_idx = rand() % dense_size;
        //}while(table[rand_idx].addr != 1);
        
        if (table[rand_idx].rw == 'W'){
            written++;
        }


        table[rand_idx].addr = -1;
        table[rand_idx].rw = ' ';
        table[page].addr = 1;
        table[page].rw = rw;

        continue;
    }
    print_result();
}