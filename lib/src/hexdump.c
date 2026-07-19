/*
Реализация логики вывода содержимого файлов в шестнадцатиричном виде.
Дозоров Владимир Евгеньевич
Группа: МК - 101
*/

#include <stdio.h>
#include <io.h>
#include "../include/hexdump.h"

static void byte_to_hex(unsigned char byte, char* out) {
    const char* hex_chars = "0123456789ABCDEF";
    out[0] = hex_chars[byte >> 4];
    out[1] = hex_chars[byte & 0x0F];
}

static int is_printable(unsigned char c) {
    return (c >= 32 && c <= 126);
}

static void process_format_string(const Config* cfg, int line_idx, long offset, unsigned char* buffer, size_t read_bytes) {
    const char* p = cfg->format_str;
    while (*p) {
        if (*p == '\\') {
            p++;
            if (*p == 'n') printf("\n");
            else if (*p == 'r') printf("\r");
            else if (*p == 't') printf("\t");
            else if (*p == '\\') printf("\\");
        } else if (*p == '%') {
            p++;
            if (*p == 'i') printf("%d", line_idx);
            else if (*p == 'n') printf("%08lx", (unsigned long)offset);
            else {
                char* endptr;
                int chunk_idx = (int)strtol(p, &endptr, 10);
                p = endptr;
                if (*p == 'x') {
                    int start = chunk_idx * cfg->chunk_size;
                    for (int j = 0; j < cfg->chunk_size && (start + j) < (int)read_bytes; j++) {
                        char hex[2];
                        byte_to_hex(buffer[start + j], hex);
                        printf("%c%c", hex[0], hex[1]);
                    }
                } else if (*p == 'c') {
                    int start = chunk_idx * cfg->chunk_size;
                    for (int j = 0; j < cfg->chunk_size && (start + j) < (int)read_bytes; j++) {
                        printf("%c", is_printable(buffer[start + j]) ? buffer[start + j] : '.');
                    }
                }
            }
        } else {
            putchar(*p);
        }
        p++;
    }
}

int process_file(const char* filepath, const Config* cfg) {
    FILE* f = fopen(filepath, "rb");
    if (!f) { perror(filepath); return -1; }
    if (cfg->offset > 0) fseek(f, cfg->offset, SEEK_SET);

    long bytes_per_line = (long)cfg->chunk_size * cfg->chunks_per_line;
    unsigned char* buffer = (unsigned char*)malloc(bytes_per_line);
    long current_offset = cfg->offset;
    int line_idx = 0;

    while (1) {
        size_t read_bytes = fread(buffer, 1, bytes_per_line, f);
        if (read_bytes == 0) break;

        if (cfg->format_str != NULL) {
            process_format_string(cfg, line_idx, current_offset, buffer, read_bytes);
        } else {
            printf("%08lx  ", (unsigned long)current_offset);
            for (size_t i = 0; i < read_bytes; i++) {
                char hex[2]; byte_to_hex(buffer[i], hex);
                printf("%c%c ", hex[0], hex[1]);
            }
            printf("\n");
        }

        current_offset += read_bytes;
        line_idx++;
        if (read_bytes < (size_t)bytes_per_line) break;
    }

    free(buffer);
    fclose(f);
    return 0;
}

int process_directory(const char* dirpath, const Config* cfg) {
    char search_path[1024];
    snprintf(search_path, sizeof(search_path), "%s/*", dirpath);
    struct _finddata_t find_data;
    intptr_t h_find = _findfirst(search_path, &find_data);
    if (h_find == -1){ 
        return -1;
    }
    do {
        if (strcmp(find_data.name, ".") != 0 && strcmp(find_data.name, "..") != 0) {
            char filepath[1024];
            snprintf(filepath, sizeof(filepath), "%s/%s", dirpath, find_data.name);
            if (cfg->format_str == NULL) printf("========== %s ==========\n", find_data.name);
            process_file(filepath, cfg);
        }
    } while (_findnext(h_find, &find_data) == 0);
    _findclose(h_find);
    return 0;
}