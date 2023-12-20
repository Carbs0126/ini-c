#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include "ini/ini_info.h"
#include "ini/ini_file_parser.h"
#include "ini/ini_file_generator.h"

void parse_ini_file();

void print_comments(linked_list *comments, char *format);

int main(int argc, char *argv[]) {
    parse_ini_file();
    return 0;
}

void parse_ini_file() {

    int error_code = 0;
    ini_object *ini_obj = parse_file_to_ini_object("../test-input.ini", &error_code);

    ini_section_object *section_object = new_ini_section_object();
    section_object->section_header = new_ini_element_section_header_p(new_ini_position(NULL, 0,0,0), "[new_section]", false);
    ini_entry_object *entry_object_0 = new_ini_entry_object();
    ini_entry_object *entry_object_1 = new_ini_entry_object();
    entry_object_0->kv_pair = new_ini_element_kv_pair_p(new_ini_position(NULL, 0, 0, 0), "new_key_0", "new_value_0", false, false);
    entry_object_1->kv_pair = new_ini_element_kv_pair_p(new_ini_position(NULL, 0, 0, 0), "new_key_1", "new_value_1", false, false);

    add_entry_object_to_section_object(section_object, entry_object_0, true);
    add_entry_object_to_section_object(section_object, entry_object_1, true);
    add_section_to_ini_object(ini_obj, section_object, true);

    printf("---------error_code:%d---------", error_code);
    if (error_code == 0) {
        printf("----------------no errors----------------\n");

        char *a_value = find_value_by_section_and_key(ini_obj, "[owner]", "organization");
        printf("--------->section [owner] key organization value-->%s<---\n", a_value);

        int index = 0;
        int sections_name_size = ini_obj->ordered_sections_name->size;
        linked_list_node *section_name_node = ini_obj->ordered_sections_name->head;
        while (index < sections_name_size) {
            printf("section_name: %s\n", (char *) (section_name_node->value));
            index++;
            section_name_node = section_name_node->next;
        }

        index = 0;
        int sections_object_size = ini_obj->ordered_sections_object->size;
        linked_list_node *section_obj_node = ini_obj->ordered_sections_object->head;
        while (index < sections_object_size) {
            ini_section_object *section_obj = (ini_section_object *) (section_obj_node->value);

            printf("section_obj->section_header->name: %s\n", section_obj->section_header->name);
            printf("section_obj->section_header->position: %s\n",
                   get_ini_position_description(&section_obj->section_header->position));

            print_comments(section_obj->comments, "section_obj->comment: %s\n");
            linked_list_node *entry_obj_node = section_obj->entry_objects->head;
            int entry_obj_index = 0;
            int entry_objects_size = section_obj->entry_objects->size;
            while (entry_obj_index < entry_objects_size) {
                ini_entry_object *entry_obj = (ini_entry_object *) (entry_obj_node->value);
                char *kv = get_ini_element_kv_pair_output_string(entry_obj->kv_pair, NULL);
                printf("section_obj->entry_obj->kv: %s\n", kv);
                printf("section_obj->entry_obj->position: %s\n",
                       get_ini_position_description(&entry_obj->kv_pair->position));
                print_comments(entry_obj->comments, "section_obj->entry_obj->comment: %s\n");
                entry_obj_index++;
                entry_obj_node = entry_obj_node->next;
            }
            index++;
            section_obj_node = section_obj_node->next;
        }
    } else {
        printf("----------------some errors happened----------------\n");
    }

    generate_file_from_ini_object(ini_obj, "../test-output.ini", &error_code);

    free_ini_object(ini_obj);
}

void print_comments(linked_list *comments, char *format) {
    if (comments == NULL) {
        return;
    }
    linked_list_node *comment_node = comments->head;
    int index = 0;
    int comments_size = comments->size;
    while (index < comments_size) {
        ini_element_comment *ele_comment = (ini_element_comment *) (comment_node->value);
        printf(format, ele_comment->comment);
        index++;
        comment_node = comment_node->next;
    }
}

void test_element() {
    linked_list *elements_info = new_linked_list();

    ini_element_empty *empty_element0 = (ini_element_empty *) malloc(sizeof(ini_element_empty));
    empty_element0->position = new_ini_position("c:", 1, 2, 10);
    ini_element *element0 = new_ini_element(LINE_EMPTY, empty_element0);
    append_obj_to_linked_list(elements_info, element0, true);

    ini_element_comment *comment_element1 = (ini_element_comment *) malloc(sizeof(ini_element_comment));
    comment_element1->position = new_ini_position("cd:", 2, 0, 10);
    comment_element1->comment = "this is a comment";
    comment_element1->comment_allocated = false;
    ini_element *element1 = new_ini_element(LINE_COMMENT, comment_element1);
    append_obj_to_linked_list(elements_info, element1, true);

    ini_element_kv_pair *kv_element2 = (ini_element_kv_pair *) malloc(sizeof(ini_element_kv_pair));
    kv_element2->position = new_ini_position("er:", 3, 0, 10);
    kv_element2->key = "mykey";
    kv_element2->value = "myvalue";
    ini_element *element2 = new_ini_element(LINE_KV_PAIR, kv_element2);
    append_obj_to_linked_list(elements_info, element2, true);

    ini_element_section_header *section_header_element3 = (ini_element_section_header *) malloc(
            sizeof(ini_element_section_header));
    section_header_element3->position = new_ini_position("er:", 4, 0, 10);
    section_header_element3->name = "mysectionname";
    ini_element *element3 = new_ini_element(LINE_SECTION_HEADER, section_header_element3);
    append_obj_to_linked_list(elements_info, element3, true);

    char *str_element_info = get_ini_elements_description(elements_info);
    printf("%s\n", str_element_info);

    printf("=============================\n");

    ini_position *position_pointer = get_position((ini_element *) (elements_info->head->next->next->value));
    char *position_des = get_ini_position_description(position_pointer);
    printf("position_des---> %s\n", position_des);

}