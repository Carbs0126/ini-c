//
// Created by Carbs0126 on 2023/6/20.
//
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ini_info.h"
#include "ini_utils.h"

ini_position new_ini_position(const char *file_location, int line_number, int char_begin, int char_end) {
    ini_position position;
    // 多个指针指向同一个内存，使用count的方式释放内存
    position.file_location = file_location;
    position.line_number = line_number;
    position.char_begin = char_begin;
    position.char_end = char_end;
    return position;
}

ini_element_comment *new_ini_element_comment_p(ini_position position, char *comment, bool comment_allocated) {
    ini_element_comment *element_comment = (ini_element_comment *) malloc(sizeof(ini_element_comment));
    element_comment->comment = comment;
    element_comment->comment_allocated = comment_allocated;
    element_comment->position = position;
    return element_comment;
}

ini_element_section_header *new_ini_element_section_header_p(ini_position position, char *name, bool name_allocated) {
    ini_element_section_header *element_section_header = (ini_element_section_header *) malloc(
            sizeof(ini_element_section_header));
    element_section_header->name = name;
    element_section_header->name_allocated = name_allocated;
    element_section_header->position = position;
    return element_section_header;
}

ini_element_empty *new_ini_element_empty_p(ini_position position) {
    ini_element_empty *element_empty = (ini_element_empty *) malloc(sizeof(ini_element_empty));
    element_empty->position = position;
    return element_empty;
}

ini_element_kv_pair *new_ini_element_kv_pair_p(ini_position position, char *key, char *value, bool key_allocated, bool value_allocated) {
    ini_element_kv_pair *element_kv_pair = (ini_element_kv_pair *) malloc(sizeof(ini_element_kv_pair));
    element_kv_pair->key = key;
    element_kv_pair->value = value;
    element_kv_pair->key_allocated = key_allocated;
    element_kv_pair->value_allocated = value_allocated;
    element_kv_pair->position = position;
    return element_kv_pair;
}

ini_element *new_ini_element(int type, void *pointer) {
    ini_element *element = (ini_element *) malloc(sizeof(ini_element));
    element->type = type;
    element->pointer = pointer;
    return element;
}

ini_object *new_ini_object() {
    ini_object *ini_obj = (ini_object *) malloc(sizeof(ini_object));
    ini_obj->ordered_sections_name = new_linked_list();
    ini_obj->ordered_sections_object = new_linked_list();
    return ini_obj;
}

ini_section_object *new_ini_section_object() {
    ini_section_object *section_obj = (ini_section_object *) malloc(sizeof(ini_section_object));
    // 注释可能没有，因此先不进行初始化
    section_obj->comments = NULL;
    section_obj->entry_objects = new_linked_list();
    // 使用 new_ini_element_section_header_p() 函数 初始化 section header 时，需要提供 position 和 name，所以暂时先不进行初始化
    section_obj->section_header = NULL;
    return section_obj;
}

ini_entry_object *new_ini_entry_object() {
    ini_entry_object *entry_obj = (ini_entry_object *) malloc(sizeof(ini_entry_object));
    entry_obj->kv_pair = NULL;
    entry_obj->comments = NULL;
    return entry_obj;
}

char *get_ini_position_description(ini_position *position) {
    if (position == NULL) {
        return NULL;
    } else {
        char *str_line_number = int_to_string(position->line_number);
        char *str_char_begin = int_to_string(position->char_begin);
        char *str_char_end = int_to_string(position->char_end);
        char *str_format = " ini_position { file_location=%s, line_number=%s, str_char_begin=%s, str_char_end=%s } ";
        int strlen_variables = strlen(position->file_location == NULL ? "" : position->file_location)
                               + strlen(str_line_number)
                               + strlen(str_char_begin)
                               + strlen(str_char_end)
                               + strlen(str_format)
                               + 1;
        char *str = (char *) malloc(sizeof(char) * strlen_variables);
        sprintf(str,
                str_format,
                position->file_location == NULL ? "" : position->file_location,
                str_line_number,
                str_char_begin,
                str_char_end);
        free(str_line_number);
        free(str_char_begin);
        free(str_char_end);
        return str;
    }
}

