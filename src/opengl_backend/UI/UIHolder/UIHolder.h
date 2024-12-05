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
        explicit UIHolder(i32 flags =
            UIHOLDER_NOMOVE | UIHOLDER_NOCOLLAPSE | UIHOLDER_AUTORESIZE | UIHOLDER_NOMOVE | UIHOLDER_NOBACKGROUND | UIHOLDER_NOTITLEBAR);
        void add_component_in_row(iUIComponentSPtr component, u32 row_number);
        void add_component(iUIComponentSPtr component);
        void add_component_in_new_row(iUIComponentSPtr component);
        void replace_component(iUIComponentSPtr component_to_replace, const glm::u32vec2& index, bool should_delete_old);
        void render();
    private:
        std::vector<std::vector<iUIComponentSPtr>> ui_components;
        glm::u32vec2 current_index {0,0};
        i32 holder_flags {0};
    };
}



#endif //UIHOLDER_H
