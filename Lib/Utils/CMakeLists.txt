set(NAME Utils)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR} SOURCE)

add_library(${NAME} STATIC ${SOURCE} Inc/Utils/to_bytes.hpp)

target_include_directories(${NAME} PUBLIC Inc)