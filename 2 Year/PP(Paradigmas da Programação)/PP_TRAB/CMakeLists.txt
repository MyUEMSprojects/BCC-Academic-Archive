cmake_minimum_required(VERSION 3.0)

project(hipo)

# Set the source directory
set(SOURCE_DIR ${CMAKE_SOURCE_DIR})

# Include directories
include_directories(${SOURCE_DIR}/include)

# Source files
set(SOURCES
    ${SOURCE_DIR}/src/main.cpp
    ${SOURCE_DIR}/src/Interface.cpp
    ${SOURCE_DIR}/src/Memory.cpp
    ${SOURCE_DIR}/src/CPU.cpp
    # Add all your source files here
)

# Create an executable
add_executable(hipo ${SOURCES})

# Link the SFML and any other necessary libraries
target_link_libraries(hipo sfml-graphics sfml-window sfml-system)

# Copy resource files (e.g., PNG, font, .hipo) to the build directory
configure_file(${SOURCE_DIR}/Textures/background-interface.png ${CMAKE_CURRENT_BINARY_DIR}/Textures/background-interface.png COPYONLY)
configure_file(${SOURCE_DIR}/Fonts/Inter-Regular.ttf ${CMAKE_CURRENT_BINARY_DIR}/Fonts/Inter-Regular.ttf COPYONLY)
configure_file(${SOURCE_DIR}/files/programa.hipo ${CMAKE_CURRENT_BINARY_DIR}/files/programa.hipo COPYONLY)
configure_file(${SOURCE_DIR}/files/basic_sum.hipo ${CMAKE_CURRENT_BINARY_DIR}/files/basic_sum.hipo COPYONLY)
configure_file(${SOURCE_DIR}/files/seq_sum.hipo ${CMAKE_CURRENT_BINARY_DIR}/files/seq_sum.hipo COPYONLY)

# Pass the source directory to the executable during build
target_compile_definitions(hipo PRIVATE SOURCE_DIR="${SOURCE_DIR}")