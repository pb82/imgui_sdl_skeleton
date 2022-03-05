cmake_minimum_required(VERSION 3.21)
project(skeleton)

INCLUDE(FindPkgConfig)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${skeleton_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 20)

add_executable(
        skeleton
        main.cpp
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_impl_sdl.cpp
        imgui/imgui_impl_sdlrenderer.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
)

target_include_directories(
        skeleton
        PRIVATE ${skeleton_SOURCE_DIR}/imgui
        PRIVATE ${skeleton_SOURCE_DIR}/sdl/linux
)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

target_link_libraries(
        skeleton
        PRIVATE ${SDL2_LIBRARY}
        PRIVATE ${SDL2_IMAGE_LIBRARIES}
)