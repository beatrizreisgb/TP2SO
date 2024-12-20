#include "../include/hier3.h"
#include "../include/mem_address.h"

#define INF 1000000000

void hier3_fifo(int num_pages, int page_size, FILE* file, char* debug_flag, FILE* outfile){
    int s = find_s(page_size);
    int shift = 32 - 10 - s;

    int rows = power(2, 4);
    int columns = power(2, 6);
    int width = power(2, shift);

    struct mem_address*** table = (struct mem_address***)malloc(rows * sizeof(struct mem_address**));

    for (int i = 0; i < rows; i++) {
       table[i] = (struct mem_address**)malloc(columns * sizeof(struct mem_address*));
       for (int j = 0; j < columns; j++) {
           table[i][j] = (struct mem_address*)malloc(width * sizeof(struct mem_address));
       }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            for (int k = 0; k < width; k++) {
                table[i][j][k].addr = -1;
            }
        }
    }

    struct page_time* time_table;
    time_table = (struct page_time*) malloc(num_pages * sizeof(struct page_time));
    
    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        if (debug_flag[0] == 'd') fprintf(outfile, "Endereço: %x ", addr);
        pg.addr = addr >> s;
        pg.first = find_row(pg.addr, shift + 6);
        int temp = find_row(pg.addr, shift);
        pg.second = find_column(temp, 6);
        pg.third = find_column(pg.addr, shift);
        pg.rw = rw;

        int page = pg.addr;
        char rw = pg.rw;

        int found = 0;
        int min_time = INF;
        int choice_first = 0;
        int choice_second = 0;
        int choice_third = 0;
        int time_idx = 0;

        if (table[pg.first][pg.second][pg.third].addr == 1) {
            hit++;
            if (debug_flag[0] == 'd') fprintf(outfile, "Hit\n");
            if (rw == 'W'){
                table[pg.first][pg.second][pg.third].dirty = 1;
            }
            continue;
        }

        miss++;
        if (debug_flag[0] == 'd') fprintf(outfile, "Miss\n");

        if (pages_count < num_pages){
            table[pg.first][pg.second][pg.third].addr = 1;
            table[pg.first][pg.second][pg.third].rw = rw;
            table[pg.first][pg.second][pg.third].dirty = (rw == 'W') ? 1 : 0;
            table[pg.first][pg.second][pg.third].time = global_time++;

            time_table[pages_count].time = global_time++;
            time_table[pages_count].first = pg.first;
            time_table[pages_count].second = pg.second;
            time_table[pages_count].third = pg.third;

            pages_count++;
            continue;
        }

        for (int i = 0; i < num_pages; i++) {
            if (time_table[i].time < min_time) {
                min_time = time_table[i].time;
                choice_first = time_table[i].first;
                choice_second = time_table[i].second;
                choice_third = time_table[i].third;
                time_idx = i;
            }
        }

        table[choice_first][choice_second][choice_third].addr = -1;
        table[choice_first][choice_second][choice_third].rw = ' ';

        if (table[choice_first][choice_second][choice_third].dirty == 1){
            written++;
        }

        table[pg.first][pg.second][pg.third].addr = 1;
        table[pg.first][pg.second][pg.third].rw = rw;
        table[pg.first][pg.second][pg.third].dirty = (rw == 'W') ? 1 : 0;


        time_table[time_idx].time = global_time++;
        time_table[time_idx].first = pg.first;
        time_table[time_idx].second = pg.second;
        time_table[time_idx].third = pg.third;

        continue;
    }
    print_result();
}

