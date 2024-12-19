#include "../include/inverted.h"
#include "../include/mem_address.h"

void inverted_fifo(int num_pages, int page_size, FILE* file){
    struct mem_address* table;
    table = (struct mem_address*) malloc(num_pages * sizeof(struct mem_address));
    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        int s = find_s(page_size);
        pg.addr = addr >> s;
        pg.dirty = 0;
        int page = pg.addr;
        int found = 0;

        for (int i = 0; i < num_pages; i++) {
            if (table[i].addr == page) {
                found = 1;
                hit++;
                if (rw == 'W'){
                    table[i].dirty = 1;
                }
                
                break;
            }
            if(table[i].addr == -1){
                table[i].addr = page;
                if (rw == 'W'){
                    table[i].dirty = 1;
                }
                found = 1;
                miss++;
                break;
            }
        }
        if (found) continue;
        miss++;

        table[fifo_first].addr = page;

        if (rw == 'W'){
            table[fifo_first].dirty = 1;
        }

        if (table[fifo_first].dirty == 1){
            written++;
        }

        fifo_first = (fifo_first + 1) % num_pages;
    
        continue;
    }
    print_result();
}

void inverted_lru(int num_pages, int page_size, FILE* file){
    struct mem_address* table;
    table = (struct mem_address*) malloc(num_pages * sizeof(struct mem_address));
    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        int s = find_s(page_size);
        pg.addr = addr >> s;
        pg.rw = rw;
        int page = pg.addr;
        char rw = pg.rw;
        int found = 0;

        for (int i = 0; i < num_pages; i++) {
            if (table[i].addr == page) {
                hit++;
                table[i].time = global_time;
                global_time++;
                found++;
                break;
            }
            if(table[i].addr == -1){
                table[i].addr = page;
                table[i].time = global_time;
                table[i].rw = rw;
                miss++;
                global_time++;
                found++;
                break;
            }
        }
        if (found) continue;
        miss++;
        int lru = 0; // least recently used position

        for (int i = 0; i < num_pages; i++) {
            if (table[i].time < table[lru].time) {
                lru = i;
            }
        }
        
        if (table[lru].rw == 'W'){
            written++;
        }

        table[lru].addr = page;
        table[lru].rw = rw;
        table[lru].time = global_time;

        global_time++;

        continue;
    }
    print_result();
}

void inverted_random(int num_pages, int page_size, FILE* file){
    struct mem_address* table;
    table = (struct mem_address*) malloc(num_pages * sizeof(struct mem_address));
    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        int s = find_s(page_size);
        pg.addr = addr >> s;
        pg.rw = rw;
        int page = pg.addr;
        char rw = pg.rw;
        int found = 0;

        for (int i = 0; i < num_pages; i++) {
            if (table[i].addr == page) {
                hit++;
                found++;
                break;
            }
            if(table[i].addr == -1){
                table[i].addr = page;
                table[i].rw = rw;
                miss++;
                found++;
                break;
            }
        }
        if (found) continue;
        miss++;
        int rand_idx = rand() % num_pages;
        if (table[rand_idx].rw == 'W'){
            written++;
        }
        table[rand_idx].addr = page;
        table[rand_idx].rw = rw;
        continue;
    }
    print_result();
}

void inverted_2a(int num_pages, int page_size, FILE* file){
    struct mem_address* table;
    table = (struct mem_address*) malloc(num_pages * sizeof(struct mem_address));
    int* second_chance;
    second_chance = (int*) malloc(num_pages * sizeof(int));
    for (int i = 0; i < num_pages; i++){
        table[i].addr = -1;
        table[i].time = 0;
    }
    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        int s = find_s(page_size);
        pg.addr = addr >> s;
        pg.rw = rw;
        int page = pg.addr;
        char rw = pg.rw;
        int found = 0;

        for (int i = 0; i < num_pages; i++) {
            if (table[i].addr == page) {
                hit++;
                second_chance[i] = 1;
                found++;
                break;
            }
            if(table[i].addr == -1){
                table[i].addr = page;
                table[i].rw = rw;
                miss++;
                found++;
                break;
            }
        }
        if (found) continue;
        miss++;
        int choice = -1;

        for (int i = 0; i < num_pages; i++) {
            int idx = (second_chance_idx + i) % num_pages;
            if (second_chance[idx] == 1) {
                second_chance[idx] = 0;
                continue;
            }
            else{
                choice = idx;
                break;
            }
        }

        if (choice == -1) choice = second_chance_idx;
        second_chance_idx = (choice + 1) % num_pages;

        if (table[choice].rw == 'W'){
            written++;
        }

        table[choice].addr = page;
        table[choice].rw = rw;
        second_chance[choice] = 0;
        continue;
    }
    print_result();
}