#include "../include/dense.h"
#include "../include/mem_address.h"

#define INF 1000000000

void dense_fifo(int num_pages, int page_size, FILE* file, int dense_size, char* debug_flag, FILE* outfile){
    // Cria a tabela de endereços de memória
    struct mem_address* table = create_table(dense_size);
       
    // Inicializa a tabela com -1
    for (int i = 0; i < dense_size; i++){
        table[i].addr = -1;
        table[i].time = INF;
    }

    // Cria a tabela de tempos
    struct page_time* time_table = create_time_table(num_pages);

    int choice = 0;

    // Lê o arquivo de entrada
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

        // Se a página já está na tabela, atualiza o número de hits e "suja" a página se for escrita, para evidenciar que foi alterada
        if (table[page].addr == 1) {
            hit++;
            if (debug_flag[0] == 'd') fprintf(outfile, "Hit\n");
            global_time++;
            if (rw == 'W'){
                table[page].dirty = 1;
            }
            continue;
        }

        // Caso contrário, incrementa o número de misses
        miss++;
        if (debug_flag[0] == 'd') fprintf(outfile, "Miss\n");

        // Se ainda há espaço na tabela, adiciona a página - sem passar pela política de substituição
        if (pages_count < num_pages){
            table[page].addr = 1;
            table[page].rw = rw;
            table[page].dirty = (rw == 'W') ? 1 : 0;
            time_table[pages_count].time = global_time++;
            time_table[pages_count].page = page;
            pages_count++;
            continue;
        }

        // Se chegou até aqui, é porque a página não está na tabela e não há mais espaço.
        // Portanto, é necessário escolher uma página para substituir.
        for (int i = 0; i < num_pages; i++) {
            if (time_table[i].time < min_time) {
                min_time = time_table[i].time;
                choice = time_table[i].page;
                time_idx = i;
            }
        }

        // Se a página substituída foi escrita, incrementa o número de escritas
        if (table[choice].dirty == 1){
            written++;
        }

        // Tira a página escolhida da tabela
        table[choice].addr = -1;
        table[choice].rw = ' ';

        // Coloca a nova página na tabela
        table[page].addr = 1;
        table[page].rw = rw;
        table[page].dirty = (rw == 'W') ? 1 : 0;

        // Atualiza a tabela de tempo com dados da nova página
        time_table[time_idx].time = global_time++;
        time_table[time_idx].page = page;

        continue;
    }

    // Por fim, imprime o resultado e libera a memória alocada
    print_result();
    free_table(table);
    free_time_table(time_table);
}

void dense_lru(int num_pages, int page_size, FILE* file, int dense_size, char* debug_flag, FILE* outfile){
    // Cria a tabela de endereços de memória
    struct mem_address* table = create_table(dense_size);
       
    // Inicializa a tabela com -1
    for (int i = 0; i < dense_size; i++){
        table[i].addr = -1;
        table[i].time = INF;
    }

    // Cria a tabela de tempos
    struct page_time* time_table = create_time_table(num_pages);

    int choice = 0;

    // Lê o arquivo de entrada
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

        // Se a página já está na tabela, atualiza o número de hits e "suja" a página se for escrita, para evidenciar que foi alterada
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
        
        // Caso contrário, incrementa o número de misses
        miss++;
        if (debug_flag[0] == 'd') fprintf(outfile, "Miss\n");

        // Se ainda há espaço na tabela, adiciona a página - sem passar pela política de substituição
        if (pages_count < num_pages){
            table[page].addr = 1;
            table[page].rw = rw;
            table[page].dirty = (rw == 'W') ? 1 : 0;
            time_table[pages_count].time = global_time++;
            time_table[pages_count].page = page;
            pages_count++;
            continue;
        }

        // Se chegou até aqui, é porque a página não está na tabela e não há mais espaço.
        // Portanto, é necessário escolher uma página para substituir.
        for (int i = 0; i < num_pages; i++) {
            if (time_table[i].time < min_time) {
                min_time = time_table[i].time;
                choice = time_table[i].page;
                time_idx = i;
            }
        }

        // Se a página substituída foi escrita, incrementa o número de escritas
        if (table[choice].dirty == 1){
            written++;
        }

        // Tira a página escolhida da tabela
        table[choice].addr = -1;
        table[choice].rw = ' ';

        // Coloca a nova página na tabela
        table[page].addr = 1;
        table[page].rw = rw;
        table[page].dirty = (rw == 'W') ? 1 : 0;

        // Atualiza a tabela de tempo com dados da nova página
        time_table[time_idx].time = global_time++;
        time_table[time_idx].page = page;

        continue;
    }

    // Por fim, imprime o resultado e libera a memória alocada
    print_result();
    free_table(table);
    free_time_table(time_table);
}

