#pragma once
#include <includes.h>
#include <unordered_map>
namespace Cold {
    using ShaderId = u32;
    struct Shader{};
    class ShaderSystem {
    public:
        void initiate();
        void shut_down();
        ShaderId create_shader();
        void compile_shaders(ShaderId shader_id, const char* vertex_shader, const char* fragment_shader);
        
    private:
        std::unordered_map<ShaderId, Shader> shaders_data;
    };
}