char *get_ini_element_empty_description(ini_element_empty *element) {
    if (element == NULL) {
        return NULL;
    }
    char *position_des = get_ini_position_description(&(element->position));
    char *str_format = " ini_element_empty { position=%s } ";
    int strlen_variables = strlen(position_des) + strlen(str_format) + 1;
    char *str = (char *) malloc(sizeof(char) * strlen_variables);
    sprintf(str, str_format, position_des);
    free(position_des);
    return str;
}

char *get_ini_element_comment_description(ini_element_comment *element) {
    if (element == NULL) {
        return NULL;
    }
    char *position_des = get_ini_position_description(&(element->position));
    char *str_format = " ini_element_comment { position=%s, comment=%s } ";
    int strlen_variables = strlen(position_des) + strlen(element->comment) + strlen(str_format) + 1;
    char *str = (char *) malloc(sizeof(char) * strlen_variables);
    sprintf(str, str_format, position_des, element->comment);
    free(position_des);
    return str;
}

char *get_ini_element_kv_pair_description(ini_element_kv_pair *element) {
    if (element == NULL) {
        return NULL;
    }
    char *position_des = get_ini_position_description(&(element->position));
    char *str_format = " ini_element_kv_pair { position=%s, key=%s, value=%s } ";
    int strlen_variables =
            strlen(position_des) + strlen(element->key) + strlen(element->value) + strlen(str_format) + 1;
    char *str = (char *) malloc(sizeof(char) * strlen_variables);
    sprintf(str, str_format, position_des, element->key, element->value);
    free(position_des);
    return str;
}

char *get_ini_element_section_header_description(ini_element_section_header *element) {
    if (element == NULL) {
        return NULL;
    }
    char *position_des = get_ini_position_description(&(element->position));
    char *str_format = " ini_element_section_header { position=%s, name=%s } ";
    int strlen_variables = strlen(position_des) + strlen(element->name) + strlen(str_format) + 1;
    char *str = (char *) malloc(sizeof(char) * strlen_variables);
    sprintf(str, str_format, position_des, element->name);
    free(position_des);
    return str;
}

char *get_real_ini_element_description_by_type(ini_element *element) {
    if (element == NULL) {
        return NULL;
    }
    switch (element->type) {
        case LINE_EMPTY:
            return get_ini_element_empty_description((ini_element_empty *) (element->pointer));
        case LINE_COMMENT:
            return get_ini_element_comment_description((ini_element_comment *) (element->pointer));
        case LINE_KV_PAIR:
            return get_ini_element_kv_pair_description((ini_element_kv_pair *) (element->pointer));
        case LINE_SECTION_HEADER:
            return get_ini_element_section_header_description((ini_element_section_header *) (element->pointer));
        default:
            return NULL;
    }
}

char *get_ini_element_description(ini_element *element) {
    if (element == NULL) {
        return NULL;
    } else {
        char *str_type = int_to_string(element->type);
        char *str_pointer = pointer_address_to_string(element->pointer);
        char *str_real_element = get_real_ini_element_description_by_type(element);
        char *str_format = " ini_element { type=%s, pointer=%s, element=%s } ";
        int strlen_variables = strlen(str_type)
                               + strlen(str_pointer)
                               + strlen(str_real_element)
                               + strlen(str_format)
                               + 1;
        char *str = (char *) malloc(sizeof(char) * strlen_variables);
        sprintf(str, str_format, str_type, str_pointer, str_real_element);
        free(str_type);
        free(str_pointer);
        free(str_real_element);
        return str;
    }
}

