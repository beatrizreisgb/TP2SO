#include "../include/inverted.h"
#include "../include/mem_address.h"

void inverted_fifo(int num_pages, int page_size, FILE* file, char* debug_flag, FILE* outfile){
    struct mem_address* table = create_table(num_pages);
    int choice = 0;

    // Lê o arquivo de entrada
    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        if (debug_flag[0] == 'd') fprintf(outfile, "Endereço: %x ", addr);
        int s = find_s(page_size);
        pg.addr = addr >> s;
        pg.dirty = 0;
        int page = pg.addr;
        int found = 0;

        // Verifica se a página já está na tabela
        for (int i = 0; i < num_pages; i++) {
            if (table[i].addr == page) {
                found++;
                hit++;
                if (debug_flag[0] == 'd') fprintf(outfile, "Hit\n");
                if (rw == 'W'){
                    table[i].dirty = 1;
                }
                break;
            }
            if(table[i].addr == -1){
                table[i].addr = page;
                table[i].dirty = (rw == 'W') ? 1 : 0;
                found++;
                miss++;
                if (debug_flag[0] == 'd') fprintf(outfile, "Miss\n");
                break;
            }
        }
        
        if (found) continue;
        miss++;
        if (debug_flag[0] == 'd') fprintf(outfile, "Miss\n");

        // Se a página substituída foi escrita, incrementa o número de escritas
        if (table[choice].dirty == 1){
            written++;
        }

        // Substitui a página na tabela
        table[choice].addr = page;
        table[choice].dirty = (rw == 'W') ? 1 : 0;

        // Atualiza a posição de escolha para a próxima substituição
        choice = (choice + 1) % num_pages;

        continue;
    }
    print_result();
    free_table(table);
}

void inverted_lru(int num_pages, int page_size, FILE* file, char* debug_flag, FILE* outfile){
    struct mem_address* table = create_table(num_pages);

    // Lê o arquivo de entrada
    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        if (debug_flag[0] == 'd') fprintf(outfile, "Endereço: %x ", addr);
        int s = find_s(page_size);
        pg.addr = addr >> s;
        pg.rw = rw;
        int page = pg.addr;
        char rw = pg.rw;
        int found = 0;

        // Verifica se a página já está na tabela
        for (int i = 0; i < num_pages; i++) {
            if (table[i].addr == page) {
                found++;
                hit++;
                if (debug_flag[0] == 'd') fprintf(outfile, "Hit\n");
                if (rw == 'W'){
                    table[i].dirty = 1;
                }
                table[i].time = global_time;
                global_time++;
                break;
            }
            if(table[i].addr == -1){
                table[i].addr = page;
                table[i].time = global_time;
                table[i].rw = rw;
                table[i].dirty = (rw == 'W') ? 1 : 0;
                global_time++;
                found++;
                miss++;
                if (debug_flag[0] == 'd') fprintf(outfile, "Miss\n");
                break;
            }
        }
        if (found) continue;
        miss++;
        if (debug_flag[0] == 'd') fprintf(outfile, "Miss\n");

        // Encontra a página menos recentemente usada
        int choice = 0; // posição da página menos recentemente usada
        for (int i = 0; i < num_pages; i++) {
            if (table[i].time < table[choice].time) {
                choice = i;
            }
        }
        
        // Se a página substituída foi escrita, incrementa o número de escritas
        if (table[choice].dirty == 1){
            written++;
        }

        // Substitui a página na tabela
        table[choice].addr = page;
        table[choice].rw = rw;
        table[choice].time = global_time;
        table[choice].dirty = (rw == 'W') ? 1 : 0;

        global_time++;

        continue;
    }
    print_result();
    free_table(table);
}

