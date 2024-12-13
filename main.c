#include <stdlib.h>
#include <stdio.h>

struct mem_address{
    unsigned addr;
};

int FindFirstEmpty(int* inverted_table, int num_pages){
    for (int i = 0; i < num_pages; i++) {
        if(inverted_table[i] == -1){
            return i;
        }
    }
    return -1;
}

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

    file = fopen(file_name, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    int inverted_table[num_pages];

    for (int i = 0; i < num_pages; i++) {
        inverted_table[i] = -1;
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

        int page = addr >> s; //end da pagina

        struct mem_address pagina;
        pagina.addr = page;

        int aux = FindFirstEmpty(inverted_table, num_pages);

        if(aux != -1){
            inverted_table[aux] = pagina; //apenas uma ideia, a tabela ia ter que ser de struct pra fazer essa atribuiçao
        }
        else{
            //chama algoritmo
        } 

    }

    fclose(file);

    return (0);
}