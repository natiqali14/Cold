#version 330 core
out vec4 FragColor;

struct fragment_data_object {
    vec3 directional_light_color;
    vec3 directional_light_vector;
    vec3 normal;
    vec3 ambient_color;
    vec3 diffuse_color;
};

in vec2 tc;
in fragment_data_object out_fdb;
uniform sampler2D frameTexture;

vec4 calculate_color_with_directional_light(vec4 color)
{
    float diffuse_factor = max(dot(normalize(out_fdb.normal) , (out_fdb.directional_light_vector)), 0.0);
    vec3 directional_light_color = out_fdb.directional_light_color * diffuse_factor * 1.3;
    vec3 ambience = out_fdb.ambient_color * out_fdb.diffuse_color;

    vec4 new_color = color * vec4(vec3(directional_light_color), color.a);
    vec4 a = vec4(vec3(ambience * color.rgb),color.a);
    return new_color + a;
}

// main
void main()
{
    vec4 col = texture(frameTexture, tc);
    FragColor = calculate_color_with_directional_light(col);
}

