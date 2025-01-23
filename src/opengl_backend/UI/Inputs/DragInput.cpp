//
// Created by frio on 17/01/2025.
//

#include "DragInput.h"

#include <imgui.h>

namespace Cold {
    DragInput::DragInput(EDragInput input_type, f32 min, f32 max, std::string label, f32 size)
    : drag_label(std::move(label))
    , min_val(min)
    , max_val(max)
    , input(input_type)
    , ui_size(size) {

    }

    void DragInput::render() {
        if (ui_size > -0.5)
            ImGui::PushItemWidth(ui_size);
        render_drag();
        if (ui_size > -0.5)
            ImGui::PopItemWidth();

    }

    f32 DragInput::get_value() {
        if (input == EDI_INT) return data_int;
        else return data_float;
    }

    void DragInput::render_drag() {
        switch (input)
        {
            case EDI_INT:
            {
                ImGui::DragInt(drag_label.c_str(), &data_int, 1, min_val, max_val);
                break;
            }
            case EDI_FLOAT:
            {
                ImGui::DragFloat(drag_label.c_str(), &data_float, 0.1, min_val, max_val);
                break;
            }
        }
    }
}
