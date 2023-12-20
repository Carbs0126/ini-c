//
// Created by Carbs0126 on 2023/6/20.
//

#ifndef INI_C_INI_INFO_H
#define INI_C_INI_INFO_H

#include "../linked_list/linked_list.h"

// ============================= 类型 =============================

enum {
    LINE_EMPTY,
    LINE_COMMENT,
    LINE_KV_PAIR,
    LINE_SECTION_HEADER
};

// 每个 line 中可能包含一个或者 ini_element
typedef struct _ini_element_ {
    int type;
    void *pointer;
} ini_element;

// element 的位置信息
typedef struct _ini_position_ {
    const char *file_location;
    // todo 多个指针指向同一块内存，可能会多次释放，因此需要使用count计数
    bool file_location_allocated;
    int line_number;
    int char_begin;
    int char_end;
} ini_position;

// 一行元素，空
typedef struct _ini_element_empty_ {
    ini_position position;
} ini_element_empty;

// 一行元素，注释
typedef struct _ini_element_comment_ {
    ini_position position;
    char *comment;
    bool comment_allocated;
} ini_element_comment;

// 一行元素，kv
typedef struct _ini_element_kv_pair_ {
    ini_position position;
    char *key;
    char *value;
    bool key_allocated;
    bool value_allocated;
} ini_element_kv_pair;

// 一行元素，section header
typedef struct _ini_element_section_header_ {
    ini_position position;
    char *name;
    bool name_allocated;
} ini_element_section_header;

// ini文件中的一个kv对
typedef struct _ini_entry_object_ {
    ini_element_kv_pair *kv_pair;
    // ini_element_comment* 组成的列表
    linked_list *comments;
} ini_entry_object;

// ini文件中的一个section
typedef struct _ini_section_object_ {
    ini_element_section_header *section_header;
    linked_list *comments;
    linked_list *entry_objects;
} ini_section_object;

// 一个文件解析后生成一个ini_object
typedef struct _ini_object_ {
    // 按照从ini文件中出现的位置来排序的列表，每个node中保存的是char*
    linked_list *ordered_sections_name;
    // 按照从ini文件中出现的位置来排序的列表，每个node中保存的是ini_section_object*
    linked_list *ordered_sections_object;
} ini_object;




// ============================= 函数 =============================

ini_position new_ini_position(const char *file_location, int line_number, int char_begin, int char_end);

ini_element_comment *new_ini_element_comment_p(ini_position position, char *comment, bool comment_allocated);

ini_element_section_header *new_ini_element_section_header_p(ini_position position, char *name, bool name_allocated);

ini_element_empty *new_ini_element_empty_p(ini_position position);

ini_element_kv_pair *new_ini_element_kv_pair_p(ini_position position, char *key, char *value, bool key_allocated, bool value_allocated);

ini_element *new_ini_element(int type, void *pointer);

ini_object *new_ini_object();

ini_section_object *new_ini_section_object();

ini_entry_object *new_ini_entry_object();

char *get_ini_position_description(ini_position *position);

char *get_ini_element_empty_description(ini_element_empty *element);

char *get_ini_element_comment_description(ini_element_comment *element);

char *get_ini_element_kv_pair_description(ini_element_kv_pair *element);

char *get_ini_element_section_header_description(ini_element_section_header *element);

char *get_ini_element_description(ini_element *element);

char *get_ini_elements_description(linked_list *elements);

ini_position *get_position(ini_element *element);

char *get_ini_element_empty_output_string(ini_element_empty *element, bool *str_allocated);

char *get_ini_element_comment_output_string(ini_element_comment *element, bool *str_allocated);

char *get_ini_element_kv_pair_output_string(ini_element_kv_pair *element, bool *str_allocated);

char *get_ini_element_section_header_output_string(ini_element_section_header *element, bool *str_allocated);

char *get_ini_element_output_string(ini_element *element, bool *str_allocated);

linked_list_node *append_string_to_linked_list(linked_list *list_info, char *str, bool allocated);

linked_list_node *append_char_to_linked_list(linked_list *list_info, char c);

linked_list_node *append_obj_to_linked_list(linked_list *list_info, void *obj, bool obj_allocated);

void add_comment_to_entry_object(ini_entry_object *entry_obj, char *comment, bool is_comment_allocated);

void add_comments_to_entry_object(ini_entry_object *entry_obj, linked_list *comments_linked_list);

void add_comment_to_section_object(ini_section_object *section_obj, char *comment, bool is_comment_allocated);

void add_comments_to_section_object(ini_section_object *section_obj, linked_list *comments_linked_list);

void add_entry_object_to_section_object(ini_section_object *section_obj, ini_entry_object *entry_obj, bool entry_obj_allocated);

void add_section_to_ini_object(ini_object *ini_obj, ini_section_object *section_obj, bool section_obj_allocated);

ini_section_object *find_section_object_by_name(ini_object *ini_obj, char *section_name, int *out_index);

char *find_value_by_section_and_key(ini_object *ini_obj, char *section_name, char *key_name);

linked_list *generate_elements_for_entry_obj(ini_entry_object *entry_obj);

linked_list *generate_elements_for_section_obj(ini_section_object *section_obj);

linked_list *generate_string_lines_for_ini_object(ini_object *ini_obj);

void free_ini_file_elements(linked_list *ini_file_elements);

void free_entry_obj(ini_entry_object *entry_obj);

void free_section_obj(ini_section_object *section_obj);

void free_ini_object(ini_object *ini_obj);

#endif //INI_C_INI_INFO_H
