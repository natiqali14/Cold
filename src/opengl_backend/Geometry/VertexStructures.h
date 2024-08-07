#pragma once
#include <includes.h>
namespace Cold {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normals;
        glm::vec2 tex_coord; 
    };

     struct Material { /* Just for now will move it to its on class and sub system*/
        std::string diff_tex;
        u32 diff_tex_id;
    };

}