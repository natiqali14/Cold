//
// Created by frio on 17/01/2025.
//

#ifndef DRAGINPUT_H
#define DRAGINPUT_H


#include <includes.h>
#include <i_UIComponent.h>
namespace Cold {
    class DragInput : public Cold::i_UIComponent{
    public:
        enum EDragInput {
            EDI_INT,
            EDI_FLOAT,

        };
        // -1 size means default.
        DragInput(EDragInput input_type, f32 min, f32 max, std::string label, f32 size = -1);
        ~DragInput() override = default;
        void render() override;
        f32 get_value();
    private:
        std::string drag_label;
        f32 min_val;
        f32 max_val;
        EDragInput input;
        f32 ui_size;
        i32 data_int {0};
        f32 data_float {0};

        void render_drag();

    };
}




#endif //DRAGINPUT_H
