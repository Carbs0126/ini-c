//
// Created by Carbs0126 on 2023/6/25.
//
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ini_utils.h"

char *int_to_string(int number) {
    char *str = (char *) malloc(sizeof(char) * 32);
    sprintf(str, "%d", number);
    return str;
}

char *pointer_address_to_string(void *pointer) {
    static int pointer_byte_size = 0;// 64位，pointer_byte_size == 8
    if (pointer_byte_size == 0) {
        pointer_byte_size = sizeof(void *);
    }
    // 64 位 地址： 0x60000343c000
    // 0x 占两个 char，4个bit输出一个16进制的数字，因此数字部分长度为 pointer_byte_size * 8 / 4 个 char
    char *str = (char *) malloc(sizeof(char) * (2 + pointer_byte_size * 2));
    sprintf(str, "%p", pointer);
    return str;
}

char *sub_string_with_length(const char *str, int start, int length) {
    char *sub_str = (char *) malloc(sizeof(char) * (length + 1));
    strncpy(sub_str, str + start, length);
    sub_str[length] = '\0';
    return sub_str;
}

char *sub_string_to_end(const char *str, int start) {
    int length = strlen(str) - start;
    return sub_string_with_length(str, start, length);
}

char *sub_string_with_index(const char* str, int start, int end) {
    int length = end - start;
    return sub_string_with_length(str, start, length);
}

char *trim(char *str) {
    int start = 0;
    int end = strlen(str) - 1;

    while (start <= end && isspace(str[start])) {
        start++;
    }

    while (start <= end && isspace(str[end])) {
        end--;
    }
    char *trimmed_str = (char *) malloc(sizeof(char) * (end - start + 2));
    memcpy(trimmed_str, str + start, end - start + 1);
    trimmed_str[end - start + 1] = '\0';
    return trimmed_str;
}

bool start_with(char *str, char* start_str) {
    if (str == NULL || start_str == NULL) {
        return false;
    }
    int len_str = strlen(str);
    int len_start_str = strlen(start_str);
    if (len_str < len_start_str) {
        return false;
    }
    bool ret = false;
    for (int i = 0; i < len_start_str; i++) {
        if (str[i] == start_str[i]) {
            ret = true;
        } else {
            ret = false;
            break;
        }
    }
    return ret;
}

int index_of(char *str, char c) {
    if (str == NULL) {
        return -1;
    }
    if (c < 0) {
        return -1;
    }
    for (int i = 0; i < strlen(str); i++) {
        if (c == str[i]) {
            return i;
        }
    }
    return -1;
}

int index_of_sub_str(char *str, char *sub_str) {
    if (str == NULL || sub_str == NULL) {
        return -1;
    }
    char *result = strstr(str, sub_str);
    if (result != NULL) {
        int position = result - str;
        return position;
    }
    return -1;
}

union X {
    struct Y {
        char a;
    };
    struct Y y;
    char b;
};

void test() {
    union X x;
    x.b = 10;
    x.y.a = 20;
}