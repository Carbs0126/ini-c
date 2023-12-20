//
// Created by Carbs0126 on 2023/6/25.
//

#ifndef INI_C_INI_UTILS_H
#define INI_C_INI_UTILS_H

#include <string.h>
#include <ctype.h>
#include <stdbool.h>

char *int_to_string(int number);

char *pointer_address_to_string(void *pointer);

char *sub_string_with_length(const char* str, int start, int length);

char *sub_string_to_end(const char* str, int start);

char *sub_string_with_index(const char* str, int start, int end);

char *trim(char *str);

bool start_with(char *str, char* start_str);

int index_of(char *str, char c);

int index_of_sub_str(char *str, char *sub_str);

#endif //INI_C_INI_UTILS_H
