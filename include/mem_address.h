#ifndef MEM_ADDRESS_H
#define MEM_ADDRESS_H

#include <stdio.h>
#include <stdlib.h>

// Definição de variáveis globais
extern int fifo_first;
extern int hit;
extern int miss;
extern int global_time;
extern int written;
extern int second_chance_idx;
extern int pages_count;
extern unsigned int addr;
extern char rw;

/**
 * @brief Estrutura de dados que representa um endereço de memória.
 *
 * @param addr Endereço completo.
 * @param first Primeiro trecho do endereço após divisão dos bits.
 * @param second Segundo trecho do endereço após divisão dos bits.
 * @param third Terceiro trecho do endereço após divisão dos bits.
 * @param dirty Bit que indica se a página foi ou não suja, isto é, se foi realizada sobre ela uma operação de escrita (do tipo 'W').
 * @param rw Caractere que indica a operação realizada (escrita, 'W', ou leitura, 'R').
 */
struct mem_address
{
    unsigned addr;
    unsigned first;
    unsigned second;
    unsigned third;
    unsigned time;
    unsigned dirty;
    char rw;
};

/**
 * @brief Estrutura de dados que representa uma página e o tempo em que ele foi acessado.
 *
 * @param page Endereço.
 * @param first Primeiro trecho do endereço após divisão dos bits.
 * @param second Segundo trecho do endereço após divisão dos bits.
 * @param third Terceiro trecho do endereço após divisão dos bits.
 * @param time Momento em que o endereço foi adicionado à memória ou em que é acessado (a 
 * depender o algoritmo escolhido)
 */
struct page_time
{
    unsigned page;
    unsigned first;
    unsigned second;
    unsigned third;
    unsigned time;
};

/**
 * @brief Cria uma tabela de endereços de memória.
 *
 * @param size Tamanho da tabela.
 * @return Ponteiro para a tabela criada.
 */
struct mem_address *create_table(int size);

/**
 * @brief Cria uma tabela de endereços de memória e o tempo em que foram acessados.
 *
 * @param size Tamanho da tabela.
 * @return Ponteiro para a tabela criada.
 */
struct page_time *create_time_table(int size);

/**
 * @brief Cria uma tabela de endereços de memória de duas dimensões.
 *
 * @param rows Número de linhas da tabela.
 * @param columns Número de colunas da tabela.
 * @return Ponteiro para a tabela criada.
 */
struct mem_address **create_2aTable(int rows, int columns);

/**
 * @brief Libera a memória alocada para a tabela de endereços de memória.
 * @param table Ponteiro para a tabela de endereços de memória.
 */
void free_table(struct mem_address *table);

/**
 * @brief Libera a memória alocada para a tabela de endereços de memória com o tempo em que foram acessados.
 * @param time_table Ponteiro para a tabela.
 */
void free_time_table(struct page_time *time_table);

/**
 * @brief Libera a memória alocada para a tabela de endereços de memória de duas dimensões.
 * @param table Ponteiro para a tabela de endereços de memória.
 * @param rows Número de linhas da tabela.
 */
void free_2aTable(struct mem_address **table, int rows);

/**
 * @brief Libera a memória alocada para a tabela de endereços de memória de três dimensões.
 * @param table Ponteiro para a tabela de endereços de memória.
 * @param rows Número de linhas da tabela.
 * @param columns Número de colunas da tabela.
 */
void free_3aTable(struct mem_address ***table, int rows, int columns);

/**
 * @brief Função que calcula a potência de um número.
 * @param base Base.
 * @param exp Expoente.
 */
int power(int base, int exp);

/**
 * @brief Função que encontra o valor de s (bits menos significativos a serem descartados do endereço).
 * @param page_size Tamanho da página.
 * @return O valor de s.
 */
unsigned find_s(int page_size);

/**
 * @brief Função que imprime o número de páginas lidas e de páginas encontradas após a execução do algoritmo.
 */
void print_result();

/**
 * @brief Função que encontra o primeiro trecho do endereço após a divisão dos bits.
 * @param num Número do endereço.
 * @param shift Quantidade de bits a serem deslocados.
 * @return O primeiro trecho do endereço.
 */
unsigned find_row(unsigned num, int shift);

/**
 * @brief Função que encontra o segundo trecho do endereço após a divisão dos bits.
 * @param num Número do endereço.
 * @param shift Quantidade de bits a serem deslocados.
 * @return O segundo trecho do endereço.
 */
unsigned find_column(unsigned num, int shift);

#endif // MEM_ADDRESS_H