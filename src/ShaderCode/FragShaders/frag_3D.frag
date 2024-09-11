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

struct point_light {
    vec3 position;
    vec3 color;
    float linear;
    float quadratic;
    float constant;
};

in vec2 tc;
in fragment_data_object out_fdb;
in point_light p0;
uniform sampler2D frameTexture[3];
uniform float shininess;
uniform int hasNormal;
const int DIFFUSE = 0;
const int SPECULAR = 1;
const int NORMAL = 2;


vec4 calculate_point_light(point_light p, vec4 sample_color) {
    vec3 light_direction = normalize(p.position - out_fdb.frag_position);
    vec3 eye = normalize(out_fdb.camera_position - out_fdb.frag_position);
    // first diffusion lighting
    float diff_factor = dot(light_direction, out_fdb.normal);
    vec3 diff_light = p.color * diff_factor;
    vec4 final_diff_color = vec4(vec3(diff_light * sample_color.rgb), sample_color.a);

    vec4 spec_color = vec4(0);
    //second specular lighing
    if (shininess > 0.0) {
        vec3 reflection = reflect(-light_direction, out_fdb.normal);
        float spec_factor = pow(max(dot(reflection, eye), 0),shininess);
        vec3 spec_map= (texture(frameTexture[SPECULAR], tc).rgb);
        spec_map *= spec_factor;
        spec_color = vec4(vec3(spec_map * sample_color.rgb), sample_color.a);
    }

    vec4 amb = vec4(vec3(out_fdb.ambient_color), 1.0) * sample_color;

    vec4 final =  amb + final_diff_color + spec_color;

    float distance_ = length(p0.position - out_fdb.frag_position);
    float attenuation = 1.0 / (p0.constant + (p0.linear * distance_) + (p0.quadratic * (distance_ * distance_)));
    return final * attenuation;

    
}

vec4 calculate_color_with_directional_light(vec4 color, vec3 normal)
{
   // color.rgb *= out_fdb.diffuse_color;
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
    vec3 ambience = out_fdb.ambient_color;

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
    vec4 p0_light = calculate_point_light(p0, col);
    vec4 directional =  calculate_color_with_directional_light(col, new_normal);
    FragColor = p0_light + directional;
}

