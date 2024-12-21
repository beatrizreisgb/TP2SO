#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Função que retorna o endereço da página
 * @param page_size Tamanho da página
 * 
 * @return int Endereço da página
 */
int get_address(int page_size);

/**
 * @brief Função que implementa o algoritmo FIFO para a tabela hierárquica de 2 níveis
 * @param num_pages Número de páginas
 * @param page_size Tamanho da página
 * @param file Arquivo de entrada
 * @param debug_flag Flag de debug
 * @param outfile Arquivo de saída
 */
void hier2_fifo(int num_pages, int page_size, FILE* file, char* debug_flag, FILE* outfile);

/**
 * @brief Função que implementa o algoritmo LRU para a tabela hierárquica de 2 níveis
 * @param num_pages Número de páginas
 * @param page_size Tamanho da página
 * @param file Arquivo de entrada
 * @param debug_flag Flag de debug
 * @param outfile Arquivo de saída
 */
void hier2_lru(int num_pages, int page_size, FILE* file, char* debug_flag, FILE* outfile);

/**
 * @brief Função que implementa o algoritmo RANDOM para a tabela hierárquica de 2 níveis
 * @param num_pages Número de páginas
 * @param page_size Tamanho da página
 * @param file Arquivo de entrada
 * @param debug_flag Flag de debug
 * @param outfile Arquivo de saída
 */
void hier2_random(int num_pages, int page_size, FILE* file, char* debug_flag, FILE* outfile);

/**
 * @brief Função que implementa o algoritmo 2a para a tabela hierárquica de 2 níveis
 * @param num_pages Número de páginas
 * @param page_size Tamanho da página
 * @param file Arquivo de entrada
 * @param debug_flag Flag de debug
 * @param outfile Arquivo de saída
 */
void hier2_2a(int num_pages, int page_size, FILE* file, char* debug_flag, FILE* outfile);
