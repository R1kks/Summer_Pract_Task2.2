/*
Главный файл программы. Точка входа, вызов парсера аргументов и hexdump.

Дозоров Владимир Евгеньевич
Группа: МК - 101
 */

#include <stdio.h>
#include "args_parser.h"
#include "hexdump.h"

int main(int argc, char** argv) {
    Config cfg;
    int result;

    init_config(&cfg);

    if (parse_args(argc, argv, &cfg) != 0) {
        return 1;
    }

    if (cfg.input_file != NULL) {
        result = process_file(cfg.input_file, &cfg);
    } else {
        result = process_directory(cfg.directory, &cfg);
    }

    return (result == 0) ? 0 : 1;
}