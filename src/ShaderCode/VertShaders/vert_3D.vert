#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
uniform mat4 model;
uniform vec3 diffuse_color;

layout(std140) uniform GlobalUniformObject {
    mat4 projection;
    mat4 view_model;
    vec3 ambient_color;
    vec3 directional_light_color;
    vec3 directional_light_vector;
};

struct fragment_data_object {
    vec3 directional_light_color;
    vec3 directional_light_vector;
    vec3 normal;
    vec3 ambient_color;
    vec3 diffuse_color;
};

out vec2 tc;
out fragment_data_object out_fdb;
void main() 
{
    out_fdb.ambient_color = ambient_color;
    out_fdb.directional_light_color = directional_light_color;
    out_fdb.directional_light_vector = directional_light_vector;
    out_fdb.normal = mat3(model) * aNormal;
    out_fdb.diffuse_color = diffuse_color;
    gl_Position = projection * view_model * model * vec4(aPos, 1.0);
    tc = aTexCoord;
}
