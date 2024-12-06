//
// Created by frio on 06/12/2024.
//

#include "Text.h"

#include <imgui.h>

namespace Cold {
    Text::Text(std::string text, f32 scale, const glm::vec4 &color)
    : text_color(color), content(std::move(text)), text_scale(scale)
    {
    }

    void Text::render() {
        // push color
        ImGui::PushStyleColor(ImGuiCol_Text,
            {text_color.r, text_color.g, text_color.b, text_color.a});
        // push font size
        auto current_font = ImGui::GetFont();
        current_font->Scale = text_scale;
        ImGui::PushFont(current_font);

        // Text
        ImGui::Text(content.c_str());

        // pop font size
        ImGui::PopStyleColor();
        ImGui::PopFont();


        current_font->Scale = 1.0;
        ImGui::PushFont(current_font);
        ImGui::PopFont();

    }

    void Text::set_content(std::string text) {
        content = std::move(text);
    }

    void Text::set_scale(f32 scale) {
        text_scale = scale;
    }

    glm::vec2 Text::get_text_size() {
        auto current_font = ImGui::GetFont();
        current_font->FontSize = text_scale;
        ImGui::PushFont(current_font);
        auto text_size =  ImGui::CalcTextSize(content.c_str());
        ImGui::PopFont();

        current_font->FontSize = 1.0;
        ImGui::PushFont(current_font);
        ImGui::PopFont();
        return {text_size.x, text_size.y};
    }

    void Text::set_color(const glm::vec4 &color) {
        text_color = color;
    }

}

