#version 330 core

#define USE_PHONG 1

out vec4 FragColor;

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

in vec2 tc;
in fragment_data_object out_fdb;
uniform sampler2D frameTexture[3];
uniform float shininess;
uniform int hasNormal;
const int DIFFUSE = 0;
const int SPECULAR = 1;
const int NORMAL = 2;

vec4 calculate_color_with_directional_light(vec4 color, vec3 normal)
{
    vec4 spec_color = vec4(0.0);
    if(shininess > 0.0)
    {
        vec3 eye = normalize(out_fdb.camera_position - out_fdb.frag_position);
#if USE_PHONG

        // for now 0 shininess mean no specular light
        
        vec3 reflection = normalize(reflect(-out_fdb.directional_light_vector, normalize(normal)));
        float spec = pow(max(dot(eye, reflection), 0.0), shininess);
        spec_color = vec4(vec3(out_fdb.directional_light_color * spec), color.a);
        spec_color *= vec4(texture(frameTexture[SPECULAR], tc).rgb, color.a);
#else
        vec3 half_dir = normalize( out_fdb.directional_light_vector  + eye);
        float spec = pow(max(dot(half_dir, normal),0.0), shininess);
        spec_color = vec4(vec3(out_fdb.directional_light_color * spec), color.a);
        spec_color *= vec4(texture(frameTexture[SPECULAR], tc).rgb, color.a);

#endif
    }
    float diffuse_factor = max(dot(normalize(normal) , (out_fdb.directional_light_vector)), 0.0);
    vec3 directional_light_color = out_fdb.directional_light_color * diffuse_factor;
    vec3 ambience = out_fdb.ambient_color * out_fdb.diffuse_color;

    vec4 new_color = color * vec4(vec3(directional_light_color), color.a);
    vec4 a = vec4(vec3(ambience * color.rgb),color.a);
    return new_color + a + spec_color;
}

// main
void main()
{
    vec3 normal_map;
    vec3 new_normal;
    if(hasNormal > 0) {
        normal_map = 2.0 * texture(frameTexture[NORMAL], tc).rgb - 1.0;
        new_normal = normalize(out_fdb.TBN * normal_map);
    }
    else {
        new_normal = normalize(out_fdb.normal);
    }
    vec4 col = texture(frameTexture[DIFFUSE], tc);
    FragColor = calculate_color_with_directional_light(col, new_normal);
}

