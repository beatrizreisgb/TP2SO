#include "../include/hier2.h"
#include "../include/mem_address.h"

#define INF 1000000000

void hier2_fifo(int num_pages, int page_size, FILE* file, char* debug_flag, FILE* outfile){
    int s = find_s(page_size);
    int shift = 32 - 8 - s;

    int rows = power(2, 8);
    int columns = power(2, shift);

    // Cria a tabela de endereços de memória
    struct mem_address** table = create_2aTable(rows, columns);

    // Cria a tabela de tempos
    struct page_time* time_table = create_time_table(num_pages);
    
    // Lê o arquivo de entrada
    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        if (debug_flag[0] == 'd') fprintf(outfile, "Endereço: %x ", addr);

        // Calcula o endereço da página
        pg.addr = addr >> s;

        // Calcula a linha e coluna da tabela
        pg.first = find_row(pg.addr, shift);
        pg.second = find_column(pg.addr, shift);

        // Lê se é leitura ou escrita
        pg.rw = rw;

        int page = pg.addr;
        char rw = pg.rw;

        int min_time = INF;
        int choice_first = 0;
        int choice_second = 0;
        int time_idx = 0;

        // Se a página já está na tabela, atualiza o número de hits e "suja" a página se for escrita, para evidenciar que foi alterada
        if (table[pg.first][pg.second].addr == 1) {
            hit++;
            if (debug_flag[0] == 'd') fprintf(outfile, "Hit\n");
            if (rw == 'W'){
                table[pg.first][pg.second].dirty = 1;
            }
            continue;
        }

        // Caso contrário, incrementa o número de misses
        miss++;
        if (debug_flag[0] == 'd') fprintf(outfile, "Miss\n");

        // Se ainda há espaço na tabela, adiciona a página - sem passar pela política de substituição
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

        // Se chegou até aqui, é porque a página não está na tabela e não há mais espaço.
        // Portanto, é necessário escolher uma página para substituir.
        for (int i = 0; i < num_pages; i++) {
            if (time_table[i].time < min_time) {
                min_time = time_table[i].time;
                choice_first = time_table[i].first;
                choice_second = time_table[i].second;
                time_idx = i;
            }
        }

        // Tira a página escolhida da tabela
        table[choice_first][choice_second].addr = -1;
        table[choice_first][choice_second].rw = ' ';

        // Se a página substituída foi escrita, incrementa o número de escritas
        if (table[choice_first][choice_second].dirty == 1){
            written++;
        }

        // Coloca a nova página na tabela
        table[pg.first][pg.second].addr = 1;
        table[pg.first][pg.second].rw = rw;
        table[pg.first][pg.second].dirty = (rw == 'W') ? 1 : 0;

        // Atualiza a tabela de tempo com dados da nova página
        time_table[time_idx].time = global_time++;
        time_table[time_idx].first = pg.first;
        time_table[time_idx].second = pg.second;

        continue;
    }

    // Por fim, imprime o resultado e libera a memória alocada
    print_result();
    free_2aTable(table, rows);
    free_time_table(time_table);
}

void hier2_lru(int num_pages, int page_size, FILE* file, char* debug_flag, FILE* outfile){
    int s = find_s(page_size);
    int shift = 32 - 8 - s;

    int rows = power(2, 8);
    int columns = power(2, shift);

    // Cria a tabela de endereços de memória
    struct mem_address** table = create_2aTable(rows, columns);

    // Cria a tabela de tempos
    struct page_time* time_table = create_time_table(num_pages);
    
    // Lê o arquivo de entrada
    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        if (debug_flag[0] == 'd') fprintf(outfile, "Endereço: %x ", addr);

        // Calcula o endereço da página
        pg.addr = addr >> s;

        // Calcula a linha e coluna da tabela
        pg.first = find_row(pg.addr, shift);
        pg.second = find_column(pg.addr, shift);

        // Lê se é leitura ou escrita
        pg.rw = rw;

        int page = pg.addr;
        char rw = pg.rw;

        int min_time = INF;
        int choice_first = 0;
        int choice_second = 0;
        int time_idx = 0;

        // Se a página já está na tabela, atualiza o número de hits e "suja" a página se for escrita, para evidenciar que foi alterada
        if (table[pg.first][pg.second].addr == 1) {
            hit++;
            if (debug_flag[0] == 'd') fprintf(outfile, "Hit\n");
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

        // Caso contrário, incrementa o número de misses
        miss++;
        if (debug_flag[0] == 'd') fprintf(outfile, "Miss\n");

        // Se ainda há espaço na tabela, adiciona a página - sem passar pela política de substituição
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

        // Se chegou até aqui, é porque a página não está na tabela e não há mais espaço.
        // Portanto, é necessário escolher uma página para substituir.
        for (int i = 0; i < num_pages; i++) {
            if (time_table[i].time < min_time) {
                min_time = time_table[i].time;
                choice_first = time_table[i].first;
                choice_second = time_table[i].second;
                time_idx = i;
            }
        }

        // Tira a página escolhida da tabela
        table[choice_first][choice_second].addr = -1;
        table[choice_first][choice_second].rw = ' ';

        // Se a página substituída foi escrita, incrementa o número de escritas
        if (table[choice_first][choice_second].dirty == 1){
            written++;
        }

        // Coloca a nova página na tabela
        table[pg.first][pg.second].addr = 1;
        table[pg.first][pg.second].rw = rw;
        table[pg.first][pg.second].dirty = (rw == 'W') ? 1 : 0;

        // Atualiza a tabela de tempo com dados da nova página
        time_table[time_idx].time = global_time++;
        time_table[time_idx].first = pg.first;
        time_table[time_idx].second = pg.second;

        continue;
    }

    // Por fim, imprime o resultado e libera a memória alocada
    print_result();
    free_2aTable(table, rows);
    free_time_table(time_table);
}

