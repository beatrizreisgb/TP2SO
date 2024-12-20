#include "../include/hier2.h"
#include "../include/mem_address.h"

#define INF 1000000000

void hier2_fifo(int num_pages, int page_size, FILE* file){
    int s = find_s(page_size);
    int shift = 32 - 8 - s;

    int rows = power(2, 8);
    int columns = power(2, shift);

    struct mem_address** table = create_2aTable(rows, columns);

    struct page_time* time_table = create_time_table(num_pages);
    
    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        pg.addr = addr >> s;
        pg.first = find_row(pg.addr, shift);
        pg.second = find_column(pg.addr, shift);
        pg.rw = rw;

        int page = pg.addr;
        char rw = pg.rw;

        int min_time = INF;
        int choice_first = 0;
        int choice_second = 0;
        int time_idx = 0;

        if (table[pg.first][pg.second].addr == 1) {
            hit++;
            if (rw == 'W'){
                table[pg.first][pg.second].dirty = 1;
            }
            continue;
        }

        miss++;

        if (pages_count < num_pages){
            table[pg.first][pg.second].addr = 1;
            table[pg.first][pg.second].rw = rw;
            table[pg.first][pg.second].dirty = (rw == 'W') ? 1 : 0;
            table[pg.first][pg.second].time = global_time++;

            time_table[pages_count].time = global_time++;
            time_table[pages_count].first = pg.first;
            time_table[pages_count].second = pg.second;

            pages_count++;
            continue;
        }

        for (int i = 0; i < num_pages; i++) {
            if (time_table[i].time < min_time) {
                min_time = time_table[i].time;
                choice_first = time_table[i].first;
                choice_second = time_table[i].second;
                time_idx = i;
            }
        }

        table[choice_first][choice_second].addr = -1;
        table[choice_first][choice_second].rw = ' ';

        if (table[choice_first][choice_second].dirty == 1){
            written++;
        }

        table[pg.first][pg.second].addr = 1;
        table[pg.first][pg.second].rw = rw;
        table[pg.first][pg.second].dirty = (rw == 'W') ? 1 : 0;

        time_table[time_idx].time = global_time++;
        time_table[time_idx].first = pg.first;
        time_table[time_idx].second = pg.second;

        continue;
    }
    print_result();
}

void hier2_lru(int num_pages, int page_size, FILE* file){
    int s = find_s(page_size);
    int shift = 32 - 8 - s;

    int rows = power(2, 8);
    int columns = power(2, shift);

    struct mem_address** table = create_2aTable(rows, columns);

    struct page_time* time_table = create_time_table(num_pages);
    
    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        pg.addr = addr >> s;
        pg.first = find_row(pg.addr, shift);
        pg.second = find_column(pg.addr, shift);
        pg.rw = rw;

        int page = pg.addr;
        char rw = pg.rw;

        int min_time = INF;
        int choice_first = 0;
        int choice_second = 0;
        int time_idx = 0;

        if (table[pg.first][pg.second].addr == 1) {
            hit++;
            for (int i = 0; i < num_pages; i++) {
                if (time_table[i].first == pg.first && time_table[i].second == pg.second){
                    time_table[i].time = global_time++;
                    break;
                }
            }
            if (rw == 'W'){
                table[pg.first][pg.second].dirty = 1;
            }
            continue;
        }

       

        miss++;

        if (pages_count < num_pages){
            table[pg.first][pg.second].addr = 1;
            table[pg.first][pg.second].rw = rw;
            table[pg.first][pg.second].dirty = (rw == 'W') ? 1 : 0;
            table[pg.first][pg.second].time = global_time++;

            time_table[pages_count].time = global_time++;
            time_table[pages_count].first = pg.first;
            time_table[pages_count].second = pg.second;

            pages_count++;
            continue;
        }

        for (int i = 0; i < num_pages; i++) {
            if (time_table[i].time < min_time) {
                min_time = time_table[i].time;
                choice_first = time_table[i].first;
                choice_second = time_table[i].second;
                time_idx = i;
            }
        }

        table[choice_first][choice_second].addr = -1;
        table[choice_first][choice_second].rw = ' ';

        if (table[choice_first][choice_second].dirty == 1){
            written++;
        }

        table[pg.first][pg.second].addr = 1;
        table[pg.first][pg.second].rw = rw;
        table[pg.first][pg.second].dirty = (rw == 'W') ? 1 : 0;

        time_table[time_idx].time = global_time++;
        time_table[time_idx].first = pg.first;
        time_table[time_idx].second = pg.second;

        continue;
    }
    print_result();
}