// Algoritmo RANDOM para a tabela invertida
void inverted_random(int num_pages, int page_size, FILE* file, char* debug_flag, FILE* outfile){
    struct mem_address* table = create_table(num_pages);

    // Lê o arquivo de entrada
    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        if (debug_flag[0] == 'd') fprintf(outfile, "Endereço: %x ", addr);
        int s = find_s(page_size);
        pg.addr = addr >> s;
        pg.rw = rw;
        int page = pg.addr;
        char rw = pg.rw;
        int found = 0;

        // Verifica se a página já está na tabela
        for (int i = 0; i < num_pages; i++) {
            if (table[i].addr == page) {
                hit++;
                if (debug_flag[0] == 'd') fprintf(outfile, "Hit\n");
                found++;
                if (rw == 'W'){
                    table[i].dirty = 1;
                }
                break;
            }
            if(table[i].addr == -1){
                table[i].addr = page;
                table[i].rw = rw;
                table[i].dirty = (rw == 'W') ? 1 : 0;
                found++;
                miss++;
                if (debug_flag[0] == 'd') fprintf(outfile, "Miss\n");
                break;
            }
        }
        if (found) continue;
        miss++;
        if (debug_flag[0] == 'd') fprintf(outfile, "Miss\n");

        // Escolhe uma página aleatoriamente para substituir
        int choice = rand() % num_pages;

        // Se a página substituída foi escrita, incrementa o número de escritas
        if (table[choice].dirty == 1){
            written++;
        }

        // Substitui a página na tabela
        table[choice].addr = page;
        table[choice].rw = rw;
        table[choice].dirty = (rw == 'W') ? 1 : 0;

        continue;
    }
    print_result();
    free_table(table);
}

// Algoritmo Second-Chance para a tabela invertida
void inverted_2a(int num_pages, int page_size, FILE* file, char* debug_flag, FILE* outfile){
    struct mem_address* table = create_table(num_pages);
    int* second_chance;
    second_chance = (int*) malloc(num_pages * sizeof(int));
    for (int i = 0; i < num_pages; i++){
        table[i].addr = -1;
        table[i].time = 0;
    }

    // Lê o arquivo de entrada
    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        if (debug_flag[0] == 'd') fprintf(outfile, "Endereço: %x ", addr);
        int s = find_s(page_size);
        pg.addr = addr >> s;
        pg.rw = rw;
        int page = pg.addr;
        char rw = pg.rw;
        int found = 0;

        // Verifica se a página já está na tabela
        for (int i = 0; i < num_pages; i++) {
            if (table[i].addr == page) {
                found++;
                hit++;
                if (debug_flag[0] == 'd') fprintf(outfile, "Hit\n");
                second_chance[i] = 1;
                if (rw == 'W'){
                    table[i].dirty = 1;
                }
                break;
            }
            if(table[i].addr == -1){
                table[i].addr = page;
                table[i].rw = rw;
                table[i].dirty = (rw == 'W') ? 1 : 0;
                miss++;
                if (debug_flag[0] == 'd') fprintf(outfile, "Miss\n");
                found++;
                break;
            }
        }
        if (found) continue;
        miss++;
        if (debug_flag[0] == 'd') fprintf(outfile, "Miss\n");

        // Encontra uma página para substituir usando o algoritmo Second-Chance
        int choice = -1;
        for (int i = 0; i < num_pages; i++) {
            int idx = (second_chance_idx + i) % num_pages;
            if (second_chance[idx] == 1) {
                second_chance[idx] = 0;
                continue;
            } else {
                choice = idx;
                break;
            }
        }

        if (choice == -1) choice = second_chance_idx;
        second_chance_idx = (choice + 1) % num_pages;

        // Se a página substituída foi escrita, incrementa o número de escritas
        if (table[choice].dirty == 1){
            written++;
        }

        // Substitui a página na tabela
        table[choice].addr = page;
        table[choice].rw = rw;
        second_chance[choice] = 0;
        table[choice].dirty = (rw == 'W') ? 1 : 0;

        continue;
    }
    print_result();
    free_table(table);
}