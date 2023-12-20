//
// Created by Carbs0126 on 2023/6/26.
//
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"


linked_list *new_linked_list() {
    linked_list *list_info = (linked_list *)malloc(sizeof(linked_list));
    list_info->size = 0;
    list_info->head = NULL;
    list_info->tail = NULL;
    return list_info;
}

void append_linked_list_node(linked_list *list_info, linked_list_node *list_node) {
    if (list_info == NULL) {
        return;
    }
    if (list_node == NULL) {
        return;
    }
    // 尾部添加 list_node
    if (list_info->head == NULL) {
        list_info->head = list_node;
    }
    if (list_info->tail == NULL) {
        list_info->tail = list_node;
    } else {
        list_info->tail->next = list_node;
    }
    if (list_info->size > 0) {
        list_node->prev = list_info->tail;
    }
//    list_node->next = NULL;
    list_info->tail = list_node;
    // 更新 size
    list_info->size = list_info->size + 1;
}

// 实际上是move from src to dst
void append_linked_list_nodes(linked_list *list_info, linked_list_node *list_node_head, int nodes_length) {
    if (list_info == NULL) {
        return ;
    }
    linked_list_node *current_node = list_node_head;
    int index = 0;
    while(index < nodes_length) {
        append_linked_list_node(list_info, current_node);
        current_node = current_node->next;
        index++;
    }
}

void append_linked_list(linked_list *list_dst, linked_list *list_src) {
    if (list_dst == NULL || list_src == NULL) {
        return;
    }
    if (list_src->head == NULL) {
        return;
    }
    append_linked_list_nodes(list_dst, list_src->head, list_src->size);
}

char *concat_strings_in_linked_list(linked_list *list_of_str) {
    int list_strings_length = 0;
    int index = 0;
    int size = list_of_str->size;
    linked_list_node *current_node = list_of_str->head;
    while (index < size) {
        list_strings_length = list_strings_length + strlen((char *) (current_node->value));
        index++;
        current_node = current_node->next;
    }

    char *one_line_str = (char *) malloc(sizeof(char) * list_strings_length + 1);
    int current_length = 0;
    index = 0;
    current_node = list_of_str->head;
    while (index < size) {
        strcpy(one_line_str + current_length, (char *) (current_node->value));
        current_length = current_length + strlen((char *) (current_node->value));
        index++;
        current_node = current_node->next;
    }
//    free_linked_list_nodes(list_of_str);
    return one_line_str;
}

void traverse_linked_list(linked_list *list_info, LINKED_LIST_TRAVERSE_FUNCTION traverse_function) {
    if (list_info == NULL) {
        return;
    }
    if (traverse_function == NULL) {
        return;
    }
    linked_list_node *current_node = list_info->head;
    int index = 0;
    int size = list_info->size;
    bool stop_traverse = false;
    while (index < size && !stop_traverse) {
        traverse_function(index, current_node, &stop_traverse);
        index++;
        current_node = current_node->next;
    }
}

void empty_free_function(void *pointer_do_not_care) {

}

void free_linked_list_nodes_with_function(linked_list *list_info, CUSTOM_FREE_FUNCTION free_function) {
    if (list_info == NULL || free_function == NULL) {
        return;
    }
    linked_list_node *curr_node = list_info->head;
    linked_list_node *prev_node = NULL;
    int index = 0;
    int size = list_info->size;
    while (index < size) {
        if (curr_node->value != NULL && curr_node->allocated) {
            free_function(curr_node->value);
            curr_node->value = NULL;
        }
        prev_node = curr_node;
        curr_node = curr_node->next;
        if (list_info->tail == prev_node) {
            list_info->tail = NULL;
        }
        free(prev_node);
        prev_node = NULL;
        list_info->head = curr_node;
        list_info->size--;
        index++;
    }
}

void free_linked_list_nodes(linked_list *list_info) {
    free_linked_list_nodes_with_function(list_info, free);
}

void free_linked_list_nodes_without_value(linked_list *list_info) {
    free_linked_list_nodes_with_function(list_info, empty_free_function);
}

void clear_linked_list_by_only_cut_pointers(linked_list *list_info) {
    if (list_info == NULL) {
        return;
    }
    list_info->tail = NULL;
    list_info->head = NULL;
    list_info->size = 0;
}
