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

void test_args_chunk_size_too_large() {
    Config cfg;
    init_config(&cfg);
    char* argv[] = {"hexdump", "-i", "test.bin", "-g", "9999"};
    assert(parse_args(5, argv, &cfg) == -1);
}

void test_args_negative_offset() {
    Config cfg;
    init_config(&cfg);
    char* argv[] = {"hexdump", "-i", "test.bin", "-o", "-10"};
    assert(parse_args(5, argv, &cfg) == -1); 
}

void test_file_not_found() {
    Config cfg;
    init_config(&cfg);
    assert(process_file("non_existent_file_12345.bin", &cfg) == -1);
}

void test_offset_exceeds_file_size() {
    FILE* f = fopen("small.bin", "wb");
    fprintf(f, "123");
    fclose(f);
    
    Config cfg;
    init_config(&cfg);
    cfg.offset = 1000;
    assert(process_file("small.bin", &cfg) == 0); 
    remove("small.bin");
}

void test_format_string_malformed() {
    Config cfg;
    init_config(&cfg);
    cfg.input_file = "test.bin";
    cfg.format_str = "%z";
    
    FILE* f = fopen("test.bin", "wb");
    fclose(f);
    
    assert(process_file("test.bin", &cfg) == 0);
    remove("test.bin");
}

void test_process_empty_directory() {
    system("mkdir empty_test_dir");
    Config cfg;
    init_config(&cfg);
    
    assert(process_directory("empty_test_dir", &cfg) == 0);
    system("rmdir empty_test_dir");
}

int main() {
    printf("--- Starting Integration Tests ---\n");
    RUN_TEST(test_args_no_required_params);
    RUN_TEST(test_args_conflict_params);
    RUN_TEST(test_args_invalid_chunk_size);
    RUN_TEST(test_args_chunk_size_too_large);
    RUN_TEST(test_args_negative_offset);
    RUN_TEST(test_file_not_found);
    RUN_TEST(test_offset_exceeds_file_size);
    RUN_TEST(test_format_string_malformed);
    RUN_TEST(test_process_empty_directory);
    return 0;
}