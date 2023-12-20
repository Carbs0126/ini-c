//
// Created by Carbs0126 on 2023/6/26.
//

#ifndef INI_C_LINKED_LIST_H
#define INI_C_LINKED_LIST_H
#include <stdbool.h>

typedef struct _linked_list_node_ {
    void *value;
    bool allocated; // value 指向的内存是否为 alloc 函数族开辟的
    struct _linked_list_node_ *prev;
    struct _linked_list_node_ *next;
} linked_list_node;

// todo 把 size 改成 length
typedef struct _linked_list_ {
    int size;
    linked_list_node *head;
    linked_list_node *tail;
} linked_list;

// int : index
typedef void (*LINKED_LIST_TRAVERSE_FUNCTION)(int, linked_list_node *, bool *stop_traverse);

typedef void (*CUSTOM_FREE_FUNCTION)(void *pointer_to_be_freed);

// 新建 linked_list
linked_list *new_linked_list();

// 新增一个节点
void append_linked_list_node(linked_list *list_info, linked_list_node *list_node);

// 新增多个节点，传入第一个节点，节点都是头尾相接的
void append_linked_list_nodes(linked_list *list_info, linked_list_node *list_node_head, int nodes_length);

// 将 list_src 合并到 list_dst
void append_linked_list(linked_list *list_dst, linked_list *list_src);

// 针对只存放字符串的场景
char *concat_strings_in_linked_list(linked_list *list_of_str);

// 遍历所有节点
void traverse_linked_list(linked_list *list_info, LINKED_LIST_TRAVERSE_FUNCTION traverse_function);

void free_linked_list_nodes_with_function(linked_list *list_info, CUSTOM_FREE_FUNCTION free_function);

void free_linked_list_nodes(linked_list *list_info);

void free_linked_list_nodes_without_value(linked_list *list_info);

void clear_linked_list_by_only_cut_pointers(linked_list *list_info);

#endif //INI_C_LINKED_LIST_H
