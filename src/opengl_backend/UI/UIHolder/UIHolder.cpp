//
// Created by frio on 05/12/2024.
//

#include "UIHolder.h"
namespace Cold {
    UIHolder::UIHolder(i32 flags) : holder_flags(flags) {
    }

    void UIHolder::add_component_in_row(iUIComponentSPtr component, u32 row_number) {
    }

    void UIHolder::add_component(iUIComponentSPtr component) {
    }

    void UIHolder::add_component_in_new_row(iUIComponentSPtr component) {
    }

    void UIHolder::replace_component(iUIComponentSPtr component_to_replace, const glm::u32vec2 &index,
        bool should_delete_old) {
    }

    void UIHolder::render() {
    }
}
