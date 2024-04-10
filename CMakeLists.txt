cmake_minimum_required(VERSION 3.10)
project(tetrisTutor)

set(CMAKE_C_STANDARD 11)

link_directories(lib)

add_executable(tetrisTutor main.c)

target_link_libraries(tetrisTutor libformw.dll libmenuw.dll libncursesw.dll libpanelw.dll)

# Подключение заголовочных файлов ncurses
include_directories(${CMAKE_SOURCE_DIR}/include)
