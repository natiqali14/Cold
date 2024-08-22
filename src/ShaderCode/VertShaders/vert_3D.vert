#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
uniform mat4 model;
uniform vec3 diffuse_color;

layout(std140) uniform GlobalUniformObject {
    mat4 projection;
    mat4 view_model;
    vec3 camera_position;
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
    vec3 camera_position;
    vec3 frag_position;
    mat3 TBN;
};

out vec2 tc;
out fragment_data_object out_fdb;
void main() 
{
    vec3 frag  = vec3(model * vec4(aPos, 1.0));
    out_fdb.ambient_color = ambient_color;
    out_fdb.directional_light_color = directional_light_color;
    out_fdb.directional_light_vector = directional_light_vector;
    mat3 model_mat = mat3(model);
    out_fdb.normal = normalize(model_mat * aNormal);
    vec3 norm_tangent = normalize(model_mat * aTangent);
    vec3 norm_bitangent = normalize(model_mat * aBitangent);
    norm_tangent = normalize((norm_tangent - dot(norm_tangent, out_fdb.normal) * out_fdb.normal));
    out_fdb.TBN = mat3(norm_tangent, norm_bitangent, out_fdb.normal);

    out_fdb.diffuse_color = diffuse_color;
    out_fdb.camera_position = camera_position;
    out_fdb.frag_position = frag;
    gl_Position = projection * view_model * model * vec4(aPos, 1.0);
    tc = aTexCoord;
}
