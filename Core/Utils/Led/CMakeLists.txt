set(NAME led)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR} SOURCE)

add_library(${NAME} STATIC ${SOURCE})

target_include_directories(${NAME}
        PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Inc
        )

target_link_libraries(${NAME}
        freertos_wrapper
        OBC)

set_target_properties(${NAME} PROPERTIES LINKER_LANGUAGE CXX)