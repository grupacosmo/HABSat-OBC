set(NAME MCU_Board)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR} SOURCE)

add_library(${NAME} INTERFACE ${SOURCE})

target_include_directories(${NAME}
        INTERFACE ${CMAKE_CURRENT_LIST_DIR}
        )

target_link_libraries(${NAME} INTERFACE
        Utils
        )

add_subdirectory(Buses)