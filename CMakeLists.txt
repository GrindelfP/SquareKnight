cmake_minimum_required(VERSION 3.27)
project(SquareKnight)

set(CMAKE_APPLE_SILICON_PROCESSOR "arm64")
set(CMAKE_OSX_ARCHITECTURES "arm64")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
find_package(SFML 3 COMPONENTS Window Graphics System Audio REQUIRED)

add_executable(${PROJECT_NAME} MACOSX_BUNDLE
        src/main.cc
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

set(SFML_BREW_PATH "/opt/homebrew/opt/sfml/lib")
set(APP_BUNDLE "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app")
set(FRAMEWORKS_DIR "${APP_BUNDLE}/Contents/Frameworks")
set(EXECUTABLE "${APP_BUNDLE}/Contents/MacOS/${PROJECT_NAME}")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${APP_BUNDLE}/Contents/Frameworks"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets" "${APP_BUNDLE}/Contents/Resources/assets"
        COMMAND dylibbundler -od -b -x "${EXECUTABLE}" -d "${APP_BUNDLE}/Contents/Frameworks/" -p "@executable_path/../Frameworks/"
        COMMAND codesign --force --deep --sign - "${APP_BUNDLE}"
        COMMENT "Bundling dependencies for ${PROJECT_NAME}..."
)
