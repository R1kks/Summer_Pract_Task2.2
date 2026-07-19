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