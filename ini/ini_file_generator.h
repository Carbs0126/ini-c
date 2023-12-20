//
// Created by Carbs0126 on 2023/7/6.
//

#ifndef INI_C_INI_FILE_GENERATOR_H
#define INI_C_INI_FILE_GENERATOR_H

#include "ini_info.h"

FILE *generate_file_from_ini_object(ini_object *ini_obj, char *file_path, int *error_code);

FILE *write_file(linked_list *string_lines, const char *file_path, int *error_code);

#endif //INI_C_INI_FILE_GENERATOR_H
