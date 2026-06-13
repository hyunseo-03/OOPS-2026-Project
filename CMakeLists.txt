cmake_minimum_required(VERSION 3.15)
project(ImGui_Project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui)

set(SOURCES
    src/main.cpp

    # Model
    src/model/Machine.cpp
    src/model/PrepMachine.cpp
    src/model/GrillMachine.cpp
    src/model/SauceMachine.cpp
    src/model/AssemblyMachine.cpp
    src/model/QualityChecker.cpp
    src/model/PackingMachine.cpp
    src/model/ProductionLine.cpp
    src/model/InventoryManager.cpp
    src/model/MoneyManager.cpp
    src/model/OrderManager.cpp
    src/model/BurgerFactoryModel.cpp

    # Controller
    src/controller/FactoryController.cpp

    # View
    src/view/FactoryView.cpp

    # ImGui
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

add_custom_command(TARGET imgui_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:imgui_app>/fonts"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${IMGUI_DIR}/misc/fonts/Roboto-Medium.ttf"
            "$<TARGET_FILE_DIR:imgui_app>/fonts/Roboto-Medium.ttf"
)

find_package(OpenGL REQUIRED)

if(APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)

    target_include_directories(imgui_app PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_directories(imgui_app PRIVATE ${SDL2_LIBRARY_DIRS})
    target_compile_options(imgui_app PRIVATE ${SDL2_CFLAGS_OTHER})
    target_link_libraries(imgui_app PRIVATE ${SDL2_LIBRARIES} OpenGL::GL)
else()
    find_package(SDL2 REQUIRED)

    if(WIN32)
        target_link_libraries(imgui_app PRIVATE SDL2::SDL2main)
        set_target_properties(imgui_app PROPERTIES WIN32_EXECUTABLE TRUE)
    endif()

    target_link_libraries(imgui_app PRIVATE
        SDL2::SDL2
        OpenGL::GL
    )
endif()
