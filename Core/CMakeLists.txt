add_subdirectory(Utils)
add_subdirectory(System)
add_subdirectory(Peripherals)
add_subdirectory(Include_Files)
add_subdirectory(External_Libs)
add_subdirectory(MCU_Board)
add_subdirectory(Services)

set(NAME Core)

add_library(${NAME} STATIC mainLoop.cpp interruptHandlers.cpp Obc.cpp)

target_include_directories(${NAME}
        PUBLIC ${CMAKE_CURRENT_LIST_DIR}
        )

target_link_libraries(${NAME}
        Peripherals
        Services
        System
        )