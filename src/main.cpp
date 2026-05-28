/* CROSS-PLATFORM */
#include <SDL.h>
#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#endif
#include <SDL_opengl.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

// MVC 클래스 포함
#include "model/BurgerFactoryModel.h"
#include "controller/FactoryController.h"
#include "view/FactoryView.h"

// 폰트 아이콘 매크로
#include "IconsFontAwesome6.h"

#include <stdio.h>

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("SDL Error: %s\n", SDL_GetError());
        return -1;
    }

    const char* glsl_version;
#if defined(__APPLE__)
    glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow(
        "Burger Factory Simulation",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 720, window_flags);

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // ============================================================
    // 폰트 설정
    // 1) Roboto-Medium (이미 libs에 포함) → 기본 텍스트 폰트
    // 2) fa-solid-900.ttf → Roboto에 merge (아이콘 글리프 추가)
    // ============================================================

    // [1] Roboto 로드 (기본 폰트)
    io.Fonts->AddFontFromFileTTF(
        "../libs/imgui/misc/fonts/Roboto-Medium.ttf", 15.0f);

    // [2] Font Awesome를 Roboto에 merge
    //     MergeMode=true → 앞서 로드한 폰트에 아이콘 글리프를 추가
    ImFontConfig faConfig;
    faConfig.MergeMode        = true;
    faConfig.GlyphMinAdvanceX = 14.0f; // 아이콘 최소 너비 (줄 맞춤)
    faConfig.GlyphOffset      = ImVec2(0.0f, 2.0f); // 세로 위치 미세조정
    static const ImWchar fa_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

    io.Fonts->AddFontFromFileTTF(
        "../libs/fonts/fa-solid-900.ttf", 14.0f, &faConfig, fa_ranges);

    // [3] 큰 아이콘 전용 폰트 (기계 카드 중앙 아이콘용)
    ImFontConfig faLargeConfig;
    faLargeConfig.GlyphMinAdvanceX = 28.0f;
    ImFont* largeIconFont = io.Fonts->AddFontFromFileTTF(
        "../libs/fonts/fa-solid-900.ttf", 30.0f, &faLargeConfig, fa_ranges);

    // ============================================================
    // MVC 객체 생성
    // ============================================================
    BurgerFactoryModel model;
    FactoryController  controller(model);
    FactoryView        view(model, controller);

    view.setLargeIconFont(largeIconFont); // 큰 아이콘 폰트 전달
    view.setupStyle();

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clearColor = ImVec4(0.08f, 0.08f, 0.13f, 1.0f);

    // ============================================================
    // 게임 루프
    // ============================================================
    Uint64 lastTime = SDL_GetPerformanceCounter();
    bool   running  = true;

    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) running = false;
        }

        Uint64 now = SDL_GetPerformanceCounter();
        float  dt  = (float)(now - lastTime) / (float)SDL_GetPerformanceFrequency();
        lastTime   = now;

        controller.update(dt);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        view.render();

        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
