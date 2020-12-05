set(NAME BME280_Compensation)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR} FILES)

add_library(${NAME} INTERFACE ${FILES})

target_include_directories(${NAME}
        INTERFACE ${CMAKE_CURRENT_LIST_DIR}
        )