char *get_ini_elements_description(linked_list *elements) {
    if (elements == NULL) {
        return NULL;
    } else {
        char *str_size = int_to_string(elements->size);
        char *str_head_pointer = pointer_address_to_string(elements->head);
        linked_list *des_linked_list = new_linked_list();

        int des_char_length = 0;
        linked_list_node *current_node = elements->head;
        int index = 0;
        int elements_size = elements->size;
        while (index < elements_size) {
            char *des = get_ini_element_description((ini_element *) (current_node->value));
            append_string_to_linked_list(des_linked_list, des, true);
            des_char_length = des_char_length + strlen(des);
            current_node = current_node->next;
            index++;
        }
        char *one_str = (char *) malloc(sizeof(char) * des_char_length + 1);

        int current_length = 0;
        current_node = des_linked_list->head;
        index = 0;
        int des_size = des_linked_list->size;
        while (index < des_size) {
            strcpy(one_str + current_length, (char *) (current_node->value));
            current_length = current_length + strlen((char *) (current_node->value));
            current_node = current_node->next;
            index++;
        }

        char *str_format = " ini_elements_info { size=%s, head=%s, ini_elements=%s } ";
        int strlen_variables = strlen(str_size)
                               + strlen(str_head_pointer)
                               + strlen(one_str)
                               + strlen(str_format)
                               + 1;
        char *str = (char *) malloc(sizeof(char) * strlen_variables);
        sprintf(str, str_format, str_size, str_head_pointer, one_str);

        free(str_size);
        free(str_head_pointer);
        free_linked_list_nodes(des_linked_list);
        free(des_linked_list);

        return str;
    }
}

ini_position *get_position(ini_element *element) {
    if (element == NULL) {
        return NULL;
    }
    return (ini_position *) (element->pointer);
}

char *get_ini_element_output_string(ini_element *element, bool *str_allocated) {
    if (element == NULL) {
        if (str_allocated != NULL) {
            *str_allocated = false;
        }
        return "";
    }
    switch (element->type) {
        case LINE_EMPTY:
            return get_ini_element_empty_output_string((ini_element_empty *) (element->pointer), str_allocated);
        case LINE_COMMENT:
            return get_ini_element_comment_output_string((ini_element_comment *) (element->pointer), str_allocated);
        case LINE_KV_PAIR:
            return get_ini_element_kv_pair_output_string((ini_element_kv_pair *) (element->pointer), str_allocated);
        case LINE_SECTION_HEADER:
            return get_ini_element_section_header_output_string((ini_element_section_header *) (element->pointer), str_allocated);
        default:
            return NULL;
    }
}

char *get_ini_element_empty_output_string(ini_element_empty *element, bool *str_allocated) {
    if (str_allocated != NULL) {
        *str_allocated = false;
    }
    return "";
}

char *get_ini_element_comment_output_string(ini_element_comment *element, bool *str_allocated) {
    if (element == NULL) {
        if (str_allocated != NULL) {
            *str_allocated = false;
        }
        return "";
    }
    if (str_allocated != NULL) {
        *str_allocated = element->comment_allocated;
    }
    return element->comment;
}

char *get_ini_element_kv_pair_output_string(ini_element_kv_pair *element, bool *str_allocated) {
    if (element == NULL || element->key == NULL) {
        if (str_allocated != NULL) {
            *str_allocated = false;
        }
        return "";
    }
    char *value = element->value;
    if (value == NULL) {
        value = "";
    }
    char *str_format = "%s=%s";
    int strlen_variables = strlen(element->key)
                           + strlen(value)
                           + strlen(str_format)
                           + 1;
    char *str = (char *) malloc(sizeof(char) * strlen_variables);
    sprintf(str, str_format, element->key, value);
    if (str_allocated != NULL) {
        *str_allocated = true;
    }
    return str;
}

char *get_ini_element_section_header_output_string(ini_element_section_header *element, bool *str_allocated) {
    if (element == NULL || element->name == NULL) {
        if (str_allocated != NULL) {
            *str_allocated = false;
        }
        return "";
    }
    if (str_allocated != NULL) {
        *str_allocated = element->name_allocated;
    }
    return element->name;
}

linked_list_node *append_string_to_linked_list(linked_list *list_info, char *str, bool allocated) {
    linked_list_node *node = (linked_list_node *) malloc(sizeof(linked_list_node));
    node->value = str;
    node->allocated = allocated;
    append_linked_list_node(list_info, node);
    return node;
}

