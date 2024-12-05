//
// Created by natiq on 04/12/2024.
//

#include "Button.h"

#include <utility>
namespace Cold {
    Button::Button(const std::function<void()> &ftn_ptr_on_click, std::string label, const glm::vec2& size)
        : on_click(ftn_ptr_on_click) , button_label(std::move(label)), button_size(size) {}

    Button::Button(Button &&object) noexcept {
        this->button_size = object.button_size;
        this->button_label = std::move(object.button_label);
        this->on_click = std::move(on_click);
        this->is_enable = object.is_enable;
    }

    Button::~Button() = default;

    void Button::render() {
        if (is_enable && on_click
        && ImGui::Button(button_label.c_str(), {button_size.x, button_size.y})) {
            on_click();
        }
    }

    void Button::set_on_click(const std::function<void()> &ftn_ptr) {
        on_click = ftn_ptr;
    }

    void Button::set_button_enable(bool enable) {
        is_enable = enable;
    }

    Button & Button::operator=(Button &&object) noexcept {
        this->button_size = object.button_size;
        this->button_label = std::move(object.button_label);
        this->on_click = std::move(on_click);
        this->is_enable = object.is_enable;
        return *this;
    }
}

