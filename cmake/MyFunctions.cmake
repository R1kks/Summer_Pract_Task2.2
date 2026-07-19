# MyFunctions.cmake - пользовательские макросы для сборки.
# 
#Дозоров Владимир Евгеньевич
#Группа: МК - 101

macro(get_all_sources dir result_var)
    file(GLOB ${result_var} "${dir}/*.c" "${dir}/*.h")
endmacro()