void hier2_random(int num_pages, int page_size, FILE* file){
    int s = find_s(page_size);
    int shift = 32 - 8 - s;

    int rows = power(2, 8);
    int columns = power(2, shift);

    struct mem_address** table = create_2aTable(rows, columns);

    struct page_time* time_table = create_time_table(num_pages);
    
    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        pg.addr = addr >> s;
        pg.first = find_row(pg.addr, shift);
        pg.second = find_column(pg.addr, shift);
        pg.rw = rw;

        int page = pg.addr;
        char rw = pg.rw;

        int min_time = INF;
        int time_idx = 0;

        if (table[pg.first][pg.second].addr == 1) {
            hit++;
            for (int i = 0; i < num_pages; i++) {
                if (time_table[i].first == pg.first && time_table[i].second == pg.second){
                    time_table[i].time = global_time++;
                    break;
                }
            }
            if (rw == 'W'){
                table[pg.first][pg.second].dirty = 1;
            }
            continue;
        }

        miss++;

        if (pages_count < num_pages){
            table[pg.first][pg.second].addr = 1;
            table[pg.first][pg.second].rw = rw;
            table[pg.first][pg.second].dirty = (rw == 'W') ? 1 : 0;
            table[pg.first][pg.second].time = global_time++;

            time_table[pages_count].time = global_time++;
            time_table[pages_count].first = pg.first;
            time_table[pages_count].second = pg.second;

            pages_count++;
            continue;
        }

        time_idx = rand() % num_pages;

        int choice_first = time_table[time_idx].first;
        int choice_second = time_table[time_idx].second;

        if (table[choice_first][choice_second].dirty == 1){
            written++;
        }

        table[choice_first][choice_second].addr = -1;
        table[choice_first][choice_second].rw = ' ';

        table[pg.first][pg.second].addr = 1;
        table[pg.first][pg.second].rw = rw;
        table[pg.first][pg.second].dirty = (rw == 'W') ? 1 : 0;

        time_table[time_idx].time = global_time++;
        time_table[time_idx].first = pg.first;
        time_table[time_idx].second = pg.second;

        continue;
    }
    print_result();
}

void hier2_2a(int num_pages, int page_size, FILE* file){
    int s = find_s(page_size);
    int shift = 32 - 8 - s;

    int rows = power(2, 8);
    int columns = power(2, shift);

    struct mem_address** table = create_2aTable(rows, columns);

    struct page_time* time_table = create_time_table(num_pages);
    
    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        pg.addr = addr >> s;
        pg.first = find_row(pg.addr, shift);
        pg.second = find_column(pg.addr, shift);
        pg.rw = rw;

        int page = pg.addr;
        char rw = pg.rw;

        int min_time = INF;
        int fifo_first = 0;
        int fifo_second = 0;
        int time_idx = 0;

        if (table[pg.first][pg.second].addr == 1) {
            hit++;
            for (int i = 0; i < num_pages; i++) {
                if (time_table[i].first == pg.first && time_table[i].second == pg.second){
                    time_table[i].time = 1;
                    break;
                }
            }
            if (rw == 'W'){
                table[pg.first][pg.second].dirty = 1;
            }
            continue;
        }

        miss++;

        if (pages_count < num_pages){
            table[pg.first][pg.second].addr = 1;
            table[pg.first][pg.second].rw = rw;
            table[pg.first][pg.second].dirty = (rw == 'W') ? 1 : 0;
            table[pg.first][pg.second].time =0;

            time_table[pages_count].time = 0;
            time_table[pages_count].first = pg.first;
            time_table[pages_count].second = pg.second;

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

        table[first_idx][second_idx].addr = -1;
        table[first_idx][second_idx].rw = ' ';

        if (table[first_idx][second_idx].dirty == 1){
            written++;
        }

        table[pg.first][pg.second].addr = 1;
        table[pg.first][pg.second].rw = rw;
        table[pg.first][pg.second].dirty = (rw == 'W') ? 1 : 0;

        time_table[choice].time = 0;
        time_table[choice].first = pg.first;
        time_table[choice].second = pg.second;

        continue;
    }
    print_result();
}