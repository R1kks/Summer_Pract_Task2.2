# CMakeLists.txt - корневой файл сборки проекта.
#Дозоров Владимир Евгеньевич
#Группа: МК - 101

cmake_minimum_required(VERSION 3.10)

project(task2_2_hexdump C)

set(CMAKE_C_STANDARD 11)

include(${CMAKE_SOURCE_DIR}/cmake/MyFunctions.cmake)

enable_testing()

add_subdirectory(lib)
add_subdirectory(app)
add_subdirectory(tests)