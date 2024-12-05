//
// Created by natiq on 04/12/2024.
//

#ifndef BUTTON_H
#define BUTTON_H

#include <includes.h>
#include <imgui.h>
#include <functional>
#include "../i_UIComponent.h"
namespace Cold {
    class Button : public i_UIComponent{
    public:
        explicit Button(const std::function<void()>& ftn_ptr_on_click, std::string label, const glm::vec2& size);
        Button(Button&& object) noexcept ;
        ~Button() override;
        void render() override;
        void set_on_click(const std::function<void()>& ftn_ptr);
        void set_button_enable(bool enable);

        Button& operator=(Button&& object) noexcept ;
    private:
        std::function<void()> on_click;
        std::string button_label;
        bool is_enable {true};
        glm::vec2 button_size{};


    };
}



#endif //BUTTON_H
