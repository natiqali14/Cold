//
// Created by frio on 05/12/2024.
//

#include "UIHolder.h"
namespace Cold {
    UIHolder::UIHolder(const glm::vec2& location, std::string title, i32 flags)
    : window_title(std::move(title))
    , holder_location(location)
    , holder_flags(flags) {
        ui_components.emplace_back();
    }

    void UIHolder::add_component_in_row(iUIComponentSPtr component, u32 row_number) {
        if (row_number <= ui_components.size()) {
            ui_components[row_number].push_back(std::move(component));
        }

    }

    void UIHolder::add_component(iUIComponentSPtr component) {
        ui_components[ui_components.size() - 1].push_back(std::move(component));
    }

    void UIHolder::add_component_in_new_row(iUIComponentSPtr component) {
        ui_components.emplace_back();
        ui_components[ui_components.size() - 1].push_back(std::move(component));
    }

    iUIComponentSPtr UIHolder::replace_component(iUIComponentSPtr component_to_replace, const glm::u32vec2 &index) {
        if (ui_components.size() <= index.x && ui_components[index.y].size() <= index.y) {
            auto replaced_component = ui_components[index.x][index.y];
            ui_components[index.x][index.y] = std::move(component_to_replace);
            return replaced_component;
        }
        return nullptr;

    }

    void UIHolder::render() {
        ImGui::SetNextWindowPos({holder_location.x, holder_location.y}, ImGuiCond_Always);
        ImGui::Begin(window_title.c_str(), nullptr, holder_flags);
        for (auto& ui_component_row : ui_components) {
            for (const auto& ui : ui_component_row) {
                ui->render();
            }
        }
        ImGui::End();
    }
}
