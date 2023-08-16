cmake_minimum_required(VERSION 3.24)
project(sfml_sorting LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(SFML_VERSION 2.6.0)
set(IMGUI_VERSION 1.87)
set(IMGUI_SFML_VERSION 2.5)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
        imgui
        URL "https://github.com/ocornut/imgui/archive/v${IMGUI_VERSION}.zip"
)
FetchContent_MakeAvailable(imgui)

FetchContent_Declare(
        imgui-sfml
        URL "https://github.com/SFML/imgui-sfml/archive/v${IMGUI_SFML_VERSION}.zip"
)
set(IMGUI_DIR ${imgui_SOURCE_DIR})
option(IMGUI_SFML_FIND_SFML "Use find_package to find SFML" OFF)
option(IMGUI_SFML_IMGUI_DEMO "Build imgui_demo.cpp" OFF)
FetchContent_MakeAvailable(imgui-sfml)

add_executable(sfml_sorting main.cpp ./src/gtime.cpp)
target_link_libraries(sfml_sorting PRIVATE sfml-graphics)
target_link_libraries(sfml_sorting PUBLIC ImGui-SFML::ImGui-SFML)

target_compile_features(sfml_sorting PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET sfml_sorting POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:sfml_sorting> $<TARGET_FILE_DIR:sfml_sorting> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS sfml_sorting)