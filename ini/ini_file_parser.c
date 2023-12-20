//
// Created by Carbs0126 on 2023/6/30.
//
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ini_file_parser.h"
#include "ini_utils.h"

ini_object *parse_file_to_ini_object(const char *file_path, int *error_code) {
    linked_list *ini_file_strings = read_ini_file(file_path, error_code);
    if (*error_code != 0) {
        return NULL;
    }
    linked_list *ini_file_elements = new_linked_list();
    int line_number = 0;
    const char *file_name = file_path;

    linked_list_node *current_node = ini_file_strings->head;
    int index = 0;
    int file_strings_size = ini_file_strings->size;
    while (index < file_strings_size) {
        char *curr_str = (char *) (current_node->value);
        char *origin_curr_str = curr_str == NULL ? "" : curr_str;
        char *trimmed_curr_str = trim(origin_curr_str);
        if (start_with(trimmed_curr_str, ";")) {
            // comment
            bool comment_allocated = false;
            if (current_node->value != NULL && current_node->allocated) {
                comment_allocated = true;
            }
            ini_element_comment *element_comment = new_ini_element_comment_p(
                    new_ini_position(file_name, line_number, 0, strlen(origin_curr_str)),
                    origin_curr_str, comment_allocated);
            ini_element *element = new_ini_element(LINE_COMMENT, element_comment);
            append_line_content_into_line_list(element, ini_file_elements);
        } else if (start_with(trimmed_curr_str, "[")) {
            // section header
            int right_square_brackets_position = index_of(trimmed_curr_str, ']');
            if (right_square_brackets_position < 2) {
                printf("Right square bracket's position should be greater than 1, now it is %d.\n",
                       right_square_brackets_position);
                *error_code = 100;
                free_linked_list_nodes(ini_file_strings);
                free(trimmed_curr_str);
                free_ini_file_elements(ini_file_elements);
                return NULL;
            }

            char *section_name = sub_string_with_index(trimmed_curr_str, 0, right_square_brackets_position + 1);
            if (index_of(section_name, ';') > -1) {
                printf("Section's name should not contain ';' symbol.\n");
                *error_code = 101;
                free(section_name);
                free_linked_list_nodes(ini_file_strings);
                free(trimmed_curr_str);
                free_ini_file_elements(ini_file_elements);
                return NULL;
            }
            int char_begin = index_of(origin_curr_str, '[');
            int char_end = index_of(origin_curr_str, ']');
            ini_element_section_header *section_header = new_ini_element_section_header_p(
                    new_ini_position(file_name, line_number, char_begin, char_end),
                    section_name, true);
            ini_element *element = new_ini_element(LINE_SECTION_HEADER, section_header);
            append_line_content_into_line_list(element, ini_file_elements);
            check_semicolon(origin_curr_str, char_end + 1, ini_file_elements, file_name, line_number, error_code);
            if (*error_code != 0) {
                free(section_name);
                free_linked_list_nodes(ini_file_strings);
                free(trimmed_curr_str);
                free_ini_file_elements(ini_file_elements);
                return NULL;
            }
        } else if (strlen(trimmed_curr_str) == 0) {
            ini_element_empty *element_empty = new_ini_element_empty_p(new_ini_position(file_name, line_number, 0, 0));
            ini_element *element = new_ini_element(LINE_EMPTY, element_empty);
            append_line_content_into_line_list(element, ini_file_elements);
        } else {
            // kv
            int index_of_equal_in_trimmed_str = index_of(trimmed_curr_str, '=');
            if (index_of_equal_in_trimmed_str < 1) {
                printf("Equal's position should be greater than 0\n");
                *error_code = 102;
                free_linked_list_nodes(ini_file_strings);
                free(trimmed_curr_str);
                free_ini_file_elements(ini_file_elements);
                return NULL;
            }
            int index_of_equal_in_origin_str = index_of(origin_curr_str, '=');
            char *key_name_origin = sub_string_with_index(trimmed_curr_str, 0, index_of_equal_in_trimmed_str);
            char *key_name_trimmed = trim(key_name_origin);
            free(key_name_origin);
            char *right_str_of_equal = sub_string_to_end(trimmed_curr_str, index_of_equal_in_trimmed_str + 1);
            int length = strlen(right_str_of_equal);
            if (length > 0) {
                // string builder
                linked_list *value_name_sb = new_linked_list();

                int stat = 0;
                int i = 0;
                for (; i < length; i++) {
                    char c = right_str_of_equal[i];
                    if (stat == 0) {
                        // 过滤前面的空格
                        if (c == ' ' || c == '\t') {
                            continue;
                        } else {
                            stat = 1;
                            append_char_to_linked_list(value_name_sb, c);
                        }
                    } else if (stat == 1) {
                        // 正在记录value
                        // value中允许有空格
                        if (c == ';') {
                            // 记录 value 结束
                            stat = 2;
                            break;
                        } else {
                            stat = 1;
                            append_char_to_linked_list(value_name_sb, c);
                        }
                    }
                }

                char *value_name = concat_strings_in_linked_list(value_name_sb);
                free_linked_list_nodes(value_name_sb);

                // 在字符串中寻找第一个子字符串
                int char_begin = index_of_sub_str(origin_curr_str, key_name_trimmed);
                int char_end = index_of_equal_in_origin_str + 1 + i;

                ini_element_kv_pair *element_kv_pair = new_ini_element_kv_pair_p(
                        new_ini_position(file_name, line_number, char_begin, char_end), key_name_trimmed, value_name, true, true);
                ini_element *element = new_ini_element(LINE_KV_PAIR, element_kv_pair);
                append_line_content_into_line_list(element, ini_file_elements);
                if (i != length) {
                    // 没有到结尾，检测是不是有分号
                    check_semicolon(origin_curr_str, index_of_equal_in_origin_str + 1 + i, ini_file_elements, file_name,
                                    line_number, error_code);
                    if (*error_code != 0) {
                        free(value_name);
                        free(right_str_of_equal);
                        free_linked_list_nodes(ini_file_strings);
                        free(trimmed_curr_str);
                        free_ini_file_elements(ini_file_elements);
                        return NULL;
                    }
                }
            }
            free(right_str_of_equal);
        }

        origin_curr_str = NULL;
        free(trimmed_curr_str);
        line_number++;
        index++;
        current_node = current_node->next;
    }
    // 最终解析为一个实体
    ini_object *ini_obj = new_ini_object();
    // 收集 section 的 comments 或者 kv 的 comments
    linked_list *comments_cache = new_linked_list();
    // 解析完当前的 section ，一次存入
    ini_section_object *current_section_object = NULL;
    // 解析当前的 kvPair
    ini_entry_object *current_entry_object = NULL;
    // 0 解析 section 阶段，还没有解析到 section
    // 1 已经解析出 sectionName 阶段，(刚刚解析完 sectionHeader ) 还没有解析到下一个 section
    int parse_state = 0;
    ini_element *pre_ini_element = NULL;
    ini_element *cur_ini_element = NULL;

    linked_list_node *cur_node = ini_file_elements->head;
    index = 0;
    int file_elements_size = ini_file_elements->size;
    while (index < file_elements_size) {
        ini_element *cur_element = (ini_element *) cur_node->value;
        if (cur_element == NULL) {
            cur_node = cur_node->next;
            index++;
            continue;
        }
        if (cur_element->type == LINE_EMPTY) {
            cur_node = cur_node->next;
            index++;
            continue;
        }
        cur_ini_element = cur_element;
        if (cur_element->type == LINE_COMMENT) {
            ini_element_comment *element_comment = (ini_element_comment *) (cur_element->pointer);
            if (parse_state == 0) {
                // 还没解析到 section
                append_obj_to_linked_list(comments_cache, element_comment, true);
            } else {
                if (pre_ini_element != NULL && pre_ini_element->type == LINE_SECTION_HEADER) {
                    if (check_same_line(pre_ini_element, cur_ini_element)) {
                        // 当前 comment 属于 section
                        append_obj_to_linked_list(comments_cache, element_comment, true);
                        if (current_section_object == NULL) {
                            current_section_object = new_ini_section_object();
                        }
                        add_comments_to_section_object(current_section_object, comments_cache);
                        clear_linked_list_by_only_cut_pointers(comments_cache);
                        // 当前 section 的所有 comment 已经结束
                    } else {
                        // 当前 comment 属于当前 section 的 kv 或者下一个 section 的 section
                        if (current_section_object == NULL) {
                            current_section_object = new_ini_section_object();
                        }
                        add_comments_to_section_object(current_section_object, comments_cache);
                        clear_linked_list_by_only_cut_pointers(comments_cache);
                        append_obj_to_linked_list(comments_cache, element_comment, true);
                    }
                } else if (pre_ini_element != NULL && pre_ini_element->type == LINE_COMMENT) {
                    // comment 累加
                    append_obj_to_linked_list(comments_cache, element_comment, true);
                } else if (pre_ini_element != NULL && pre_ini_element->type == LINE_KV_PAIR) {
                    if (check_same_line(pre_ini_element, cur_ini_element)) {
                        // 当前 comment 属于 kv
                        append_obj_to_linked_list(comments_cache, element_comment, true);
                        if (current_entry_object == NULL) {
                            // 不走这里
                            current_entry_object = new_ini_entry_object();
                        }
                        add_comments_to_entry_object(current_entry_object, comments_cache);
                        if (current_section_object == NULL) {
                            current_section_object = new_ini_section_object();
                        }
                        add_entry_object_to_section_object(current_section_object, current_entry_object, true);
                        current_entry_object = NULL;
                        clear_linked_list_by_only_cut_pointers(comments_cache);
                        // 当前 kv 收尾
                    } else {
                        // 当前 comment 属于当前 section 的下一个 kv 或者下一个 section 的 section
                        clear_linked_list_by_only_cut_pointers(comments_cache);
                        append_obj_to_linked_list(comments_cache, element_comment, true);
                    }
                }
            }
        } else if (cur_element->type == LINE_SECTION_HEADER) {
            ini_element_section_header *element_section_header = (ini_element_section_header *) (cur_element->pointer);

            if (parse_state == 0) {
                // 解析到第一个 section
                parse_state = 1;
                current_section_object = new_ini_section_object();
                current_section_object->section_header = element_section_header;
            } else {
                if (pre_ini_element != NULL && pre_ini_element->type == LINE_SECTION_HEADER) {
                    // 连着两个 section header
                    // 收尾上一个 section header
                    if (current_section_object != NULL) {
                        add_comments_to_section_object(current_section_object, comments_cache);
                        clear_linked_list_by_only_cut_pointers(comments_cache);
                        add_section_to_ini_object(ini_obj, current_section_object, true);
                    }
                    // 新建 section header
                    current_section_object = new_ini_section_object();
                    current_section_object->section_header = element_section_header;
                } else if (pre_ini_element != NULL && pre_ini_element->type == LINE_COMMENT) {
                    if (comments_cache->size == 0) {
                        // 说明上一个 comment 和其之前的元素是一行，需要收尾上一个 section
                        if (current_section_object != NULL) {
                            add_section_to_ini_object(ini_obj, current_section_object, true);
                        }
                        current_section_object = new_ini_section_object();
                        current_section_object->section_header = element_section_header;
                    } else {
                        current_section_object = new_ini_section_object();
                        current_section_object->section_header = element_section_header;
                        add_comments_to_section_object(current_section_object, comments_cache);
                        clear_linked_list_by_only_cut_pointers(comments_cache);
                    }
                } else if (pre_ini_element != NULL && pre_ini_element->type == LINE_KV_PAIR) {
                    // 说明上一个 section 结束了，需要收尾
                    if (current_section_object != NULL) {
                        if (current_entry_object != NULL) {
                            add_entry_object_to_section_object(current_section_object, current_entry_object, true);
                            current_entry_object = NULL;
                        }
                        add_section_to_ini_object(ini_obj, current_section_object, true);
                    }
                    current_section_object = new_ini_section_object();
                    current_section_object->section_header = element_section_header;
                }
            }
        } else if (cur_element->type == LINE_KV_PAIR) {
            ini_element_kv_pair *element_kv_pair = (ini_element_kv_pair *) (cur_element->pointer);
            if (parse_state == 0) {
                // 没有 section，就出现了 kv，说明格式出错
                printf("There should be a section header before key-value pairs\n");
                *error_code = 103;
                free_linked_list_nodes(ini_file_strings);
                free_ini_file_elements(ini_file_elements);
                return NULL;
            } else {
                if (pre_ini_element != NULL && pre_ini_element->type == LINE_SECTION_HEADER) {
                    current_entry_object = new_ini_entry_object();
                    current_entry_object->kv_pair = element_kv_pair;
                } else if (pre_ini_element != NULL && pre_ini_element->type == LINE_COMMENT) {
                    if (comments_cache->size == 0) {
                        // 说明上一行中，comment 是右边的注释，还包含左边的元素
                        // 当上一行的左侧是 section 时，不需要关心 section
                        // 当上一行的左侧是 kv 时，不需要关心当前 section 或者上一个 kv
                        current_entry_object = new_ini_entry_object();
                        current_entry_object->kv_pair = element_kv_pair;
                    } else {
                        current_entry_object = new_ini_entry_object();
                        current_entry_object->kv_pair = element_kv_pair;
                    }
                } else if (pre_ini_element != NULL && pre_ini_element->type == LINE_KV_PAIR) {
                    // 把前一个 kv 收尾到 section 中
                    if (current_entry_object != NULL) {
                        add_comments_to_entry_object(current_entry_object, comments_cache);
                        clear_linked_list_by_only_cut_pointers(comments_cache);
                        if (current_section_object != NULL) {
                            add_entry_object_to_section_object(current_section_object, current_entry_object, true);
                        }
                    }
                    current_entry_object = new_ini_entry_object();
                    current_entry_object->kv_pair = element_kv_pair;
                }
            }
        }
        pre_ini_element = cur_ini_element;
        cur_node = cur_node->next;
        index++;
    }
    // 最后一个元素
    if (current_entry_object != NULL) {
        add_comments_to_entry_object(current_entry_object, comments_cache);
        clear_linked_list_by_only_cut_pointers(comments_cache);
    }
    if (current_section_object != NULL) {
        add_comments_to_section_object(current_section_object, comments_cache);
        clear_linked_list_by_only_cut_pointers(comments_cache);
        if (current_entry_object != NULL) {
            add_entry_object_to_section_object(current_section_object, current_entry_object, true);
            current_entry_object = NULL;
        }
        add_section_to_ini_object(ini_obj, current_section_object, true);
    }
    free_linked_list_nodes_without_value(ini_file_strings);
    // ini_file_elements 的 linked_list_node 中保存的是 ini_element 类型元素，因此可以free掉
    free_linked_list_nodes(ini_file_elements);
    return ini_obj;
}

