cmake_minimum_required(VERSION 3.27)
project(SquareKnight)

set(CMAKE_APPLE_SILICON_PROCESSOR "arm64")
set(CMAKE_OSX_ARCHITECTURES "arm64")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
find_package(SFML 3 COMPONENTS Window Graphics System Audio REQUIRED)

add_executable(${PROJECT_NAME} MACOSX_BUNDLE
        src/main.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        INSTALL_RPATH "@executable_path/../Frameworks"
        BUILD_WITH_INSTALL_RPATH TRUE
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER "to.grindelf.squareknight"
        MACOSX_BUNDLE_BUNDLE_NAME "Square Knight"
)

target_link_libraries(${PROJECT_NAME}
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
)

set(SFML_LIB_DIR "/opt/homebrew/lib")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app/Contents/Frameworks"

        COMMAND ${CMAKE_COMMAND} -E copy
        "${SFML_LIB_DIR}/libsfml-graphics.3.0.dylib"
        "${SFML_LIB_DIR}/libsfml-window.3.0.dylib"
        "${SFML_LIB_DIR}/libsfml-system.3.0.dylib"
        "${SFML_LIB_DIR}/libsfml-audio.3.0.dylib"
        "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app/Contents/Frameworks"

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app/Contents/Resources/assets"
)