linked_list_node *append_char_to_linked_list(linked_list *list_info, char c) {
    linked_list_node *node = (linked_list_node *) malloc(sizeof(linked_list_node));
    char *s = (char *) malloc(sizeof(char) * 2);
    s[0] = c;
    s[1] = '\0';
    node->value = s;
    node->allocated = true;
    append_linked_list_node(list_info, node);
    return node;
}

linked_list_node *append_obj_to_linked_list(linked_list *list_info, void *obj, bool obj_allocated) {
    linked_list_node *node = (linked_list_node *) malloc(sizeof(linked_list_node));
    node->value = obj;
    node->allocated = obj_allocated;
    append_linked_list_node(list_info, node);
    return node;
}

void add_comment_to_entry_object(ini_entry_object *entry_obj, char *comment, bool is_comment_allocated) {
    if (entry_obj == NULL || comment == NULL) {
        return;
    }
    if (entry_obj->comments == NULL) {
        entry_obj->comments = new_linked_list();
    }
    append_string_to_linked_list(entry_obj->comments, comment, is_comment_allocated);
}

void add_comments_to_entry_object(ini_entry_object *entry_obj, linked_list *comments_linked_list) {
    if (entry_obj == NULL) {
        return;
    }
    if (comments_linked_list == NULL) {
        return;
    }
    if (entry_obj->comments == NULL) {
        entry_obj->comments = new_linked_list();
    }
    append_linked_list(entry_obj->comments, comments_linked_list);
}

void add_comment_to_section_object(ini_section_object *section_obj, char *comment, bool is_comment_allocated) {
    if (section_obj == NULL || comment == NULL) {
        return;
    }
    if (section_obj->comments == NULL) {
        section_obj->comments = new_linked_list();
    }
    append_string_to_linked_list(section_obj->comments, comment, is_comment_allocated);
}

void add_comments_to_section_object(ini_section_object *section_obj, linked_list *comments_linked_list) {
    if (section_obj == NULL) {
        return;
    }
    if (comments_linked_list == NULL) {
        return;
    }
    if (section_obj->comments == NULL) {
        section_obj->comments = new_linked_list();
    }
    append_linked_list(section_obj->comments, comments_linked_list);
}

void add_entry_object_to_section_object(ini_section_object *section_obj, ini_entry_object *entry_obj,
                                        bool entry_obj_allocated) {
    if (section_obj == NULL) {
        return;
    }
    if (entry_obj == NULL) {
        return;
    }
    if (section_obj->entry_objects == NULL) {
        section_obj->entry_objects = new_linked_list();
    }
    append_obj_to_linked_list(section_obj->entry_objects, entry_obj, entry_obj_allocated);
}

void add_section_to_ini_object(ini_object *ini_obj, ini_section_object *section_obj, bool section_obj_allocated) {
    if (ini_obj == NULL) {
        return;
    }
    if (section_obj == NULL || section_obj->section_header == NULL || section_obj->section_header->name == NULL) {
        return;
    }
    if (ini_obj->ordered_sections_name == NULL) {
        ini_obj->ordered_sections_name = new_linked_list();
    }
    if (ini_obj->ordered_sections_object == NULL) {
        ini_obj->ordered_sections_object = new_linked_list();
    }
    int name_len = strlen(section_obj->section_header->name);
    char *name = (char *) malloc(sizeof(char) * (name_len + 1));
    strcpy(name, section_obj->section_header->name);
    append_string_to_linked_list(ini_obj->ordered_sections_name, name, true);
    append_obj_to_linked_list(ini_obj->ordered_sections_object, section_obj, section_obj_allocated);
}

ini_section_object *find_section_object_by_name(ini_object *ini_obj, char *section_name, int *out_index) {
    if (ini_obj == NULL || section_name == NULL || ini_obj->ordered_sections_name == NULL ||
        ini_obj->ordered_sections_object == NULL) {
        return NULL;
    }
    int sections_name_size = ini_obj->ordered_sections_name->size;
    int index = 0;
    linked_list_node *current_node = ini_obj->ordered_sections_name->head;
    while (index < sections_name_size) {
        char *current_node_name = (char *) (current_node->value);
        if (strcmp(current_node_name, section_name) == 0) {
            break;
        }
        current_node = current_node->next;
        index++;
    }
    if (index == sections_name_size) {
        // 没有找到
        return NULL;
    }
    if (out_index != NULL) {
        *out_index = index;
    }
    current_node = ini_obj->ordered_sections_object->head;
    int index_of_sections_obj = 0;
    while (index_of_sections_obj < index) {
        current_node = current_node->next;
        index_of_sections_obj++;
    }
    return (ini_section_object *) (current_node->value);
}

