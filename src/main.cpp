#include <SDL.h>
#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#endif
#include <SDL_opengl.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include "model/BurgerFactoryModel.h"
#include "controller/FactoryController.h"
#include "view/FactoryView.h"

#include <filesystem>
#include <stdio.h>
#include <string>
#include <vector>

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

    std::vector<std::filesystem::path> fontPaths = {
        "fonts/Roboto-Medium.ttf",
        "libs/imgui/misc/fonts/Roboto-Medium.ttf",
        "../libs/imgui/misc/fonts/Roboto-Medium.ttf",
        "../../libs/imgui/misc/fonts/Roboto-Medium.ttf",
        "../Resources/Roboto-Medium.ttf",
        "C:/Windows/Fonts/segoeui.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/calibri.ttf",
        "/System/Library/Fonts/SFNS.ttf",
        "/System/Library/Fonts/SFNSMono.ttf",
        "/System/Library/Fonts/HelveticaNeue.ttc"
    };

    char* basePath = SDL_GetBasePath();
    if (basePath)
    {
        std::filesystem::path executableDir(basePath);
        fontPaths.insert(fontPaths.begin(), {
            executableDir / "fonts/Roboto-Medium.ttf",
            executableDir / "libs/imgui/misc/fonts/Roboto-Medium.ttf",
            executableDir / "../libs/imgui/misc/fonts/Roboto-Medium.ttf",
            executableDir / "../../libs/imgui/misc/fonts/Roboto-Medium.ttf",
            executableDir / "../Resources/Roboto-Medium.ttf"
        });
        SDL_free(basePath);
    }

    ImFont* mainFont = nullptr;
    for (const auto& path : fontPaths)
    {
        if (std::filesystem::exists(path))
        {
            mainFont = io.Fonts->AddFontFromFileTTF(path.string().c_str(), 15.0f);
            if (mainFont)
                break;
        }
    }

    if (!mainFont)
        io.Fonts->AddFontDefault();

    BurgerFactoryModel model;
    FactoryController  controller(model);
    FactoryView        view(model, controller);

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clearColor = ImVec4(0.08f, 0.08f, 0.13f, 1.0f);

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
