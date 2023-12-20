//
// Created by Carbs0126 on 2023/6/30.
//

#ifndef INI_C_INI_FILE_PARSER_H
#define INI_C_INI_FILE_PARSER_H

#include "ini_info.h"

ini_object *parse_file_to_ini_object(const char *file_path, int *error_code);

void append_line_content_into_line_list(ini_element *element, linked_list *ini_lines);

bool check_same_line(ini_element *pre_element, ini_element *cur_element);

void check_semicolon(char *origin_str, int char_begin, linked_list *ini_lines, const char *file_location, int line_number, int *error_code);

linked_list *read_ini_file(const char *file_path, int *error_code);

#endif //INI_C_INI_FILE_PARSER_H
