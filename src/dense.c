#include "../include/dense.h"
#include "../include/mem_address.h"

#define INF 1000000000

void dense_fifo(int num_pages, int page_size, FILE* file, int dense_size, char* debug_flag, FILE* outfile){

   struct mem_address* table = create_table(dense_size);
       
    for (int i = 0; i < dense_size; i++){
        table[i].addr = -1;
        table[i].time = INF;
    }

    struct page_time* time_table = create_time_table(num_pages);

    int choice = 0;

    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        if (debug_flag[0] == 'd') fprintf(outfile, "Endereço: %x ", addr);
        int s = find_s(page_size);
        pg.addr = addr >> s;
        pg.rw = rw;
        int page = pg.addr;
        char rw = pg.rw;
        int min_time = INF;
        int fifo_first = 0;
        int time_idx = 0;


        if (table[page].addr == 1) {
            hit++;
            if (debug_flag[0] == 'd') fprintf(outfile, "Hit\n");
            global_time++;
            if (rw == 'W'){
                table[page].dirty = 1;
            }
            continue;
        }

       
        miss++;
        if (debug_flag[0] == 'd') fprintf(outfile, "Miss\n");

        if (pages_count < num_pages){
            table[page].addr = 1;
            table[page].rw = rw;
            table[page].dirty = (rw == 'W') ? 1 : 0;
            time_table[pages_count].time = global_time++;
            time_table[pages_count].page = page;
            pages_count++;
            continue;
        }

        for (int i = 0; i < num_pages; i++) {
            if (time_table[i].time < min_time) {
                min_time = time_table[i].time;
                choice = time_table[i].page;
                time_idx = i;
            }
        }

        if (table[choice].dirty == 1){
            written++;
        }

        table[choice].addr = -1;
        table[choice].rw = ' ';

        table[page].addr = 1;
        table[page].rw = rw;
        table[page].dirty = (rw == 'W') ? 1 : 0;

        time_table[time_idx].time = global_time++;
        time_table[time_idx].page = page;

        continue;
    }
    print_result();
    free_table(table);
    free_time_table(time_table);
}

void dense_lru(int num_pages, int page_size, FILE* file, int dense_size, char* debug_flag, FILE* outfile){
   struct mem_address* table = create_table(dense_size);
       
    for (int i = 0; i < dense_size; i++){
        table[i].addr = -1;
        table[i].time = INF;
    }

    struct page_time* time_table = create_time_table(num_pages);

    int choice = 0;

    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        if (debug_flag[0] == 'd') fprintf(outfile, "Endereço: %x ", addr);
        int s = find_s(page_size);
        pg.addr = addr >> s;
        pg.rw = rw;
        int page = pg.addr;
        char rw = pg.rw;
        int min_time = INF;
        int fifo_first = 0;
        int time_idx = 0;


        if (table[page].addr == 1) {
            hit++;
            if (debug_flag[0] == 'd') fprintf(outfile, "Hit\n");
            for (int i = 0; i < num_pages; i++) {
                if (time_table[i].page == page){
                    time_table[i].time = global_time++;
                    break;
                }
            }
            if (rw == 'W'){
                table[page].dirty = 1;
            }
            continue;
        }
        
        miss++;
        if (debug_flag[0] == 'd') fprintf(outfile, "Miss\n");

        if (pages_count < num_pages){
            table[page].addr = 1;
            table[page].rw = rw;
            table[page].dirty = (rw == 'W') ? 1 : 0;
            time_table[pages_count].time = global_time++;
            time_table[pages_count].page = page;
            pages_count++;
            continue;
        }

        for (int i = 0; i < num_pages; i++) {
            if (time_table[i].time < min_time) {
                min_time = time_table[i].time;
                choice = time_table[i].page;
                time_idx = i;
            }
        }

        if (table[choice].dirty == 1){
            written++;
        }

        table[choice].addr = -1;
        table[choice].rw = ' ';

        table[page].addr = 1;
        table[page].rw = rw;
        table[page].dirty = (rw == 'W') ? 1 : 0;

        time_table[time_idx].time = global_time++;
        time_table[time_idx].page = page;

        continue;
    }
    print_result();
    free_table(table);
    free_time_table(time_table);
}

