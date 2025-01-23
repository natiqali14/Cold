//
// Created by frio on 05/12/2024.
//

#ifndef UIHOLDER_H
#define UIHOLDER_H

#include <memory>

#include "../i_UIComponent.h"
#include <vector>
#include <unordered_map>
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
    struct UIHolderProps {
        iUIComponentSPtr ui;
        f32 spacing_on_right_side {-1.0f};  // -1 means default spacing in im-gui.
    };
    class UIHolder : public i_UIComponent{

    public:
        static std::shared_ptr<UIHolder> create_ui_holder_object(const glm::vec2& location ,std::string title = "Window", i32 flags =
            UIHOLDER_NOMOVE | UIHOLDER_NOCOLLAPSE | UIHOLDER_AUTORESIZE | UIHOLDER_NOMOVE | UIHOLDER_NOBACKGROUND | UIHOLDER_NOTITLEBAR);
        explicit UIHolder(const glm::vec2& location, std::string title, i32 flags);
        void add_component_in_row(iUIComponentSPtr component, u32 row_number, f32 spacing = -1.0);
        void add_component(iUIComponentSPtr component, f32 spacing = -1.0);
        void add_component_in_new_row(iUIComponentSPtr component, f32 spacing = -1.0);
        UIHolderProps replace_component(iUIComponentSPtr component_to_replace, const glm::u32vec2& index,
                                            f32 new_spacing = -1.0);
        void set_spacing(i32 row, i32 index, f32 new_spacing);
        void render() override;
    private:
        std::vector<std::vector<UIHolderProps>> ui_components;
        glm::u32vec2 current_index {0,0};
        glm::vec2 holder_location;
        std::string window_title;
        i32 holder_flags {0};
        static inline std::unordered_map<std::string, u32> ui_holder_titles;
    };

    using UIHolderSPtr = std::shared_ptr<UIHolder>;
}



#endif //UIHOLDER_H
