#pragma once
#include <includes.h>

namespace Cold {
    struct DefaultData {
        std::string default_texture_path = "Assets/default.png";
        std::string default_shader_name = "DefaultShader";
    };

    struct GlobalData {
        std::string global_uniform_buffer_name = "GlobalUniformObject";
    };
    
    inline DefaultData default_data;
    inline GlobalData global_data;
}