void hier3_lru(int num_pages, int page_size, FILE* file, char* debug_flag, FILE* outfile){
    int s = find_s(page_size);
    int shift = 32 - 10 - s;

    int rows = power(2, 4);
    int columns = power(2, 6);
    int width = power(2, shift);

    struct mem_address*** table = (struct mem_address***)malloc(rows * sizeof(struct mem_address**));

    for (int i = 0; i < rows; i++) {
       table[i] = (struct mem_address**)malloc(columns * sizeof(struct mem_address*));
       for (int j = 0; j < columns; j++) {
           table[i][j] = (struct mem_address*)malloc(width * sizeof(struct mem_address));
       }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            for (int k = 0; k < width; k++) {
                table[i][j][k].addr = -1;
            }
        }
    }

    struct page_time* time_table;
    time_table = (struct page_time*) malloc(num_pages * sizeof(struct page_time));
    
    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        if (debug_flag[0] == 'd') fprintf(outfile, "Endereço: %x ", addr);
        pg.addr = addr >> s;
        pg.first = find_row(pg.addr, shift + 6);
        int temp = find_row(pg.addr, shift);
        pg.second = find_column(temp, 6);
        pg.third = find_column(pg.addr, shift);
        pg.rw = rw;

        int page = pg.addr;
        char rw = pg.rw;

        int found = 0;
        int min_time = INF;
        int choice_first = 0;
        int choice_second = 0;
        int choice_third = 0;
        int time_idx = 0;

        if (table[pg.first][pg.second][pg.third].addr == 1) {
            hit++;
            if (debug_flag[0] == 'd') fprintf(outfile, "Hit\n");
            for (int i = 0; i < num_pages; i++) {
                if (time_table[i].first == pg.first && time_table[i].second == pg.second && time_table[i].third == pg.third){
                    time_table[i].time = global_time++;
                    break;
                }
            }
            if (rw == 'W'){
                table[pg.first][pg.second][pg.third].dirty = 1;
            }

            continue;
        }

        miss++;
        if (debug_flag[0] == 'd') fprintf(outfile, "Miss\n");

        if (pages_count < num_pages){
            table[pg.first][pg.second][pg.third].addr = 1;
            table[pg.first][pg.second][pg.third].rw = rw;
            table[pg.first][pg.second][pg.third].dirty = (rw == 'W') ? 1 : 0;
            table[pg.first][pg.second][pg.third].time = global_time++;

            time_table[pages_count].time = global_time++;
            time_table[pages_count].first = pg.first;
            time_table[pages_count].second = pg.second;
            time_table[pages_count].third = pg.third;

            pages_count++;
            continue;
        }

        for (int i = 0; i < num_pages; i++) {
            if (time_table[i].time < min_time) {
                min_time = time_table[i].time;
                choice_first = time_table[i].first;
                choice_second = time_table[i].second;
                choice_third = time_table[i].third;
                time_idx = i;
            }
        }

        table[choice_first][choice_second][choice_third].addr = -1;
        table[choice_first][choice_second][choice_third].rw = ' ';

        if (table[choice_first][choice_second][choice_third].dirty == 1){
            written++;
        }

        table[pg.first][pg.second][pg.third].addr = 1;
        table[pg.first][pg.second][pg.third].rw = rw;
        table[pg.first][pg.second][pg.third].dirty = (rw == 'W') ? 1 : 0;



        time_table[time_idx].time = global_time++;
        time_table[time_idx].first = pg.first;
        time_table[time_idx].second = pg.second;
        time_table[time_idx].third = pg.third;

        continue;
    }
    print_result();
}

void hier3_random(int num_pages, int page_size, FILE* file, char* debug_flag, FILE* outfile){
    int s = find_s(page_size);
    int shift = 32 - 10 - s;

    int rows = power(2, 4);
    int columns = power(2, 6);
    int width = power(2, shift);

    struct mem_address*** table = (struct mem_address***)malloc(rows * sizeof(struct mem_address**));

    for (int i = 0; i < rows; i++) {
       table[i] = (struct mem_address**)malloc(columns * sizeof(struct mem_address*));
       for (int j = 0; j < columns; j++) {
           table[i][j] = (struct mem_address*)malloc(width * sizeof(struct mem_address));
       }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            for (int k = 0; k < width; k++) {
                table[i][j][k].addr = -1;
            }
        }
    }

    struct page_time* time_table;
    time_table = (struct page_time*) malloc(num_pages * sizeof(struct page_time));
    
    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        if (debug_flag[0] == 'd') fprintf(outfile, "Endereço: %x ", addr);
        pg.addr = addr >> s;
        pg.first = find_row(pg.addr, shift + 6);
        int temp = find_row(pg.addr, shift);
        pg.second = find_column(temp, 6);
        pg.third = find_column(pg.addr, shift);
        pg.rw = rw;

        int page = pg.addr;
        char rw = pg.rw;

        int found = 0;
        int min_time = INF;
        int fifo_first = 0;
        int fifo_second = 0;
        int fifo_third = 0;
        int time_idx = 0;

        if (table[pg.first][pg.second][pg.third].addr == 1) {
            hit++;
            if (debug_flag[0] == 'd') fprintf(outfile, "Hit\n");
            for (int i = 0; i < num_pages; i++) {
                if (time_table[i].first == pg.first && time_table[i].second == pg.second && time_table[i].third == pg.third){
                    time_table[i].time = global_time++;
                    break;
                }
            }
            if (rw == 'W'){
                table[pg.first][pg.second][pg.third].dirty = 1;
            }
            continue;
        }

        miss++;
        if (debug_flag[0] == 'd') fprintf(outfile, "Miss\n");

        if (pages_count < num_pages){
            table[pg.first][pg.second][pg.third].addr = 1;
            table[pg.first][pg.second][pg.third].rw = rw;
            table[pg.first][pg.second][pg.third].dirty = (rw == 'W') ? 1 : 0;
            table[pg.first][pg.second][pg.third].time = global_time++;

            time_table[pages_count].time = global_time++;
            time_table[pages_count].first = pg.first;
            time_table[pages_count].second = pg.second;
            time_table[pages_count].third = pg.third;

            pages_count++;
            continue;
        }

        time_idx = rand() % num_pages;

        int choice_first = time_table[time_idx].first;
        int choice_second = time_table[time_idx].second;
        int choice_third = time_table[time_idx].third;

        table[choice_first][choice_second][choice_third].addr = -1;
        table[choice_first][choice_second][choice_third].rw = ' ';

        if (table[choice_first][choice_second][choice_third].dirty == 1){
            written++;
        }

        table[pg.first][pg.second][pg.third].addr = 1;
        table[pg.first][pg.second][pg.third].rw = rw;
        table[pg.first][pg.second][pg.third].dirty = (rw == 'W') ? 1 : 0;

        time_table[time_idx].time = global_time++;
        time_table[time_idx].first = pg.first;
        time_table[time_idx].second = pg.second;
        time_table[time_idx].third = pg.third;

        continue;
    }
    print_result();
}

