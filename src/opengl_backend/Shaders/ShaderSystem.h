#pragma once
#include <includes.h>
#include <unordered_map>
#include <any>
namespace Cold {
    using ShaderId = u32;
    struct UniformObject {
        std::any value;
        EUniformType tpye;
    };

    struct UniformBuffer {
        struct SubDataBuffering {
            void* sub_data;
            u32 offset;
            u32 size;
        };
        void* data;  // TODO right now saving the pointer to data here. No idea how to make it better
        u32 size;
        u32 uniform_buffer_u_id {0};
        std::vector<SubDataBuffering> data_to_update; // this vector used to pass sub data on buffer update.
        bool should_pass_data {true};

        void add_sub_data_to_pass_to_gpu(void* sub_data, u32 size, u32 offset) {
            data_to_update.push_back({ sub_data, offset, size });
        }
    };
    using UniformBufferSPtr = std::shared_ptr<UniformBuffer>;

    struct Shader{
        u32 program_id;
        // data of uniforms to a shader has.
        std::unordered_map<std::string, UniformObject> uniform_objects;
        // data that needs to be passed to gpu
        std::vector<std::string> uniforms_to_pass_to_gpu;

        // unifrom buffers to pass to the shader
        std::unordered_map<u32, UniformBufferSPtr> uniform_buffer_objects;
        // uniform buffers to pass to GPU
        std::vector<u32> uniform_buffers_to_pass_to_gpu;

        const char* vertex_shader;
        const char* fragment_shader;  // TODO right now only vertex and fragment shaders




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
        static void global_uniform_buffer_object_add(void* data, u32 size, const std::string& name);
        static void global_uniform_buffer_object_update(const std::string& name, void* sub_data, u32 size, u32 offset);
        static void global_uniform_buffer_objects_pass_to_gpu();
        static ShaderId create_shader();
        static void shaders_compile(ShaderId shader_id, const char* vertex_shader, const char* fragment_shader);
        static void uniform_object_add(ShaderId shader_id, const std::string& uniform, const std::any& value);
        static void uniform_object_update(ShaderId shader_id, const std::string& uniform, const std::any& value);
        static void uniform_objects_pass_to_gpu(ShaderId shader_id);
        static void uniform_buffer_object_add(ShaderId shader_id, void* data, u32 size, const std::string& name);
        static void uniform_buffer_object_update(ShaderId shader_id, void* sub_data, u32 size, u32 offset);
        static void uniform_buffer_objects_pass_to_gpu(ShaderId shader_id);
        
    private:
        std::unordered_map<ShaderId, Shader> shaders_data;
        // unifrom buffers to pass to the shader
        std::unordered_map<std::string, UniformBufferSPtr> global_uniform_buffer_objects;
        // uniform buffers to pass to GPU
        std::vector<std::string> global_uniform_buffers_to_pass_to_gpu;
    };
}