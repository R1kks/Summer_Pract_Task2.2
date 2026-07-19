#CMakeLists.txt - файл сборки тестов
#
#Дозоров Владимир Евгеньевич
#Группа: МК - 101

get_all_sources(${CMAKE_CURRENT_SOURCE_DIR} TEST_SOURCES)
add_executable(test_hex ${TEST_SOURCES})

target_include_directories(test_hex PRIVATE ${CMAKE_SOURCE_DIR}/lib/include)


target_link_libraries(test_hex PRIVATE hexdump_lib)

add_test(NAME HexLogicTests COMMAND test_hex)