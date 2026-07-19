/*
 Реализация парсера аргументов командной строки с использованием getopt.
Дозоров Владимир Евгеньевич
Группа: МК - 101
 */

 #include <stdio.h>
#include "../include/args_parser.h"

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