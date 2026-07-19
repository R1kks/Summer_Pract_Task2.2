/*
Реализация логики вывода содержимого файлов в шестнадцатиричном виде.
Дозоров Владимир Евгеньевич
Группа: МК - 101
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <stdint.h>
#include <io.h>
#else
#include <dirent.h>
#endif
#include "../include/hexdump.h"

static void byte_to_hex(unsigned char byte, char* out) {
    const char* hex_chars = "0123456789ABCDEF";
    out[0] = hex_chars[byte >> 4];
    out[1] = hex_chars[byte & 0x0F];
}

static int is_printable(unsigned char c) {
    return (c >= 32 && c <= 126);
}

/* Возвращает байт номер k (0-based) внутри кусочка chunk_idx, читая его из
   buffer/read_bytes. Байты за пределами реально прочитанных данных
   считаются нулевыми - так неполный последний кусочек дополняется
   нулями до полного размера (см. пример "-g 3 -l 8": последний неполный
   кусочек из 2 байт выводится как "003020", т.е. недостающий старший
   байт кусочка - нулевой). */
static unsigned char chunk_byte(const unsigned char* buffer, size_t read_bytes, int chunk_size, int chunk_idx, int k) {
    long pos = (long)chunk_idx * chunk_size + k;
    return ((size_t)pos < read_bytes) ? buffer[pos] : 0;
}

/* Печатает кусочок в шестнадцатиричном виде: байты кусочка трактуются
   как байты числа в порядке little-endian (первый прочитанный байт -
   младший), поэтому печатаются в ОБРАТНОМ порядке (последний байт
   кусочка - первым). Именно так получается "FBFA13" из байт 13,FA,FB. */
static void print_chunk_hex(const unsigned char* buffer, size_t read_bytes, int chunk_size, int chunk_idx) {
    int k;
    for (k = chunk_size - 1; k >= 0; k--) {
        char hex[2];
        byte_to_hex(chunk_byte(buffer, read_bytes, chunk_size, chunk_idx, k), hex);
        printf("%c%c", hex[0], hex[1]);
    }
}

/* Печатает кусочок в символьном виде: байты в исходном (не развёрнутом)
   порядке, как обычный текст - непечатаемые заменяются на '.'. */
static void print_chunk_chars(const unsigned char* buffer, size_t read_bytes, int chunk_size, int chunk_idx) {
    int k;
    for (k = 0; k < chunk_size; k++) {
        unsigned char b = chunk_byte(buffer, read_bytes, chunk_size, chunk_idx, k);
        putchar(is_printable(b) ? (char)b : '.');
    }
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
            else if (*p == 'n') printf("%08lX", (unsigned long)offset);
            else {
                char* endptr;
                int chunk_idx = (int)strtol(p, &endptr, 10);
                p = endptr;
                if (*p == 'x') {
                    print_chunk_hex(buffer, read_bytes, cfg->chunk_size, chunk_idx);
                } else if (*p == 'c') {
                    print_chunk_chars(buffer, read_bytes, cfg->chunk_size, chunk_idx);
                }
            }
        } else {
            putchar(*p);
        }
        p++;
    }
}

/* Печатает одну "строку" дампа (без пользовательской -f) для read_bytes
   байт буфера, начинающихся со смещения current_offset:
   - если размер кусочка 1 (-g 1), справа дополнительно выводится символьное
     представление, а сама шестнадцатиричная часть дополняется пробелами
     до полной ширины (chunks_per_line кусочков), чтобы колонка "|"
     была выровнена на всех строках;
   - иначе (кусочек больше байта) символьное представление не выводится,
     кусочки выводятся через пробел без дополнительного выравнивания. */
static void print_default_line(const Config* cfg, long current_offset, const unsigned char* buffer, size_t read_bytes) {
    long num_chunks = ((long)read_bytes + cfg->chunk_size - 1) / cfg->chunk_size;
    long c;

    printf("%08lX  ", (unsigned long)current_offset);

    if (cfg->chunk_size == 1) {
        for (c = 0; c < cfg->chunks_per_line; c++) {
            if (c < num_chunks) {
                print_chunk_hex(buffer, read_bytes, 1, (int)c);
                printf(" ");
            } else {
                printf("   ");
            }
        }
        printf("| ");
        {
            size_t i;
            for (i = 0; i < read_bytes; i++) {
                putchar(is_printable(buffer[i]) ? (char)buffer[i] : '.');
            }
        }
    } else {
        for (c = 0; c < num_chunks; c++) {
            if (c > 0) printf(" ");
            print_chunk_hex(buffer, read_bytes, cfg->chunk_size, (int)c);
        }
    }
    printf("\n");
}

int process_file(const char* filepath, const Config* cfg) {
    FILE* f = fopen(filepath, "rb");
    if (!f) { perror(filepath); return -1; }
    if (cfg->offset > 0) fseek(f, cfg->offset, SEEK_SET);

    long bytes_per_line = (long)cfg->chunk_size * cfg->chunks_per_line;
    unsigned char* buffer = (unsigned char*)malloc(bytes_per_line);
    long current_offset = cfg->offset;
    long remaining = cfg->length; /* -1 значит "без ограничения, до конца файла" */
    int line_idx = 0;

    while (1) {
        long want = bytes_per_line;
        if (remaining >= 0) {
            if (remaining == 0) break;
            if (want > remaining) want = remaining;
        }

        size_t read_bytes = fread(buffer, 1, (size_t)want, f);
        if (read_bytes == 0) break;

        if (cfg->format_str != NULL) {
            process_format_string(cfg, line_idx, current_offset, buffer, read_bytes);
        } else {
            print_default_line(cfg, current_offset, buffer, read_bytes);
        }

        current_offset += read_bytes;
        if (remaining >= 0) remaining -= (long)read_bytes;
        line_idx++;
        if (read_bytes < (size_t)want) break;
    }

    free(buffer);
    fclose(f);
    return 0;
}

int process_directory(const char* dirpath, const Config* cfg) {
#ifdef _WIN32
    char search_path[1024];
    struct _finddata_t find_data;
    intptr_t h_find;

    snprintf(search_path, sizeof(search_path), "%s/*", dirpath);
    h_find = _findfirst(search_path, &find_data);
    if (h_find == -1) {
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
#else
    DIR* dir = opendir(dirpath);
    struct dirent* entry;

    if (dir == NULL) {
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char filepath[1024];
            snprintf(filepath, sizeof(filepath), "%s/%s", dirpath, entry->d_name);
            if (cfg->format_str == NULL) printf("========== %s ==========\n", entry->d_name);
            process_file(filepath, cfg);
        }
    }
    closedir(dir);
    return 0;
#endif
}