char *find_value_by_section_and_key(ini_object *ini_obj, char *section_name, char *key_name) {
    int index_of_section_obj = -1;
    ini_section_object *sec_obj = find_section_object_by_name(ini_obj, section_name, &index_of_section_obj);
    if (index_of_section_obj == -1 || sec_obj == NULL || sec_obj->entry_objects == NULL) {
        return NULL;
    }
    int entry_objects_size = sec_obj->entry_objects->size;
    int index = 0;
    linked_list_node *current_node = sec_obj->entry_objects->head;
    while (index < entry_objects_size) {
        ini_entry_object *entry_obj = (ini_entry_object *) (current_node->value);
        if (entry_obj != NULL && entry_obj->kv_pair != NULL && entry_obj->kv_pair->key != NULL) {
            if (strcmp(key_name, entry_obj->kv_pair->key) == 0) {
                return entry_obj->kv_pair->value;
            }
        }
        index++;
        current_node = current_node->next;
    }
    return NULL;
}

linked_list *generate_elements_for_entry_obj(ini_entry_object *entry_obj) {
    if (entry_obj == NULL) {
        return NULL;
    }
    linked_list *ini_elements = new_linked_list();
    if (entry_obj->comments != NULL) {
        int size = entry_obj->comments->size;
        int index = 0;
        linked_list_node *cur_node = entry_obj->comments->head;
        while (index < size) {
            ini_element_comment *element_comment = (ini_element_comment *) cur_node->value;
            ini_element *ini_ele = new_ini_element(LINE_COMMENT, element_comment);
            append_obj_to_linked_list(ini_elements, ini_ele, true);
            index++;
            cur_node = cur_node->next;
        }
    }
    if (entry_obj->kv_pair != NULL) {
        ini_element *ini_ele = new_ini_element(LINE_KV_PAIR, entry_obj->kv_pair);
        append_obj_to_linked_list(ini_elements, ini_ele, true);
    }
    return ini_elements;
}

linked_list *generate_elements_for_section_obj(ini_section_object *section_obj) {
    if (section_obj == NULL) {
        return NULL;
    }
    linked_list *ini_elements = new_linked_list();
    if (section_obj->comments != NULL) {
        int size = section_obj->comments->size;
        int index = 0;
        linked_list_node *cur_node = section_obj->comments->head;
        while (index < size) {
            ini_element_comment *element_comment = (ini_element_comment *) cur_node->value;
            ini_element *ini_ele = new_ini_element(LINE_COMMENT, element_comment);
            append_obj_to_linked_list(ini_elements, ini_ele, true);
            index++;
            cur_node = cur_node->next;
        }
    }
    if (section_obj->section_header != NULL) {
        ini_element *ini_ele = new_ini_element(LINE_SECTION_HEADER, section_obj->section_header);
        append_obj_to_linked_list(ini_elements, ini_ele, true);
    }
    if (section_obj->entry_objects != NULL) {
        int size = section_obj->entry_objects->size;
        int index = 0;
        linked_list_node *cur_node = section_obj->entry_objects->head;
        while (index < size) {
            ini_entry_object *entry_obj = (ini_entry_object *) cur_node->value;
            linked_list *ini_elements_of_entry_obj = generate_elements_for_entry_obj(entry_obj);
            append_linked_list(ini_elements, ini_elements_of_entry_obj);
            index++;
            cur_node = cur_node->next;
        }
    }
    return ini_elements;
}

