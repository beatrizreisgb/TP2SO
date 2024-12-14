#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int fifo_first = 0;
int hit = 0;
int miss = 0;
int lru_time = 0;
int written = 0;
int second_chance_idx = 0;

struct mem_address{
    unsigned addr;
    unsigned time;
    char rw;
};

void _fifo(struct mem_address* inverted_table, int num_pages, struct mem_address pg){
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

void _lru(struct mem_address* inverted_table, int num_pages, struct mem_address pg){
    int page = pg.addr;
    char rw = pg.rw;

    for (int i = 0; i < num_pages; i++) {
        if (inverted_table[i].addr == page) {
            hit++;
            inverted_table[i].time = lru_time;
            lru_time++;
            return;
        }
        if(inverted_table[i].addr == -1){
            inverted_table[i].addr = page;
            inverted_table[i].time = lru_time;
            miss++;
            lru_time++;
            return;
        }
    }
    miss++;
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
    inverted_table[lru].time = lru_time;

    lru_time++;

    return;
}

void _random(struct mem_address* inverted_table, int num_pages, struct mem_address pg){
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
    printf("rand_idx: %d\n", rand_idx);
    if (inverted_table[rand_idx].rw == 'W'){
        written++;
    }
    inverted_table[rand_idx].addr = page;
    return;
}

void _2a(struct mem_address* inverted_table, int num_pages, struct mem_address pg, int* second_chance){
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
    char* debug_flag = argv[5];

    int num_pages = mem_size / page_size;

    file = fopen(file_name, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    struct mem_address inverted_table[num_pages];
    int second_chance[num_pages];

    for (int i = 0; i < num_pages; i++){
        inverted_table[i].addr = -1;
        inverted_table[i].time = 0;
        second_chance[i] = 0;
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
            _lru(inverted_table, num_pages, page);
        } else if (mode[0] == 'f') {
            _fifo(inverted_table, num_pages, page);
        } else if (mode[0] == 'r') {
            _random(inverted_table, num_pages, page);
        } else{
            _2a(inverted_table, num_pages, page, second_chance);
        }

        // print vector inverted_table
        if (1){
            printf("Inverted table: ");
            for (int i = 0; i < num_pages; i++){
                printf("%d ", inverted_table[i].addr);
            }
            printf("\n");

            printf("Second chance: ");
            for (int i = 0; i < num_pages; i++){
                printf("%d ", second_chance[i]);
            }
            printf("\n");
        }

    }

    printf("Hits: %d\n", hit);
    printf("Misses/Page faults: %d\n", miss);
    printf("Writes: %d\n", written);

    fclose(file);

    return (0);
}