void hier2_random(int num_pages, int page_size, FILE* file, char* debug_flag, FILE* outfile){
    int s = find_s(page_size);
    int shift = 32 - 8 - s;

    int rows = power(2, 8);
    int columns = power(2, shift);

    // Cria a tabela de endereços de memória
    struct mem_address** table = create_2aTable(rows, columns);

    // Cria a tabela de tempos
    struct page_time* time_table = create_time_table(num_pages);
    
    // Lê o arquivo de entrada
    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        if (debug_flag[0] == 'd') fprintf(outfile, "Endereço: %x ", addr);

        // Calcula o endereço da página
        pg.addr = addr >> s;

        // Calcula a linha e coluna da tabela
        pg.first = find_row(pg.addr, shift);
        pg.second = find_column(pg.addr, shift);

        // Lê se é leitura ou escrita
        pg.rw = rw;

        int page = pg.addr;
        char rw = pg.rw;

        int min_time = INF;
        int time_idx = 0;

        // Se a página já está na tabela, atualiza o número de hits e "suja" a página se for escrita, para evidenciar que foi alterada
        if (table[pg.first][pg.second].addr == 1) {
            hit++;
            if (debug_flag[0] == 'd') fprintf(outfile, "Hit\n");
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

        // Caso contrário, incrementa o número de misses
        miss++;
        if (debug_flag[0] == 'd') fprintf(outfile, "Miss\n");

        // Se ainda há espaço na tabela, adiciona a página - sem passar pela política de substituição
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

        // Se chegou até aqui, é porque a página não está na tabela e não há mais espaço.
        // Portanto, é necessário escolher uma página para substituir.
        time_idx = rand() % num_pages;

        int choice_first = time_table[time_idx].first;
        int choice_second = time_table[time_idx].second;

        // Se a página substituída foi escrita, incrementa o número de escritas
        if (table[choice_first][choice_second].dirty == 1){
            written++;
        }

        // Tira a página escolhida da tabela
        table[choice_first][choice_second].addr = -1;
        table[choice_first][choice_second].rw = ' ';

        // Coloca a nova página na tabela
        table[pg.first][pg.second].addr = 1;
        table[pg.first][pg.second].rw = rw;
        table[pg.first][pg.second].dirty = (rw == 'W') ? 1 : 0;

        // Atualiza a tabela de tempo com dados da nova página
        time_table[time_idx].time = global_time++;
        time_table[time_idx].first = pg.first;
        time_table[time_idx].second = pg.second;

        continue;
    }

    // Por fim, imprime o resultado e libera a memória alocada
    print_result();
    free_2aTable(table, rows);
    free_time_table(time_table);
}

void hier2_2a(int num_pages, int page_size, FILE* file, char* debug_flag, FILE* outfile){
    int s = find_s(page_size);
    int shift = 32 - 8 - s;

    int rows = power(2, 8);
    int columns = power(2, shift);

    // Cria a tabela de endereços de memória
    struct mem_address** table = create_2aTable(rows, columns);

    // Cria a tabela de tempos
    struct page_time* time_table = create_time_table(num_pages);
    
    // Lê o arquivo de entrada
    while(fscanf(file, "%x %c", &addr, &rw) == 2){
        struct mem_address pg;
        if (debug_flag[0] == 'd') fprintf(outfile, "Endereço: %x ", addr);

        // Calcula o endereço da página
        pg.addr = addr >> s;

        // Calcula a linha e coluna da tabela
        pg.first = find_row(pg.addr, shift);
        pg.second = find_column(pg.addr, shift);

        // Lê se é leitura ou escrita
        pg.rw = rw;

        int page = pg.addr;
        char rw = pg.rw;

        int min_time = INF;
        int fifo_first = 0;
        int fifo_second = 0;
        int time_idx = 0;

        // Se a página já está na tabela, atualiza o número de hits e "suja" a página se for escrita, para evidenciar que foi alterada
        if (table[pg.first][pg.second].addr == 1) {
            hit++;
            if (debug_flag[0] == 'd') fprintf(outfile, "Hit\n");
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

        // Caso contrário, incrementa o número de misses
        miss++;
        if (debug_flag[0] == 'd') fprintf(outfile, "Miss\n");

        // Se ainda há espaço na tabela, adiciona a página - sem passar pela política de substituição
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

        // Se chegou até aqui, é porque a página não está na tabela e não há mais espaço.
        // Portanto, é necessário escolher uma página para substituir.
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

        // Tira a página escolhida da tabela
        table[first_idx][second_idx].addr = -1;
        table[first_idx][second_idx].rw = ' ';

        // Se a página substituída foi escrita, incrementa o número de escritas
        if (table[first_idx][second_idx].dirty == 1){
            written++;
        }

        // Coloca a nova página na tabela
        table[pg.first][pg.second].addr = 1;
        table[pg.first][pg.second].rw = rw;
        table[pg.first][pg.second].dirty = (rw == 'W') ? 1 : 0;

        // Atualiza a tabela de tempo com dados da nova página
        time_table[choice].time = 0;
        time_table[choice].first = pg.first;
        time_table[choice].second = pg.second;

        continue;
    }

    // Por fim, imprime o resultado e libera a memória alocada
    print_result();
    free_2aTable(table, rows);
    free_time_table(time_table);
}