#include <stdlib.h>
#include <stdio.h>

struct mem_address{
    unsigned addr;
    char rw; // flag leitura ou escrita
    char used;
};

int main (int argc, char* argv[]){
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

    // inicialização da tabela de páginas simples
    struct mem_address table[num_pages];
    for (int i = 0; i < num_pages; ++i) {
        table[i].used = 0;
    }

    file = fopen(file_name, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // leitura do endereço e operação
    while (fscanf(file, "%x %c", &addr, &rw) == 2) {
        unsigned s, tmp;
        // Derivar o valor de s: 
        tmp = page_size;
        s = 0;
        while (tmp>1) {
            tmp = tmp>>1;
            s++;
        }

        int page = addr >> s;

        if (!table[page].used){
            struct mem_address new_addr;
            new_addr.addr = addr;
            new_addr.rw = rw;
            new_addr.used = 1;
            table[page] = new_addr;
            page_faults++;
        }

    }


    // Close the file
    fclose(file);

    return (0);
}