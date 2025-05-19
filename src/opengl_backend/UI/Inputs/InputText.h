//
// Created by frio on 06/12/2024.
//

#ifndef INPUTTEXT_H
#define INPUTTEXT_H
#include <imgui.h>
#include <i_UIComponent.h>

#include <includes.h>
#include <Buttons/Button.h>

namespace Cold {
    enum InputTextFlag {
        // TODO fill it
        INPUT_TEXT_RESIZE = ImGuiInputTextFlags_CallbackResize
    };
    class InputText : public i_UIComponent{
    public:
        InputText(std::string label, u32 length = 8, i32 flags = INPUT_TEXT_RESIZE);
        ~InputText() override = default;
        void render() override;
        int text_input_callback(ImGuiInputTextCallbackData* data);
        std::string get_text();
    private:
        std::string input_text;
        std::string input_label;
        i32 input_flags;
        u32 text_length;
    };
}





#endif //INPUTTEXT_H
