cmake_minimum_required(VERSION 3.15)
project(ImGui_Project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui)

# [오른쪽 구조 반영] 컴파일할 모든 소스 파일 리스트
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

    # ImGui 핵심 소스 및 백엔드
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_executable(imgui_app ${SOURCES})

# OpenGL 패키지 찾기
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

    target_include_directories(imgui_app PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
        ${SDL2_INCLUDE_DIRS}
    )
    target_link_directories(imgui_app PRIVATE ${SDL2_LIBRARY_DIRS})
    target_compile_options(imgui_app PRIVATE ${SDL2_CFLAGS_OTHER})
    target_link_libraries(imgui_app PRIVATE ${SDL2_LIBRARIES} OpenGL::GL)
else()
    # Windows (MSYS2 UCRT64) 및 Linux 환경
    find_package(SDL2 REQUIRED)

    # 헤더 파일 참조 경로 설정
    target_include_directories(imgui_app PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
        ${SDL2_INCLUDE_DIRS} # 💡 [해결책] MSYS2에서 SDL.h를 바로 찾을 수 있도록 경로 강제 주입!
    )

    if(WIN32)
        # 💡 MSYS2 환경에서 SDL2 메인 함수 매핑 및 컴파일러 링크 에러 방지
        target_link_libraries(imgui_app PRIVATE mingw32 SDL2::SDL2main)
        
        # 💡 [참고] 콘솔 창을 띄워서 std::cout 같은 디버깅 메시지를 보고 싶다면 아래 줄을 주석(#) 처리하세요.
        # set_target_properties(imgui_app PROPERTIES WIN32_EXECUTABLE TRUE)
    endif()

    # 최종 라이브러리 링크
    target_link_libraries(imgui_app PRIVATE
        SDL2::SDL2
        OpenGL::GL
    )
endif()