void dense_random(int num_pages, int page_size, FILE* file, int dense_size, char* debug_flag, FILE* outfile){
   struct mem_address* table = create_table(dense_size);
       
    for (int i = 0; i < dense_size; i++){
        table[i].addr = -1;
        table[i].time = INF;
    }
    
    struct page_time* time_table = create_time_table(num_pages);

    int s = find_s(page_size);

    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        if (debug_flag[0] == 'd') fprintf(outfile, "Endereço: %x ", addr);
        pg.addr = addr >> s;
        pg.rw = rw;
        int page = pg.addr;
        char rw = pg.rw;
        int rand_idx;

        if (table[page].addr == 1) {
            hit++;
            if (debug_flag[0] == 'd') fprintf(outfile, "Hit\n");
            if (rw == 'W'){
                table[page].dirty = 1;
            }
            continue;
        }
        
        miss++;
        if (debug_flag[0] == 'd') fprintf(outfile, "Miss\n");

        if (pages_count < num_pages){
            table[page].addr = 1;
            table[page].rw = rw;
            table[page].dirty = (rw == 'W') ? 1 : 0;
            time_table[pages_count].time = global_time++;
            time_table[pages_count].page = page;
            pages_count++;
            continue;
        }

    
        rand_idx = rand() % num_pages;

        int choice = time_table[rand_idx].page;
        
        if (table[choice].dirty == 1){
            written++;
        }

        table[choice].addr = -1;
        table[choice].rw = ' ';

        table[page].addr = 1;
        table[page].rw = rw;
        table[page].dirty = (rw == 'W') ? 1 : 0;

        time_table[rand_idx].time = global_time++;
        time_table[rand_idx].page = page;

        continue;
    }
    print_result();
    free_table(table);
    free_time_table(time_table);
}


void dense_2a(int num_pages, int page_size, FILE* file, int dense_size, char* debug_flag, FILE* outfile){
   struct mem_address* table = create_table(dense_size);
       
    for (int i = 0; i < dense_size; i++){
        table[i].addr = -1;
        table[i].time = INF;
    }

    struct page_time* time_table = create_time_table(num_pages);

    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        if (debug_flag[0] == 'd') fprintf(outfile, "Endereço: %x ", addr);
        int s = find_s(page_size);
        pg.addr = addr >> s;
        pg.rw = rw;
        int page = pg.addr;
        char rw = pg.rw;
        int min_time = INF;
        int fifo_first = 0;
        int time_idx = 0;


        if (table[page].addr == 1) {
            hit++;
            if (debug_flag[0] == 'd') fprintf(outfile, "Hit\n");
            for (int i = 0; i < num_pages; i++) {
                if (time_table[i].page == page){
                    time_table[i].time = 1;
                    break;
                }
            }
            if (rw == 'W'){
                table[page].dirty = 1;
            }
            continue;

        }
        
        miss++;
        if (debug_flag[0] == 'd') fprintf(outfile, "Miss\n");

        if (pages_count < num_pages){
            table[page].addr = 1;
            table[page].rw = rw;
            table[page].dirty = (rw == 'W') ? 1 : 0;
            time_table[pages_count].time = 0;
            time_table[pages_count].page = page;
            pages_count++;
            continue;
        }
        
        int choice = -1;

        for (int i = 0; i < num_pages; i++) {
            int idx = (second_chance_idx + i) % num_pages;
            if (time_table[idx].time == 1) {
                time_table[idx].time = 0;
                continue;
            }
            else{
                choice = idx;
                break;
            }
            continue;
        }

        if (choice == -1) choice = second_chance_idx;
        second_chance_idx = (choice + 1) % num_pages;

        int table_idx = time_table[choice].page;

        if (table[table_idx].dirty == 1){
            written++;
        }

        table[table_idx].addr = -1;
        table[table_idx].rw = ' ';

        table[page].addr = 1;
        table[page].rw = rw;
        table[page].dirty = (rw == 'W') ? 1 : 0;


        time_table[choice].time = global_time++;
        time_table[choice].page = page;

        continue;
    }
    
    print_result();
    free_table(table);
    free_time_table(time_table);
}
