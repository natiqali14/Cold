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

    struct CobbleStoneData {
        std::string diffuse = "Assets/textures/cobblestone.png";
        std::string normal = "Assets/textures/cobblestone_NRM.png";
        std::string specular = "Assets/textures/cobblestone_SPEC.png";
        f32 shininess = 2.0f; 
    };
    
    inline DefaultData default_data;
    inline GlobalData global_data;
    inline CobbleStoneData cobble_stone;
}
