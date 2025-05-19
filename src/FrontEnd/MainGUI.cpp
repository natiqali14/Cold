//
// Created by frio on 15/01/2025.
//

#include "MainGUI.h"

namespace Cold {
    MainGUI::MainGUI() {
    }

    void MainGUI::hide(bool flag) {
        b_should_hide = flag;
    }

    std::string MainGUI::get_model_path() {
        return model_path;
    }

    void MainGUI::render() {
        if (!b_should_hide) {
            main_holder->render();
        }
    }

    void MainGUI::set_frame_rate(const std::string &frame_rate) {
        frame_rate_text.get()->set_content(frame_rate);
    }

    void MainGUI::initialise() {
        main_holder = UIHolder::create_ui_holder_object({0, 0});
        model_path_input = std::make_shared<InputText>("Enter Model Path", 2);
        model_path_input_btn = std::shared_ptr<Button>
        (new Button([this]() {
            this->on_model_path_input_btn_click();
        }, "Add model", {100,20}));
        model_render_button = std::shared_ptr<Button>
        (new Button([](){}, "Render model", {100,20}));
        frame_rate_text = std::shared_ptr<Text>(new Text("60",2.0, {1,0,0,1} ));
        main_holder->add_component(frame_rate_text, 400);
        main_holder->add_component_in_new_row(model_path_input);

        main_holder->add_component_in_new_row(model_path_input_btn);
        main_holder->add_component(model_render_button);

        // drags
        drag_trans_x = std::shared_ptr<DragInput>(
            new DragInput(DragInput::EDI_FLOAT, 0, 100000, "t_x", 35 ));
        drag_trans_y = std::shared_ptr<DragInput>(
            new DragInput(DragInput::EDI_FLOAT, 0, 100000, "t_y", 35 ));
        drag_trans_z = std::shared_ptr<DragInput>(
            new DragInput(DragInput::EDI_FLOAT, 0, 100000, "t_z", 35 ));
        drag_scale = std::shared_ptr<DragInput>(
            new DragInput(DragInput::EDI_FLOAT, 0, 100000, "scale", 35 ));
        main_holder->add_component_in_new_row(drag_trans_x);
        main_holder->add_component(drag_trans_y);
        main_holder->add_component(drag_trans_z);
        main_holder->add_component_in_new_row(drag_scale);

    }

    void MainGUI::on_model_path_input_btn_click() {
        model_path = model_path_input.get()->get_text();
        auto x = drag_trans_x.get()->get_value();
        auto y = drag_trans_y.get()->get_value();
        auto z = drag_trans_z.get()->get_value();
        auto s = drag_scale.get()->get_value();
        backend_ptr->create_static_mesh(model_path, {x,y,z}, {s,s,s});
    }

    void MainGUI::on_model_render_btn_click() {
        //TODO
    }
}

