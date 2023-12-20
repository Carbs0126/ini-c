cmake_minimum_required(VERSION 3.25)
project(ini_c C)

set(CMAKE_C_STANDARD 11)

add_executable(ini_c main.c ini/ini_info.c ini/ini_utils.c linked_list/linked_list.c ini/ini_file_parser.c ini/ini_file_generator.c)