// linked_list_node*
int compare_ini_element_position_function (const void * a, const void * b) {
    // 数组里真正存放的数据
    linked_list_node *node_p_a = *(linked_list_node**)a;
    linked_list_node *node_p_b = *(linked_list_node**)b;
    if (node_p_a == NULL) {
        return 1;
    }
    if (node_p_b == NULL) {
        return -1;
    }
    if (node_p_a->value == NULL) {
        return 1;
    }
    if (node_p_b->value == NULL) {
        return -1;
    }
    ini_element *ele_a = (ini_element *)node_p_a->value;
    ini_element *ele_b = (ini_element *)node_p_b->value;

    ini_position *pos_a = get_position(ele_a);
    ini_position *pos_b = get_position(ele_b);

    // 如果position是空的
    if (pos_a->file_location == NULL && pos_b->file_location == NULL) {
        return 0;
    } else if (pos_a->file_location == NULL) {
        return 1;
    } else if (pos_b->file_location == NULL) {
        return -1;
    }

    int lineNumber = pos_a->line_number - pos_b->line_number;
    if (lineNumber != 0) {
        return lineNumber;
    }

    return pos_a->char_begin - pos_b->char_begin;
}


linked_list *generate_string_lines_for_ini_object(ini_object *ini_obj) {
    if (ini_obj == NULL || ini_obj->ordered_sections_object == NULL) {
        return NULL;
    }
    // 收集 elements
    linked_list *ele_lines = new_linked_list();
    int size = ini_obj->ordered_sections_object->size;
    int index = 0;
    linked_list_node *cur_node = ini_obj->ordered_sections_object->head;
    while (index < size) {
        ini_section_object *section_obj = (ini_section_object *) cur_node->value;
        linked_list *ini_elements_of_section_obj = generate_elements_for_section_obj(section_obj);
        append_linked_list(ele_lines, ini_elements_of_section_obj);
        index++;
        cur_node = cur_node->next;
    }

    // 新建一个每个元素都是指针的数组，指向每个的node
    linked_list_node **array_nodes = (linked_list_node **) malloc(sizeof(linked_list_node *) * ele_lines->size);
    size = ele_lines->size;
    index = 0;
    cur_node = ele_lines->head;
    while (index < size) {
        array_nodes[index] = cur_node;
        index++;
        cur_node = cur_node->next;
    }

    for (int i = 0; i < ele_lines->size; i++) {
        printf("&&&&&&&&--->linked_list_node * address is  %s \n", pointer_address_to_string(array_nodes[i]));
    }

    qsort(array_nodes, ele_lines->size, sizeof(linked_list_node *), compare_ini_element_position_function);

    for (int j = 0; j < ele_lines->size; j++) {
        printf("********--->linked_list_node * address is  %s \n", pointer_address_to_string(array_nodes[j]));
    }

    // 生成 strings
    linked_list *str_lines = new_linked_list();
    linked_list *one_line_str_builder = new_linked_list();
    int pre_line_number = -1;
    int cur_line_number = -1;

    size = ele_lines->size;
    index = 0;
    while (index < size) {
        cur_node = array_nodes[index];
        ini_element *ini_ele = (ini_element *) cur_node->value;
        ini_position *position = get_position(ini_ele);
        // ini_position 是结构体，因此判断position是否没有初始化的标志是，字段file_location == NULL
        if (position->file_location == NULL) {
            if (one_line_str_builder->size > 0) {
                char *one_line_str = concat_strings_in_linked_list(one_line_str_builder);
                append_string_to_linked_list(str_lines, one_line_str, true);
                free_linked_list_nodes(one_line_str_builder);
            }
            bool str_allocated = true;
            char *output_str = get_ini_element_output_string(ini_ele, &str_allocated);
            append_string_to_linked_list(str_lines, output_str, str_allocated);
            index++;
            continue;
        }

        cur_line_number = position->line_number;

        if (pre_line_number != cur_line_number) {
            if (pre_line_number > -1) {
                char *one_line_str = concat_strings_in_linked_list(one_line_str_builder);
                append_string_to_linked_list(str_lines, one_line_str, true);
                clear_linked_list_by_only_cut_pointers(one_line_str_builder);
            }
            int lineDelta = cur_line_number - pre_line_number;
            if (lineDelta > 1) {
                // 中间有空行
                for (int i = 0; i < lineDelta - 1; i++) {
                    append_string_to_linked_list(str_lines, "", false);
                }
            }
            bool str_allocated = true;
            char *output_str = get_ini_element_output_string(ini_ele, &str_allocated);
            append_string_to_linked_list(one_line_str_builder, output_str, str_allocated);
        } else {
            bool str_allocated = true;
            char *output_str = get_ini_element_output_string(ini_ele, &str_allocated);
            append_string_to_linked_list(one_line_str_builder, output_str, str_allocated);
        }
        pre_line_number = cur_line_number;
        index++;
    }

    char *one_line_str = concat_strings_in_linked_list(one_line_str_builder);
    append_string_to_linked_list(str_lines, one_line_str, true);
    clear_linked_list_by_only_cut_pointers(one_line_str_builder);
    free(array_nodes);
    return str_lines;
}