void dense_random(int num_pages, int page_size, FILE* file, int dense_size, char* debug_flag, FILE* outfile){
    // Cria a tabela de endereços de memória
    struct mem_address* table = create_table(dense_size);
       
    // Inicializa a tabela com -1
    for (int i = 0; i < dense_size; i++){
        table[i].addr = -1;
        table[i].time = INF;
    }
    
    // Cria a tabela de tempos
    struct page_time* time_table = create_time_table(num_pages);

    int s = find_s(page_size);

    // Lê o arquivo de entrada
    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        if (debug_flag[0] == 'd') fprintf(outfile, "Endereço: %x ", addr);
        pg.addr = addr >> s;
        pg.rw = rw;
        int page = pg.addr;
        char rw = pg.rw;
        int rand_idx;

        // Se a página já está na tabela, atualiza o número de hits e "suja" a página se for escrita, para evidenciar que foi alterada
        if (table[page].addr == 1) {
            hit++;
            if (debug_flag[0] == 'd') fprintf(outfile, "Hit\n");
            if (rw == 'W'){
                table[page].dirty = 1;
            }
            continue;
        }
        
        // Caso contrário, incrementa o número de misses
        miss++;
        if (debug_flag[0] == 'd') fprintf(outfile, "Miss\n");

        // Se ainda há espaço na tabela, adiciona a página - sem passar pela política de substituição
        if (pages_count < num_pages){
            table[page].addr = 1;
            table[page].rw = rw;
            table[page].dirty = (rw == 'W') ? 1 : 0;
            time_table[pages_count].time = global_time++;
            time_table[pages_count].page = page;
            pages_count++;
            continue;
        }

        // Se chegou até aqui, é porque a página não está na tabela e não há mais espaço.
        // Portanto, é necessário escolher uma página para substituir.
        rand_idx = rand() % num_pages;

        int choice = time_table[rand_idx].page;
        
        // Se a página substituída foi escrita, incrementa o número de escritas
        if (table[choice].dirty == 1){
            written++;
        }

        // Tira a página escolhida da tabela
        table[choice].addr = -1;
        table[choice].rw = ' ';

        // Coloca a nova página na tabela
        table[page].addr = 1;
        table[page].rw = rw;
        table[page].dirty = (rw == 'W') ? 1 : 0;

        // Atualiza a tabela de tempo com dados da nova página
        time_table[rand_idx].time = global_time++;
        time_table[rand_idx].page = page;

        continue;
    }

    // Por fim, imprime o resultado e libera a memória alocada
    print_result();
    free_table(table);
    free_time_table(time_table);
}


void dense_2a(int num_pages, int page_size, FILE* file, int dense_size, char* debug_flag, FILE* outfile){
    // Cria a tabela de endereços de memória
    struct mem_address* table = create_table(dense_size);
       
    // Inicializa a tabela com -1
    for (int i = 0; i < dense_size; i++){
        table[i].addr = -1;
        table[i].time = INF;
    }

    // Cria a tabela de tempos
    struct page_time* time_table = create_time_table(num_pages);

    // Lê o arquivo de entrada
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

        // Se a página já está na tabela, atualiza o número de hits e "suja" a página se for escrita, para evidenciar que foi alterada
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
        
        // Caso contrário, incrementa o número de misses
        miss++;
        if (debug_flag[0] == 'd') fprintf(outfile, "Miss\n");

        // Se ainda há espaço na tabela, adiciona a página - sem passar pela política de substituição
        if (pages_count < num_pages){
            table[page].addr = 1;
            table[page].rw = rw;
            table[page].dirty = (rw == 'W') ? 1 : 0;
            time_table[pages_count].time = 0;
            time_table[pages_count].page = page;
            pages_count++;
            continue;
        }
        
        // Se chegou até aqui, é porque a página não está na tabela e não há mais espaço.
        // Portanto, é necessário escolher uma página para substituir.
        int choice = -1;

        for (int i = 0; i < num_pages; i++) {
            int idx = (second_chance_idx + i) % num_pages;
            if (time_table[idx].time == 1) {
                time_table[idx].time = 0;
                continue;
            } else {
                choice = idx;
                break;
            }
        }

        if (choice == -1) choice = second_chance_idx;
        second_chance_idx = (choice + 1) % num_pages;

        int table_idx = time_table[choice].page;

        // Se a página substituída foi escrita, incrementa o número de escritas
        if (table[table_idx].dirty == 1){
            written++;
        }

        // Tira a página escolhida da tabela
        table[table_idx].addr = -1;
        table[table_idx].rw = ' ';

        // Coloca a nova página na tabela
        table[page].addr = 1;
        table[page].rw = rw;
        table[page].dirty = (rw == 'W') ? 1 : 0;

        // Atualiza a tabela de tempo com dados da nova página
        time_table[choice].time = global_time++;
        time_table[choice].page = page;

        continue;
    }
    
    // Por fim, imprime o resultado e libera a memória alocada
    print_result();
    free_table(table);
    free_time_table(time_table);
}