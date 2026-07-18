#CmakeLists.txt - инициализация настроект программы
#Дозоров Владимир Евгеньевич
#Группа: МК - 101

get_all_sources(${CMAKE_CURRENT_SOURCE_DIR}/src LIB_SOURCES)
file(GLOB INCLUDE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

add_library(hexdump_lib_static STATIC ${LIB_SOURCES} ${INCLUDE_FILES})
target_include_directories(hexdump_lib_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(hexdump_lib_shared SHARED ${LIB_SOURCES} ${INCLUDE_FILES})
target_include_directories(hexdump_lib_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_target_properties(hexdump_lib_shared PROPERTIES OUTPUT_NAME hexdump_lib)