void hier3_2a(int num_pages, int page_size, FILE* file, char* debug_flag, FILE* outfile){
    int s = find_s(page_size);
    int shift = 32 - 10 - s;

    int rows = power(2, 4);
    int columns = power(2, 6);
    int width = power(2, shift);

    struct mem_address*** table = (struct mem_address***)malloc(rows * sizeof(struct mem_address**));

    for (int i = 0; i < rows; i++) {
       table[i] = (struct mem_address**)malloc(columns * sizeof(struct mem_address*));
       for (int j = 0; j < columns; j++) {
           table[i][j] = (struct mem_address*)malloc(width * sizeof(struct mem_address));
       }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            for (int k = 0; k < width; k++) {
                table[i][j][k].addr = -1;
            }
        }
    }

    struct page_time* time_table;
    time_table = (struct page_time*) malloc(num_pages * sizeof(struct page_time));
    
    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        if (debug_flag[0] == 'd') fprintf(outfile, "Endereço: %x ", addr);
        pg.addr = addr >> s;
        pg.first = find_row(pg.addr, shift + 6);
        int temp = find_row(pg.addr, shift);
        pg.second = find_column(temp, 6);
        pg.third = find_column(pg.addr, shift);
        pg.rw = rw;

        int page = pg.addr;
        char rw = pg.rw;

        int found = 0;
        int min_time = INF;
        int fifo_first = 0;
        int fifo_second = 0;
        int fifo_third = 0;
        int time_idx = 0;

        if (table[pg.first][pg.second][pg.third].addr == 1) {
            hit++;
            if (debug_flag[0] == 'd') fprintf(outfile, "Hit\n");
            for (int i = 0; i < num_pages; i++) {
                if (time_table[i].first == pg.first && time_table[i].second == pg.second && time_table[i].third == pg.third){
                    time_table[i].time = 1;
                    break;
                }
            }
            if (rw == 'W'){
                table[pg.first][pg.second][pg.third].dirty = 1;
            }
            continue;
        }

        miss++;
        if (debug_flag[0] == 'd') fprintf(outfile, "Miss\n");

        if (pages_count < num_pages){
            table[pg.first][pg.second][pg.third].addr = 1;
            table[pg.first][pg.second][pg.third].rw = rw;
            table[pg.first][pg.second][pg.third].dirty = (rw == 'W') ? 1 : 0;
            table[pg.first][pg.second][pg.third].time =0;

            time_table[pages_count].time = 0;
            time_table[pages_count].first = pg.first;
            time_table[pages_count].second = pg.second;
            time_table[pages_count].third = pg.third;

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
        }

        if (choice == -1) choice = second_chance_idx;
        second_chance_idx = (choice + 1) % num_pages;

        int first_idx = time_table[choice].first;
        int second_idx = time_table[choice].second;
        int third_idx = time_table[choice].third;

        table[first_idx][second_idx][third_idx].addr = -1;
        table[first_idx][second_idx][third_idx].rw = ' ';

        if (table[first_idx][second_idx][third_idx].dirty == 1){
            written++;
        }

        table[pg.first][pg.second][pg.third].addr = 1;
        table[pg.first][pg.second][pg.third].rw = rw;
        table[pg.first][pg.second][pg.third].dirty = (rw == 'W') ? 1 : 0;


        time_table[choice].time = 0;
        time_table[choice].first = pg.first;
        time_table[choice].second = pg.second;
        time_table[choice].third = pg.third;

        continue;
    }
    print_result();
}