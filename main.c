#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define INF 1000000000

int fifo_first = 0;
int hit = 0;
int miss = 0;
int global_time = 0;
int written = 0;
int second_chance_idx = 0;
int pages_count = 0;

struct mem_address{
    unsigned addr;
    unsigned time;
    char rw;
};

void inverted_fifo(struct mem_address* inverted_table, int num_pages, struct mem_address pg){
    int page = pg.addr;
    char rw = pg.rw;

    for (int i = 0; i < num_pages; i++) {
        if (inverted_table[i].addr == page) {
            hit++;
            return;
        }
        if(inverted_table[i].addr == -1){
            inverted_table[i].addr = page;
            miss++;
            return;
        }
    }
    miss++;

    if (inverted_table[fifo_first].rw == 'W'){
        written++;
    }

    inverted_table[fifo_first].addr = page;
    fifo_first = (fifo_first + 1) % num_pages;
    return;
}

void inverted_lru(struct mem_address* inverted_table, int num_pages, struct mem_address pg){
    int page = pg.addr;
    char rw = pg.rw;

    for (int i = 0; i < num_pages; i++) {
        if (inverted_table[i].addr == page) {
            hit++;
            printf("hit\n");
            inverted_table[i].time = global_time;
            global_time++;
            return;
        }
        if(inverted_table[i].addr == -1){
            inverted_table[i].addr = page;
            inverted_table[i].time = global_time;
            miss++;
            printf("miss tinha espaço\n");
            global_time++;
            return;
        }
    }
    miss++;
    printf("miss\n");
    int lru = 0; // least recently used position

    for (int i = 0; i < num_pages; i++) {
        if (inverted_table[i].time < inverted_table[lru].time) {
            lru = i;
        }
    }
    
    if (inverted_table[lru].rw == 'W'){
        written++;
    }

    inverted_table[lru].addr = page;
    inverted_table[lru].time = global_time;

    global_time++;

    return;
}

void inverted_random(struct mem_address* inverted_table, int num_pages, struct mem_address pg){
    int page = pg.addr;
    char rw = pg.rw;

    for (int i = 0; i < num_pages; i++) {
        if (inverted_table[i].addr == page) {
            hit++;
            return;
        }
        if(inverted_table[i].addr == -1){
            inverted_table[i].addr = page;
            miss++;
            return;
        }
    }
    miss++;
    int rand_idx = rand() % num_pages;
    if (inverted_table[rand_idx].rw == 'W'){
        written++;
    }
    inverted_table[rand_idx].addr = page;
    return;
}

void inverted_2a(struct mem_address* inverted_table, int num_pages, struct mem_address pg, int* second_chance){
    int page = pg.addr;
    char rw = pg.rw;

    for (int i = 0; i < num_pages; i++) {
        if (inverted_table[i].addr == page) {
            hit++;
            second_chance[i] = 1;
            return;
        }
        if(inverted_table[i].addr == -1){
            inverted_table[i].addr = page;
            miss++;
            return;
        }
    }
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

    if (inverted_table[choice].rw == 'W'){
        written++;
    }

    inverted_table[choice].addr = page;
    second_chance[choice] = 0;
    return;
}

void dense_fifo(struct mem_address* dense_table, int num_pages, int dense_size, struct mem_address pg){
    int page = pg.addr;
    char rw = pg.rw;
    int min_time = INF;
    int fifo_first = 0;

    if (dense_table[page].addr == 1) {
        hit++;
        global_time++;
        return;
    }
    
    miss++;

    if (pages_count < num_pages){
        dense_table[page].addr = 1;
        dense_table[page].time = global_time++;
        pages_count++;
        return;
    }

    for (int i = 0; i < dense_size; i++) {
        if (dense_table[i].time < min_time) {
            min_time = dense_table[i].time;
            fifo_first = i;
        }
    }

    if (dense_table[fifo_first].rw == 'W'){
        written++;
    }

    dense_table[fifo_first].addr = -1;
    dense_table[fifo_first].time = INF;
    dense_table[page].addr = 1;
    dense_table[page].time = global_time++;

    return;
}

void dense_lru(struct mem_address* dense_table, int num_pages, int dense_size, struct mem_address pg){
    int page = pg.addr;
    char rw = pg.rw;
    int min_time = INF;
    int lru = 0;

    if (dense_table[page].addr == 1) {
        dense_table[page].time = global_time++;
        hit++;
        return;
    }
    
    miss++;

    if (pages_count < num_pages){
        dense_table[page].addr = 1;
        dense_table[page].time = global_time++;
        pages_count++;
        return;
    }

    for (int i = 0; i < dense_size; i++) {
        if (dense_table[i].time < min_time) {
            min_time = dense_table[i].time;
            lru = i;
        }
    }

    if (dense_table[lru].rw == 'W'){
        written++;
    }

    dense_table[lru].addr = -1;
    dense_table[lru].time = INF;
    dense_table[page].addr = 1;
    dense_table[page].time = global_time++;

    return;
}



int main (int argc, char* argv[]){
    
    srand(time(NULL));

    unsigned addr;
    char rw;
    int page_faults = 0;
    FILE *file;

    char* mode = argv[1];
    char* file_name = argv[2];
    int page_size = atoi(argv[3]);
    int mem_size = atoi(argv[4]);
    char* table_type = argv[5];
    char* debug_flag = argv[6];

    int num_pages = mem_size / page_size;

    file = fopen(file_name, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    struct mem_address* table;
    int* second_chance;
    int dense_size = mem_size*1024;

    if (table_type[0] == 'i'){
        table = (struct mem_address*) malloc(num_pages * sizeof(struct mem_address));
        second_chance = (int*) malloc(num_pages * sizeof(int));
        for (int i = 0; i < num_pages; i++){
            table[i].addr = -1;
            table[i].time = 0;
        }

    } else if (table_type[0] == 'd'){
        table = (struct mem_address*) malloc(dense_size * sizeof(struct mem_address)); 
        for (int i = 0; i < dense_size; i++){
            table[i].addr = -1;
            table[i].time = INF;
        }
    }

    // leitura do endereço e operação
    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        unsigned s, tmp;
        // Derivar o valor de s: 
        tmp = page_size;
        s = 0;
        while (tmp>1) {
            tmp = tmp>>1;
            s++;
        }

        int p = addr >> s; //end da pagina

        struct mem_address page;
        page.addr = p;
        page.rw = rw;

        if (mode[0] == 'l') {
            inverted_lru(table, num_pages, page);
        } else if (mode[0] == 'f') {
            inverted_fifo(table, num_pages, page);
        } else if (mode[0] == 'r') {
            inverted_random(table, num_pages, page);
        } else{
            inverted_2a(table, num_pages, page, second_chance);
        }

        // if (mode[0] == 'l') {
        //     dense_lru(table, num_pages, dense_size, page);
        // } else if (mode[0] == 'f') {
        //     dense_fifo(table, num_pages, dense_size, page);
        // } else if (mode[0] == 'r') {
        //     // dense_random(table, num_pages, dense_size, page);
        // } else{
        //     // dense_2a(table, num_pages, dense_size, page, second_chance);
        // }

        for (int i = 0; i < dense_size; i++){
            if (table[i].addr == 1) printf("%d ", i);
        }
        printf("\n");

    }

    printf("Hits: %d\n", hit);
    printf("Misses/Page faults: %d\n", miss);
    printf("Writes: %d\n", written);

    fclose(file);

    return (0);
}