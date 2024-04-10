cmake_minimum_required(VERSION 3.10)
project(tetrisTutor)

set(CMAKE_C_STANDARD 11)

add_executable(tetrisTutor main.c ncurses/curses.h)

# Подключение заголовочных файлов ncurses
include_directories(${CMAKE_SOURCE_DIR}/ncurses)

# Поиск и подключение библиотеки ncurses
find_library(NCURSES_LIBRARY NAMES ncurses)
target_link_libraries(tetrisTutor ${NCURSES_LIBRARY})
