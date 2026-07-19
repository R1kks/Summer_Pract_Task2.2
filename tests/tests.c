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

#define RUN_TEST(test_func) \
    do { \
        printf("Running " #test_func "..."); \
        test_func(); \
        printf(" OK\n"); \
    } while(0)

void test_args_no_required_params() {
    Config cfg;
    init_config(&cfg);
    char* argv[] = {"hexdump"}; 
    assert(parse_args(1, argv, &cfg) == -1);
}

void test_args_conflict_params() {
    Config cfg;
    init_config(&cfg);
    char* argv[] = {"hexdump", "-i", "file.bin", "-d", "dir"};
    assert(parse_args(5, argv, &cfg) == -1);
}

void test_args_invalid_chunk_size() {
    Config cfg;
    init_config(&cfg);
    char* argv[] = {"hexdump", "-i", "test.bin", "-g", "-5"};
    assert(parse_args(5, argv, &cfg) == -1);
}

int main() {
    printf("--- Starting Integration Tests ---\n");
    RUN_TEST(test_args_no_required_params);
    RUN_TEST(test_args_conflict_params);
    RUN_TEST(test_args_invalid_chunk_size);
    return 0;
}