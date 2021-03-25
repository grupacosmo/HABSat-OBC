set(NAME Buses)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR} FILES)

add_library(${NAME} INTERFACE ${FILES})

target_include_directories(${NAME}
        INTERFACE ${CMAKE_CURRENT_LIST_DIR}
        )

target_link_libraries(${NAME}
        INTERFACE Utils
        Include_Files
        MCU_Board
        )