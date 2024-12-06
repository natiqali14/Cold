//
// Created by frio on 05/12/2024.
//

#ifndef UIHOLDER_H
#define UIHOLDER_H

#include <memory>

#include "../i_UIComponent.h"
#include <vector>
#include <includes.h>
#include <imgui.h>
namespace Cold {
    using iUIComponentSPtr = std::shared_ptr<i_UIComponent>;
    enum UIHolderFlag {
        UIHOLDER_NORESIZE = ImGuiWindowFlags_NoResize,
        UIHOLDER_NOCOLLAPSE = ImGuiWindowFlags_NoCollapse,
        UIHOLDER_AUTORESIZE = ImGuiWindowFlags_AlwaysAutoResize,
        UIHOLDER_NOMOVE = ImGuiWindowFlags_NoMove,
        UIHOLDER_NOBACKGROUND  = ImGuiWindowFlags_NoBackground,
        UIHOLDER_NOTITLEBAR = ImGuiWindowFlags_NoTitleBar

    };
    class UIHolder {
    public:
        explicit UIHolder(const glm::vec2& location ,std::string title = "NO NAME", i32 flags =
            UIHOLDER_NOMOVE | UIHOLDER_NOCOLLAPSE | UIHOLDER_AUTORESIZE | UIHOLDER_NOMOVE | UIHOLDER_NOBACKGROUND | UIHOLDER_NOTITLEBAR);
        void add_component_in_row(iUIComponentSPtr component, u32 row_number);
        void add_component(iUIComponentSPtr component);
        void add_component_in_new_row(iUIComponentSPtr component);
        iUIComponentSPtr replace_component(iUIComponentSPtr component_to_replace, const glm::u32vec2& index);
        void render();
    private:
        std::vector<std::vector<iUIComponentSPtr>> ui_components;
        glm::u32vec2 current_index {0,0};
        glm::vec2 holder_location;
        std::string window_title;
        i32 holder_flags {0};
    };
}



#endif //UIHOLDER_H
