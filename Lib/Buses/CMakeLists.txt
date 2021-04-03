set(NAME Buses)

add_library(${NAME} INTERFACE)

target_include_directories(${NAME} INTERFACE Inc)

target_link_libraries(${NAME} INTERFACE
        Utils
        MCU_Board
        )