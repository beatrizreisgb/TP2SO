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

int power(int base, int exp) {
    int resultado = 1;
    for (int i = 0; i < exp; i++) {
        resultado *= base;
    }
    return resultado;
}


unsigned find_s(int page_size){
    unsigned s, tmp;
    // Derivar o valor de s: 
    tmp = page_size;
    s = 0;
    while (tmp>1) {
        tmp = tmp>>1;
        s++;
    }
    return s;
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
        dense_table[page].rw = rw;
        dense_table[page].time = global_time++;
        pages_count++;
        return;
    }

    // for (int i = 0; i < dense_size; i++) {
    //     if (dense_table[i].time < min_time) {
    //         min_time = dense_table[i].time;
    //         fifo_first = i;
    //     }
    // }

    if (dense_table[fifo_first].rw == 'W'){
        written++;
    }

    dense_table[fifo_first].addr = -1;
    dense_table[fifo_first].time = INF;
    dense_table[fifo_first].rw = ' ';
    dense_table[page].addr = 1;
    dense_table[page].time = global_time++;
    dense_table[page].rw = rw;

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
        dense_table[page].rw = rw;
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
    dense_table[lru].rw = ' ';
    dense_table[page].addr = 1;
    dense_table[page].time = global_time++;
    dense_table[page].rw = rw;

    return;
}

void dense_random(struct mem_address* dense_table, int num_pages, int dense_size, struct mem_address pg){
    int page = pg.addr;
    char rw = pg.rw;
    int rand_idx;

    if (dense_table[page].addr == 1) {
        hit++;
        return;
    }
    
    miss++;

    if (pages_count < num_pages){
        dense_table[page].addr = 1;
        dense_table[page].rw = rw;
        pages_count++;
        return;
    }
    
    rand_idx = rand() % dense_size;
    

    if (dense_table[rand_idx].rw == 'W'){
        written++;
    }

    dense_table[rand_idx].addr = -1;
    dense_table[rand_idx].rw = ' ';
    dense_table[page].addr = 1;
    dense_table[page].rw = rw;

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
    int page_size = atoi(argv[3])*1024;
    int mem_size = atoi(argv[4])*1024;
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
    int dense_size1 = mem_size;
    int dense_size = power(2, (32-find_s(page_size)));

    // printf("s: %d\n", find_s(page_size));
    // printf("dense_size: %d\n", dense_size);
    // printf("dense2: %d\n", dense_size2);

    if (table_type[0] == 'd'){
        table = (struct mem_address*) malloc(dense_size * sizeof(struct mem_address)); 
        for (int i = 0; i < dense_size; i++){
            table[i].addr = -1;
            table[i].time = INF;
        }
    }

    // leitura do endereço e operação
    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        unsigned s = find_s(page_size);
        unsigned p = addr >> s; //end da pagina

        struct mem_address page;
        page.addr = p;
        page.rw = rw;

        if (mode[0] == 'l') {
            dense_lru(table, num_pages, dense_size, page);
        } else if (mode[0] == 'f') {
            dense_fifo(table, num_pages, dense_size, page);
        } else if (mode[0] == 'r') {
            dense_random(table, num_pages, dense_size, page);
        } else{
            // dense_2a(table, num_pages, dense_size, page, second_chance);
        }
    }

    printf("Hits: %d\n", hit);
    printf("Misses/Page faults: %d\n", miss);
    printf("Writes: %d\n", written);

    fclose(file);

    return (0);
}