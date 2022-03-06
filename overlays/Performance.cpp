#include "Performance.h"

#include "imgui.h"
#include "../config.h"

namespace overlays {
    Performance::Performance(int recordSeconds) {
        frametimes.resize(configTargetFps * recordSeconds);
        std::fill(frametimes.begin(), frametimes.end(), 0.0f);
        targetFrametime = (1 / configTargetFps) * 1000;
    }

    void Performance::add(float f) {
        frametimes.push_back(f);
        frametimes.pop_front();
    }

    void Performance::render() {
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
            }, &this->frametimes, frametimes.size(), 0, nullptr, 0, targetFrametime, {240, 60});
            ImGui::PopStyleColor();
            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Text("Fps: %.0f", ImGui::GetIO().Framerate);
        }
        ImGui::End();
    }
}
