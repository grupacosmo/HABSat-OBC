set(NAME Buses)

add_library(${NAME} spi.cpp)

target_include_directories(${NAME} PUBLIC Inc)

target_link_libraries(${NAME}
        MCU_Board
        GSL
        )