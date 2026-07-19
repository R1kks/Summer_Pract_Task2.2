/*
Заголовочный файл парсера аргументов командной строки.
Дозоров Владимир Евгеньевич
Группа: МК - 101
*/

#pragma once

typedef struct {
    const char* input_file;
    const char* directory;
    const char* format_str; 
    long offset;
    long length;
    int chunk_size;
    int chunks_per_line;
} Config;

void init_config(Config* cfg);

int parse_args(int argc, char** argv, Config* cfg);