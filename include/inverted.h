#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Função que retorna o endereço da página
 * @param page_size Tamanho da página
 * 
 * @return int Endereço da página
 */
int get_address(int page_size);

/**
 * @brief Função que implementa o algoritmo FIFO para a tabela invertida
 * @param num_pages Número de páginas
 * @param page_size Tamanho da página
 * @param file Arquivo de entrada
 * @param debug_flag Flag de debug
 * @param outfile Arquivo de saída
 * 
 */
void inverted_fifo(int num_pages, int page_size, FILE* file, char* debug_flag, FILE* outfile);

/**
 * @brief Função que implementa o algoritmo LRU para a tabela invertida
 * @param num_pages Número de páginas
 * @param page_size Tamanho da página
 * @param file Arquivo de entrada
 * @param debug_flag Flag de debug
 * @param outfile Arquivo de saída
 * 
 */
void inverted_lru(int num_pages, int page_size, FILE* file, char* debug_flag, FILE* outfile);

/**
 * @brief Função que implementa o algoritmo RANDOM para a tabela invertida
 * @param num_pages Número de páginas
 * @param page_size Tamanho da página
 * @param file Arquivo de entrada
 * @param debug_flag Flag de debug
 * @param outfile Arquivo de saída
 * 
 */
void inverted_random(int num_pages, int page_size, FILE* file, char* debug_flag, FILE* outfile);

/**
 * @brief Função que implementa o algoritmo 2a para a tabela invertida
 * @param num_pages Número de páginas
 * @param page_size Tamanho da página
 * @param file Arquivo de entrada
 * @param debug_flag Flag de debug
 * @param outfile Arquivo de saída
 * 
 */
void inverted_2a(int num_pages, int page_size, FILE* file, char* debug_flag, FILE* outfile);