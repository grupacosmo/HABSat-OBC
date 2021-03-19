add_subdirectory(Utils)
add_subdirectory(System)
add_subdirectory(Interrupt_Handlers)
add_subdirectory(Peripherals)
add_subdirectory(Include_Files)
add_subdirectory(External_Libs)
add_subdirectory(OBC)
add_subdirectory(MCU)
add_subdirectory(Services)

set(NAME Main_Loop)

add_library(${NAME} STATIC mainLoop.cpp)

target_include_directories(${NAME}
        PUBLIC ${CMAKE_CURRENT_LIST_DIR}
        )

target_link_libraries(${NAME}
        FreeRTOS
        System
        OBC
        Interrupt_Handlers)