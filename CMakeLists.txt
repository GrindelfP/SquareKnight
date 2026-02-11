cmake_minimum_required(VERSION 3.27)                                  # CMake version
project(SquareKnight)                                                 # Project name
set(CMAKE_APPLE_SILICON_PROCESSOR "arm64")                            # Architecture
set(CMAKE_OSX_ARCHITECTURES "arm64")
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")                        # Path to libs
find_package(SFML 3 COMPONENTS Window Graphics System Audio REQUIRED) # SFML pointer
add_executable(
    main 
    src/main.cpp
)                                                                     # Executable
target_link_libraries(main 
        SFML::Graphics 
        SFML::Window 
        SFML::System
        SFML::Audio
)                                                                     # Lib linking
add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "${CMAKE_BINARY_DIR}/assets"
)