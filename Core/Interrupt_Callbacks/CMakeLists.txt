set(NAME interrupt_callbacks)

add_library(${NAME} STATIC ${CMAKE_CURRENT_LIST_DIR}/Inc/interrupt_callbacks.h)

target_include_directories(${NAME}
        PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Inc
        )

target_link_libraries(${NAME}
        tasks)
