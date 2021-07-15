set(NAME Gps)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR} SOURCE)

add_library(${NAME} STATIC ${SOURCE} Inc/Gps/gpsMessages.hpp gpsMessages.cpp)

target_include_directories(${NAME} PUBLIC Inc)

target_link_libraries(${NAME}
        Buses
        )