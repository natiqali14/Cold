//
// Created by frio on 06/12/2024.
//

#include "InputText.h"

#include <imgui.h>
#include <Logger.h>

namespace Cold {

    static int input_callback_wrapper(ImGuiInputTextCallbackData *data) {
        auto instance = static_cast<InputText*> (data->UserData);
        return instance->text_input_callback(data);
    }


    InputText::InputText(std::string label, u32 length, i32 flags)
        : input_label(std::move(label))
        , input_flags(flags)
        , text_length(length)
    {
        input_text.resize(text_length);
    }

    void InputText::render() {
        ImGui::InputText(input_label.c_str(), input_text.data(), input_text.size(), input_flags,
            input_callback_wrapper, this);
    }

    i32 InputText::text_input_callback(ImGuiInputTextCallbackData *data) {
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize && data->BufTextLen >= text_length - 1) {
            COLD_ERROR("Resize Callback Cold in TextINPUT %d", text_length);
            text_length *= 2;
            input_text.resize(text_length);
            data->Buf = input_text.data();
            data->BufSize = static_cast<i32>(text_length);
        }

        return 0;
    }

    std::string InputText::get_text() {
        return input_text;
    }
}