bool check_same_line(ini_element *pre_element, ini_element *cur_element) {
    if (pre_element != NULL && cur_element != NULL) {
        ini_position *pre_position = get_position(pre_element);
        ini_position *cur_position = get_position(cur_element);
        if (pre_position == NULL || cur_position == NULL) {
            return false;
        }
        return pre_position->line_number == cur_position->line_number;
    }
    return false;
}

void append_line_content_into_line_list(ini_element *element, linked_list *ini_lines) {
    if (ini_lines == NULL) {
        return;
    }
    append_obj_to_linked_list(ini_lines, element, true);
}

void check_semicolon(char *origin_str, int char_begin, linked_list *ini_lines, const char *file_location, int line_number, int *error_code) {
    char *remain_str = sub_string_to_end(origin_str, char_begin);
    char *trimmed_remain_str = trim(remain_str);
    if (strlen(trimmed_remain_str) > 0) {
        if (start_with(trimmed_remain_str, ";")) {
            ini_element_comment *element_comment = new_ini_element_comment_p(
                    new_ini_position(file_location, line_number,
                                     index_of(origin_str, ';'), strlen(origin_str)),
                    trimmed_remain_str, true);
            ini_element *element = new_ini_element(LINE_COMMENT, element_comment);
            append_line_content_into_line_list(element, ini_lines);
            free(remain_str);
            return;
        } else {
            if (error_code != NULL) {
                printf("Need ';' symbol, but find %c instead", trimmed_remain_str[0]);
                *error_code = 200;
            }
        }
    }
    free(remain_str);
    free(trimmed_remain_str);
}

