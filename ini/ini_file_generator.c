//
// Created by Carbs0126 on 2023/7/6.
//

#include <string.h>
#include <stdio.h>
#include "ini_file_generator.h"

FILE *generate_file_from_ini_object(ini_object *ini_obj, char *file_path, int *error_code) {
    if (ini_obj == NULL || file_path == NULL) {
        if (error_code != NULL) {
            *error_code = 300;
        }
        return NULL;
    }
    linked_list *string_lines = generate_string_lines_for_ini_object(ini_obj);
    // 写入文件
    if (string_lines == NULL) {
        if (error_code != NULL) {
            *error_code = 301;
        }
        return NULL;
    }
    return write_file(string_lines, file_path, error_code);
}

FILE *write_file(linked_list *string_lines, const char *file_path, int *error_code) {

    if (string_lines == NULL || file_path == NULL) {
        if (error_code != NULL) {
            *error_code = 400;
        }
        return NULL;
    }

    FILE *file = fopen(file_path, "w");
    if (file == NULL) {
        // 无法打开文件
        printf("Cannot open file.\n");
        if (error_code != NULL) {
            *error_code = 400;
        }
        return NULL;
    }

    int size = string_lines->size;
    int index = 0;
    linked_list_node *cur_node = string_lines->head;
    while (index < size) {
        char *line = (char *) cur_node->value;
        fprintf(file, line);
        printf("output line =======>%s<============\n", line);
        fprintf(file, "\n");
        index++;
        cur_node = cur_node->next;
    }

    return file;
}