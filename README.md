# ini文件解析
## ini文件格式说明
https://zh.wikipedia.org/wiki/INI%E6%96%87%E4%BB%B6

``` java
; last modified 1 April 2001 by John Doe
[owner]
name=John Doe
organization=Acme Products

[database]
server=192.0.2.42 ; use IP address in case network name resolution is not working
port=143
file="acme payroll.dat"
```

## 附加说明
1. 键值对中，允许值中存在空格；
2. section 没有做嵌套处理

## 使用
``` c
void parse_ini_file() {

    int error_code = 0;
    // 将一个ini文件解析成一个struct
    ini_object *ini_obj = parse_file_to_ini_object("../test-input.ini", &error_code);

    // 向这个struct中添加section、键值对等信息
    ini_section_object *section_object = new_ini_section_object();
    section_object->section_header = new_ini_element_section_header_p(new_ini_position(NULL, 0,0,0), "[new_section]", false);
    ini_entry_object *entry_object_0 = new_ini_entry_object();
    ini_entry_object *entry_object_1 = new_ini_entry_object();
    entry_object_0->kv_pair = new_ini_element_kv_pair_p(new_ini_position(NULL, 0, 0, 0), "new_key_0", "new_value_0", false, false);
    entry_object_1->kv_pair = new_ini_element_kv_pair_p(new_ini_position(NULL, 0, 0, 0), "new_key_1", "new_value_1", false, false);

    add_entry_object_to_section_object(section_object, entry_object_0, true);
    add_entry_object_to_section_object(section_object, entry_object_1, true);
    add_section_to_ini_object(ini_obj, section_object, true);

    if (error_code == 0) {
        char *a_value = find_value_by_section_and_key(ini_obj, "[owner]", "organization");

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

    # 把这个对象存储回文件中
    generate_file_from_ini_object(ini_obj, "../test-output.ini", &error_code);
    # 释放这个内存中的ini对象
    free_ini_object(ini_obj);
}
```
