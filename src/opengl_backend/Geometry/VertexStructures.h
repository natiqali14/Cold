#pragma once
#include <includes.h>
namespace Cold {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normals;
        glm::vec2 tex_coord; 
        glm::vec3 tangents;
        glm::vec3 bitangents;
    };

     struct Material { /* Just for now will move it to its on class and sub system*/
        std::string diff_tex;
        u32 diff_tex_id;
        glm::vec3 diffuse_color {1.0,1.0,1.0}; // by default white

        std::string specular_texure {""};
        u32 specular_texture_id;
        f32 shininess{0};

        std::string normal_texture {""};
        u32 normal_texture_id;
    };

}