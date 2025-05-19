//
// Created by frio on 15/01/2025.
//

#ifndef MAINGUI_H
#define MAINGUI_H

#include <includes.h>
#include <RendererBackend.h>
#include <Inputs/DragInput.h>
#include <Inputs/InputText.h>
#include <Text/Text.h>

#include "../opengl_backend/UI/UIHolder/UIHolder.h"
namespace Cold {
    class MainGUI {
    public:
        MainGUI();
        void initialise();
        void hide(bool flag);
        std::string get_model_path();
        void render();
        void set_frame_rate(const std::string& frame_rate);
        void set_spacing_for_model_input_button();

        RendererBackend* backend_ptr;  // Find some good practice here rather then a ptr.

    private:
        UIHolderSPtr main_holder;
        bool b_should_hide {false};
        std::shared_ptr<InputText> model_path_input;
        std::shared_ptr<Button> model_path_input_btn;
        std::shared_ptr<Text> frame_rate_text;
        std::shared_ptr<Button> model_render_button;
        std::shared_ptr<DragInput> drag_trans_x, drag_trans_y, drag_trans_z, drag_scale;
        std::string model_path;

        // functions
        void on_model_path_input_btn_click();
        void on_model_render_btn_click();
    };
}




#endif //MAINGUI_H
