/*
Модульное тестирование функциональности библиотеки
Дозоров Владимир Евгеньевич
Группа: МК - 101
*/

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "../lib/include/args_parser.h"
#include "../lib/include/hexdump.h"

void test_args_no_required_params() {
    Config cfg;
    init_config(&cfg);
    char* argv[] = {"hexdump"}; 
    assert(parse_args(1, argv, &cfg) == -1);
}

int main() {
    printf("--- Starting Integration Tests ---\n");
    RUN_TEST(test_args_no_required_params);
    return 0;
}