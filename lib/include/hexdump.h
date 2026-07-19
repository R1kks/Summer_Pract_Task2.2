/*
Заголовочный файл основной логики hexdump.
Дозоров Владимир Евгеньевич
Гркппа: МК - 101
 */

#pragma once
#include "args_parser.h"

int process_file(const char* filepath, const Config* cfg);

int process_directory(const char* dirpath, const Config* cfg);
