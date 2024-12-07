#include <stdlib.h>
#include <stdio.h>

int main (int argc, char* argv[]){
    unsigned addr;
    char rw;
    FILE *file;
    
    file = fopen(argv[1], "a+");
    fscanf(file,"%x %c",&addr,&rw);

    /*unsigned s, page_size, tmp;
    /* Derivar o valor de s: 
    tmp = page_size;
    s = 0;
    while (tmp>1) {
    tmp = tmp>>1;
    s++;
    }*/

    fclose(file);

    return (0);
}