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

    struct page_time* time_table;
    time_table = (struct page_time*) malloc(num_pages * sizeof(struct page_time));

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
        int time_idx = 0;


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
            time_table[pages_count].time = global_time++;
            time_table[pages_count].page = page;
            pages_count++;
            found++;
        }

        if(found) continue;


        for (int i = 0; i < num_pages; i++) {
            if (time_table[i].time < min_time) {
                min_time = time_table[i].time;
                fifo_first = time_table[i].page;
                time_idx = i;
            }
        }

        if (table[fifo_first].rw == 'W'){
            written++;
        }

        table[fifo_first].addr = -1;
        table[fifo_first].rw = ' ';

        table[page].addr = 1;
        table[page].rw = rw;

        time_table[time_idx].time = global_time++;
        time_table[time_idx].page = page;

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

    struct page_time* time_table;
    time_table = (struct page_time*) malloc(num_pages * sizeof(struct page_time));
   
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
        int time_idx = 0;


        if (table[page].addr == 1) {
            hit++;
            for (int i = 0; i < num_pages; i++) {
                if (time_table[i].page == page){
                    time_table[i].time = global_time++;
                    break;
                }
            }
            found++;
        }

        if (found) continue;
        
        miss++;

        if (pages_count < num_pages){
            table[page].addr = 1;
            table[page].rw = rw;
            time_table[pages_count].time = global_time++;
            time_table[pages_count].page = page;
            pages_count++;
            found++;
        }

        if(found) continue;


        for (int i = 0; i < num_pages; i++) {
            if (time_table[i].time < min_time) {
                min_time = time_table[i].time;
                fifo_first = time_table[i].page;
                time_idx = i;
            }
        }

        if (table[fifo_first].rw == 'W'){
            written++;
        }

        table[fifo_first].addr = -1;
        table[fifo_first].rw = ' ';

        table[page].addr = 1;
        table[page].rw = rw;

        time_table[time_idx].time = global_time++;
        time_table[time_idx].page = page;

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
    
    struct page_time* time_table;
    time_table = (struct page_time*) malloc(num_pages * sizeof(struct page_time));

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
            time_table[pages_count].time = global_time++;
            time_table[pages_count].page = page;
            pages_count++;
            continue;
        }

    
        rand_idx = rand() % num_pages;

        int choice = time_table[rand_idx].page;
        
        if (table[choice].rw == 'W'){
            written++;
        }

        table[choice].addr = -1;
        table[choice].rw = ' ';

        table[page].addr = 1;
        table[page].rw = rw;

        time_table[rand_idx].time = global_time++;
        time_table[rand_idx].page = page;

        continue;
    }
    print_result();
}