linked_list *read_ini_file(const char *file_path, int *error_code) {

    FILE *file = fopen(file_path, "r");  // 打开文件，以只读方式
    if (file == NULL) {
        *error_code = 1;
        printf("Cannot open file.\n");
        return NULL;
    }
    linked_list *ini_file_strings = new_linked_list();
    linked_list *one_line_buff = new_linked_list();

    // 倒数第二个字符是回车，倒数第一个字符是 '\0'
    int line_size = sizeof(char) * 102;
    char *line_content = (char *) malloc(line_size);

    while (fgets(line_content, line_size, file) != NULL) {
        // 在这里对每行字符串进行处理
        int current_line_length = strlen(line_content);

        char *current_str = (char *) malloc(current_line_length + 1);
        strcpy(current_str, line_content);
        if (line_content[current_line_length - 1] == '\n') {
            // 结尾是换行符，说明一行已经结束，放入buff中，并取出buff，清空buff
            if (one_line_buff->size == 0) {
                // 直接赋值给 ini_file_str
                current_str[current_line_length - 1] = '\0';
                append_string_to_linked_list(ini_file_strings, current_str, true);
                printf("current_str ----->%s<------\n", current_str);
            } else {
                append_string_to_linked_list(one_line_buff, current_str, true);
                char *one_line_str = concat_strings_in_linked_list(one_line_buff);
                if (one_line_str[strlen(one_line_str) - 1] == '\n') {
                    one_line_str[strlen(one_line_str) - 1] = '\0';
                }
                printf("one_line_str ----->%s<------\n", one_line_str);
                append_string_to_linked_list(ini_file_strings, one_line_str, true);
                free_linked_list_nodes(one_line_buff);
            }
        } else {
            // 结尾不是换行符，说明一行还没结束，放入buff中
            append_string_to_linked_list(one_line_buff, current_str, true);
        }
    }

    if (one_line_buff->size > 0) {
        char *one_line_str = concat_strings_in_linked_list(one_line_buff);
        append_string_to_linked_list(ini_file_strings, one_line_str, true);
        free_linked_list_nodes(one_line_buff);
    }

    fclose(file);
    return ini_file_strings;
}
