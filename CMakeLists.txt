cmake_minimum_required(VERSION 3.15)
project(ImGui_Project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui)

include_directories(include)

set(SOURCES 
    src/main.cpp
    src/model/GrillMachine.cpp
    src/model/PackingMachine.cpp
    src/model/PrepMachine.cpp
    src/model/SauceMachine.cpp

    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_executable(imgui_app ${SOURCES})

target_include_directories(imgui_app PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(imgui_app PRIVATE 
    SDL2::SDL2
    OpenGL::GL
)

if(WIN32)
    target_link_libraries(imgui_app PRIVATE SDL2::SDL2main)
    set_target_properties(imgui_app PROPERTIES WIN32_EXECUTABLE TRUE)
endif()