void free_ini_file_elements(linked_list *ini_file_elements) {
    if (ini_file_elements == NULL) {
        return;
    }

    linked_list_node *curr_node = ini_file_elements->head;
    linked_list_node *prev_node = NULL;
    int index = 0;
    int size = ini_file_elements->size;
    while (index < size) {
        if (curr_node->value != NULL) {
            ini_element *ini_ele = (ini_element *) curr_node->value;
            if (ini_ele->pointer != NULL) {
                free(ini_ele->pointer);
                ini_ele->pointer = NULL;
            }
            free(ini_ele);
            curr_node->value = NULL;
        }
        prev_node = curr_node;
        curr_node = curr_node->next;
        if (ini_file_elements->tail == prev_node) {
            ini_file_elements->tail = NULL;
        }
        free(prev_node);
        prev_node = NULL;
        ini_file_elements->head = curr_node;
        ini_file_elements->size--;
        index++;
    }
}

void free_entry_obj(ini_entry_object *entry_obj) {
    if (entry_obj == NULL) {
        return;
    }
    if (entry_obj->kv_pair != NULL) {
        if (entry_obj->kv_pair->key != NULL && entry_obj->kv_pair->key_allocated) {
            free(entry_obj->kv_pair->key);
            entry_obj->kv_pair->key = NULL;
        }
        if (entry_obj->kv_pair->value != NULL && entry_obj->kv_pair->value_allocated) {
            free(entry_obj->kv_pair->value);
            entry_obj->kv_pair->value = NULL;
        }
        free(entry_obj->kv_pair);
        entry_obj->kv_pair = NULL;
    }
    if (entry_obj->comments != NULL) {
        free_linked_list_nodes(entry_obj->comments);
        free(entry_obj->comments);
        entry_obj->comments = NULL;
    }
}

void free_section_obj(ini_section_object *section_obj) {
    if (section_obj == NULL) {
        return;
    }
    if (section_obj->section_header != NULL) {
        if (section_obj->section_header->name != NULL && section_obj->section_header->name_allocated) {
            free(section_obj->section_header->name);
        }
        free(section_obj->section_header);
        section_obj->section_header = NULL;
    }
    if (section_obj->comments != NULL) {
        free_linked_list_nodes(section_obj->comments);
        free(section_obj->comments);
        section_obj->comments = NULL;
    }
    if (section_obj->entry_objects != NULL) {
        free_linked_list_nodes_with_function(section_obj->entry_objects, (CUSTOM_FREE_FUNCTION) free_entry_obj);
        free(section_obj->entry_objects);
        section_obj->entry_objects = NULL;
    }
}

void free_ini_object(ini_object *ini_obj) {
    if (ini_obj == NULL) {
        return;
    }
    if (ini_obj->ordered_sections_name != NULL) {
        free_linked_list_nodes(ini_obj->ordered_sections_name);
        free(ini_obj->ordered_sections_name);
        ini_obj->ordered_sections_name = NULL;
    }
    if (ini_obj->ordered_sections_object != NULL) {
        free_linked_list_nodes_with_function(ini_obj->ordered_sections_object, (CUSTOM_FREE_FUNCTION) free_section_obj);
        free(ini_obj->ordered_sections_object);
        ini_obj->ordered_sections_object = NULL;
    }
}