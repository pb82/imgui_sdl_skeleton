#include <iostream>

#include <tuple>
#include <chrono>
#include <queue>

#include <SDL.h>
#include <SDL_image.h>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#include "config.h"

typedef std::tuple<SDL_Window *, SDL_Renderer *> Gfx;

Gfx initSdl() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        exit(1);
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        exit(1);
    }

    auto window = SDL_CreateWindow(
            "SDL + IMGui Skeleton",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            configWindowWidth,
            configWindowHeight,
            SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI
    );

    // Deal with high dpi displays where the scaled resolution is not the same as
    // the requested resolution
    int actualW, actualH;
    SDL_GL_GetDrawableSize(window, &actualW, &actualH);
    float scaleX = actualW / configWindowWidth;
    float scaleY = actualH / configWindowHeight;

    auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetScale(renderer, scaleX, scaleY);

    return {window, renderer};
}

void initImgui(Gfx gfx) {
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui::GetIO().IniFilename = nullptr;
    ImGui::GetIO().Fonts->AddFontFromFileTTF("./assets/font.ttf", 16);

    ImGui_ImplSDL2_InitForSDLRenderer(std::get<0>(gfx), std::get<1>(gfx));
    ImGui_ImplSDLRenderer_Init(std::get<1>(gfx));
}

void loop(Gfx gfx) {
    float targetFrametime = (1 / configTargetFps) * 1000;

    auto renderer = std::get<1>(gfx);
    auto window = std::get<0>(gfx);

    SDL_Event event;
    bool run = true;

    std::deque<float> frametimes;
    frametimes.resize(configTargetFps * 2);
    std::fill(frametimes.begin(), frametimes.end(), 0.0f);

    int r = 0, g = 0, b = 0;

    while (run) {
        auto frameStart = std::chrono::system_clock::now();

        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderClear(renderer);

        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) {
                run = false;
                break;
            }
        }

        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // SDL Draws
        {
        }

        // ImGui Draws
        {
            ImGui::SetNextWindowPos({0, 0});
            ImGui::Begin("Performance", nullptr,
                         ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoBackground |
                         ImGuiWindowFlags_NoDecoration);
            {
                ImGui::Text("Frametime");
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::PushStyleColor(ImGuiCol_PlotLines, {255, 0, 0, 255});
                ImGui::PlotLines("", [](void *d, int i) -> float {
                    auto ft = static_cast<std::deque<float> *>(d);
                    return ft->at(i);
                }, &frametimes, frametimes.size(), 0, nullptr, 0, targetFrametime, {240, 60});
                ImGui::PopStyleColor();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Text("Fps: %.0f", ImGui::GetIO().Framerate);
            }
            ImGui::End();
            ImGui::Begin("Clear color");
            ImGui::SliderInt("R", &r, 0, 255);
            ImGui::SliderInt("G", &g, 0, 255);
            ImGui::SliderInt("B", &b, 0, 255);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);

        // Frametime
        auto frameTime = std::chrono::system_clock::now() - frameStart;
        float millis = std::chrono::duration_cast<std::chrono::milliseconds>(frameTime).count();
        float delay = targetFrametime - millis;
        if (delay > 0) SDL_Delay(delay);
        frametimes.push_back(millis);
        frametimes.pop_front();
    }
}

void cleanup(Gfx gfx) {
    auto window = std::get<0>(gfx);
    auto renderer = std::get<1>(gfx);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main() {
    auto gfx = initSdl();
    initImgui(gfx);
    loop(gfx);
    cleanup(gfx);
    return 0;
}
