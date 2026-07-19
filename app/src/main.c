/*
Главный файл программы. Точка входа, вызов парсера аргументов и hexdump.

Дозоров Влпдимир Евгеньевич
Группа: МК - 101
 */

#include <stdio.h>
#include "args_parser.h"

int main(int argc, char** argv) {
    Config cfg;
    init_config(&cfg);

    if (parse_args(argc, argv, &cfg) != 0) {
        return 1;
    }
    
    return 0;
}