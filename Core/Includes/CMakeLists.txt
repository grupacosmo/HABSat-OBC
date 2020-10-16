SET(NAME includes)

add_library(${NAME} INTERFACE)

target_include_directories(${NAME}
        INTERFACE ${CMAKE_CURRENT_LIST_DIR}/.
        )