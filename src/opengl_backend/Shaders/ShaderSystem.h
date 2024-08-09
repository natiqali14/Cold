#pragma once
#include <includes.h>
#include <unordered_map>
#include <any>
namespace Cold {
    using ShaderId = u32;
    struct Shader{
        u32 program_id;

    };

    enum EUniformType {
        EUT_FLOAT_1,
        EUT_FLOAT_2,
        EUT_FLOAT_3,
        EUT_FLOAT_4,
        EUT_INT_1,
        EUT_INT_2,
        EUT_INT_3,
        EUT_INT_4,
        EUT_MAT_4x4,
    };

    class ShaderSystem {
    public:
        static void initiate();
        static void shut_down();
        static void set_global_uniform_buffer_object();
        static ShaderId create_shader();
        static void compile_shaders(ShaderId shader_id, const char* vertex_shader, const char* fragment_shader);
        static void pass_uniform(ShaderId shader_id, std::any value);
        
    private:
        std::unordered_map<ShaderId, Shader> shaders_data;
    };
}