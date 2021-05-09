set(NAME LoRaSX1278)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR} SOURCE)

add_library(${NAME} STATIC ${SOURCE})

target_include_directories(${NAME} PUBLIC Inc)

target_link_libraries(${NAME}
        Buses
        )