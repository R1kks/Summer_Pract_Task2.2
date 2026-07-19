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