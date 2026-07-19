# CMakeLists.txt - настройки сборки основного приложения.
# Дозоров Владимир Евгеньевич
#Группа: МК - 101

get_all_sources(${CMAKE_CURRENT_SOURCE_DIR}/src APP_SOURCES)

add_executable(hexdump_app ${APP_SOURCES})

target_include_directories(hexdump_app PRIVATE ${CMAKE_SOURCE_DIR}/lib/include)
target_link_libraries(hexdump_app PRIVATE hexdump_lib_static)