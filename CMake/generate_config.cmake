function(generate_config)
    configure_file(CMake/Templates/hardware_config.cmake.in
            ${CMAKE_CURRENT_SOURCE_DIR}/CMake/hardware_config.cmake)

    message(FATAL_ERROR "New CMake/hardware_config.cmake was generated!\nReconfigure it and reload CMake.")
endfunction()

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/CMake/hardware_config.cmake)
    cmake_language(CALL generate_config)
else()
    include(CMake/hardware_config.cmake)

    file(READ ${CMAKE_CURRENT_SOURCE_DIR}/CMake/Templates/hardware_config.cmake.in VER)
    string(REGEX MATCH "# VERSION ([0-999]*)" _ ${VER})
    set(VER_TEMPLATE ${CMAKE_MATCH_1})

    file(READ ${CMAKE_CURRENT_SOURCE_DIR}/CMake/hardware_config.cmake VER)
    string(REGEX MATCH "# VERSION ([0-999]*)" _ ${VER})
    set(VER_GENERATED ${CMAKE_MATCH_1})

    if(${VER_TEMPLATE} GREATER ${VER_GENERATED})
        cmake_language(CALL generate_config)
    endif()

    configure_file(CMake/Templates/hardware_config.hpp.in
            ${CMAKE_CURRENT_SOURCE_DIR}/Inc/hardware_config.hpp)
endif()