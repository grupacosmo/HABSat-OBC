set(NAME peripherals)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR} SOURCE)

add_library(${NAME} STATIC ${SOURCE})

target_include_directories(${NAME}
        PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Inc
        )

target_link_libraries(${NAME}
        wifi
        lcd
        led
        includes)