/*
 Реализация парсера аргументов командной строки с использованием getopt.
Дозоров Владимир Евгеньевич
Группа: МК - 101
 */

#include <stdio.h>
#include <stdlib.h>
#include "../include/getopt.h"
#include "../include/args_parser.h"

#define MAX_CHUNK_SIZE 4096
#define MAX_CHUNKS_PER_LINE 4096
#define MAX_BYTES_PER_LINE (16 * 1024 * 1024)

/*Инициализация структуры хранения*/
void init_config(Config* cfg) {
    cfg->input_file = NULL;
    cfg->directory = NULL;
    cfg->format_str = NULL;
    cfg->offset = 0;
    cfg->length = -1;
    cfg->chunk_size = 1;
    cfg->chunks_per_line = 16;
}
/*инициализация структуры хранения*/

// заполнения структуры
int parse_args(int argc, char** argv, Config* cfg) {
    int opt;
    const char* optstring = "i:o:l:g:n:d:f:";
    
    optind = 1;

    while ((opt = getopt(argc, argv, optstring)) != -1) {
        switch (opt) {
            case 'i':
                cfg->input_file = optarg;
                break;
            case 'o':
                cfg->offset = strtol(optarg, NULL, 10);
                break;
            case 'l':
                cfg->length = strtol(optarg, NULL, 10);
                break;
            case 'g':
                cfg->chunk_size = strtol(optarg, NULL, 10);
                break;
            case 'n':
                cfg->chunks_per_line = strtol(optarg, NULL, 10);
                break;
            case 'd':
                cfg->directory = optarg;
                break;
            case 'f':
                cfg->format_str = optarg;
                break;
            case '?':
            default:
                fprintf(stderr, "Usage: %s -i <file> | -d <dir> [-o offset] [-l size] [-g chunk] [-n count]\n", argv[0]);
                return -1;
        }
    }

    if (cfg->input_file == NULL && cfg->directory == NULL) {
        fprintf(stderr, "Error: You must specify either -i (input file) or -d (directory).\n");
        return -1;
    }

    if (cfg->input_file != NULL && cfg->directory != NULL) {
        fprintf(stderr, "Error: Options -i and -d cannot be used together.\n");
        return -1;
    }

    if (cfg->chunk_size <= 0 || cfg->chunks_per_line <= 0) {
        fprintf(stderr, "Error: Chunk size (-g) and chunks per line (-n) must be greater than 0.\n");
        return -1;
    }

    if (cfg->chunk_size > MAX_CHUNK_SIZE) {
        fprintf(stderr, "Error: Chunk size (-g) is too large (max %d).\n", MAX_CHUNK_SIZE);
        return -1;
    }
    if (cfg->chunks_per_line > MAX_CHUNKS_PER_LINE) {
        fprintf(stderr, "Error: Chunks per line (-n) is too large (max %d).\n", MAX_CHUNKS_PER_LINE);
        return -1;
    }

    if (cfg->length < -1) {
        fprintf(stderr, "Error: Length (-l) cannot be negative.\n");
        return -1;
    }

    return 0;
}