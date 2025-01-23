//
// Created by frio on 05/12/2024.
//

#include "UIHolder.h"

#include <string>

namespace Cold {
    std::shared_ptr<UIHolder> UIHolder::create_ui_holder_object(const glm::vec2 &location, std::string title, i32 flags) {
        if (ui_holder_titles.count(title)) {
            ui_holder_titles[title]++;
            title+= "##" + std::to_string( ui_holder_titles[title]);
        }
        else {
            ui_holder_titles[title] = 0;
            title+= "##0";
        }

        return std::shared_ptr<UIHolder>( new UIHolder(location, std::move(title), flags));
    }

    UIHolder::UIHolder(const glm::vec2& location, std::string title, i32 flags)
    : window_title(std::move(title))
    , holder_location(location)
    , holder_flags(flags) {
        ui_components.emplace_back();
    }

    void UIHolder::add_component_in_row(iUIComponentSPtr component, u32 row_number, f32 spacing) {
        if (row_number <= ui_components.size()) {
            UIHolderProps props;
            props.ui = std::move(component);
            props.spacing_on_right_side = spacing;
            ui_components[row_number].push_back(std::move(props));
        }

    }

    void UIHolder::add_component(iUIComponentSPtr component, f32 spacing) {
        UIHolderProps props;
        props.ui = std::move(component);
        props.spacing_on_right_side = spacing;
        ui_components[ui_components.size() - 1].push_back(std::move(props));
    }

    void UIHolder::add_component_in_new_row(iUIComponentSPtr component, f32 spacing) {
        ui_components.emplace_back();
        UIHolderProps props;
        props.ui = std::move(component);
        props.spacing_on_right_side = spacing;
        ui_components[ui_components.size() - 1].push_back(std::move(props));
    }

    UIHolderProps UIHolder::replace_component(iUIComponentSPtr component_to_replace, const glm::u32vec2 &index, f32 new_spacing) {
        if (ui_components.size() <= index.x && ui_components[index.y].size() <= index.y) {
            auto replaced_component = ui_components[index.x][index.y];
            UIHolderProps props;
            props.ui = std::move(component_to_replace);
            props.spacing_on_right_side = new_spacing;
            ui_components[index.x][index.y] = std::move(props);
            return replaced_component;
        }
        return {};

    }

    void UIHolder::set_spacing(i32 row, i32 index, f32 new_spacing) {
        ui_components[row][index].spacing_on_right_side = new_spacing;
    }

    void UIHolder::render() {
        ImGui::SetNextWindowPos({holder_location.x, holder_location.y}, ImGuiCond_Always);
        ImGui::Begin(window_title.c_str(), nullptr, holder_flags);
        for (auto& ui_component_row : ui_components) {
            i32 index = 0;
            for (const auto&[ui, spacing_on_right_side] : ui_component_row) {
                ui->render();
                if (index < ui_component_row.size() - 1)
                    ImGui::SameLine(0.0f, spacing_on_right_side);
                index++;
            }
        }
        ImGui::End();
    }
}
