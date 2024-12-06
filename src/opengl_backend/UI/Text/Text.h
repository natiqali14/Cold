//
// Created by frio on 06/12/2024.
//

#ifndef TEXT_H
#define TEXT_H


#include "i_UIComponent.h"
#include <string>
#include <includes.h>
namespace Cold {
    class Text : public i_UIComponent{
    public:
        Text(std::string text, f32 scale, const glm::vec4& color);
        ~Text() override = default;
        void render() override;
        void set_content(std::string text);
        void set_scale(f32 scale);
        glm::vec2 get_text_size();
        void set_color(const glm::vec4& color);
    private:
        glm::vec4 text_color;
        std::string content;
        f32 text_scale {0.0f};
    };

}




#endif //TEXT_H
