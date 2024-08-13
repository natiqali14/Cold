#pragma once
#include <includes.h>
#include <unordered_map>
#include <any>
#include <Logger.h>
#include <ShaderGetter.h>
namespace Cold {

    enum EUniformType {
        EUT_FLOAT_1,
        EUT_FLOAT_2,
        EUT_FLOAT_3,
        EUT_FLOAT_4,
        EUT_INT_1,
        EUT_MAT_3x3,
        EUT_MAT_4x4,
    };

    using ShaderId = const std::string&;
    struct UniformObject {
        std::any value;
        EUniformType type;
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
        u32 current_binding_point {0};
        void add_sub_data_to_pass_to_gpu(void* sub_data, u32 size, u32 offset) {
            data_to_update.push_back({ sub_data, offset, size });
        }

        ~UniformBuffer() {
            COLD_TRACE("Uniform Buffer Object Destroyed");
            for (auto& s_data : data_to_update) {
                free(s_data.sub_data);
            }
            free(data);
        }
    };
    using UniformBufferSPtr = std::shared_ptr<UniformBuffer>;

    struct Shader{
        u32 program_id;
        u32 ref_count {0};
        // data of uniforms to a shader has.
        std::unordered_map<std::string, UniformObject> uniform_objects;
        // data that needs to be passed to gpu
        std::vector<std::string> uniforms_to_pass_to_gpu;

        // unifrom buffers to pass to the shader
        std::unordered_map<std::string, UniformBufferSPtr> uniform_buffer_objects;
        // uniform buffers to pass to GPU
        std::vector<std::string> uniform_buffers_to_pass_to_gpu;

        // unifrom buffers to pass to the shader
        std::unordered_map<std::string, UniformObject> local_uniform_objects;
        // uniform buffers to pass to GPU
        std::vector<std::string> local_uniforms_to_pass_to_gpu;

        u32 current_binding_point_to_give {0};
        u32 get_next_binding_point() {
            return current_binding_point_to_give++;
        }

        ShaderEnum vertex_shader {SE_NONE};
        ShaderEnum fragment_shader {SE_NONE};  // TODO right now only vertex and fragment shaders




    };
    using ShaderSPtr = std::shared_ptr<Shader>;

    
    class ShaderSystem {
    public:
        static void initiate();
        static void shut_down();
        static void global_uniform_buffer_object_add(void* data, u32 size, const std::string& name);
        static void global_uniform_buffer_object_update(const std::string& name, void* sub_data, u32 size, u32 offset);
        static void global_uniform_buffer_objects_pass_to_gpu(ShaderId shader_id);
        static void create_shader(const std::string& name);
        static void delete_shader(ShaderId shader_id);
        static void shaders_compile(ShaderId shader_id, ShaderEnum vertex_shader, ShaderEnum fragment_shader);
        static void uniform_object_add(ShaderId shader_id, const std::string& uniform, const std::any& value, EUniformType type);
        static void uniform_object_update(ShaderId shader_id, const std::string& uniform, const std::any& value, EUniformType type);
        static void uniform_objects_pass_to_gpu(ShaderId shader_id);
        static void local_uniform_object_add(ShaderId shader_id, const std::string& uniform, const std::any& value, EUniformType type);
        static void local_uniform_object_update(ShaderId shader_id, const std::string& uniform, const std::any& value, EUniformType type);
        static void local_uniform_objects_pass_to_gpu(ShaderId shader_id);
        static void uniform_buffer_object_add(ShaderId shader_id, void* data, u32 size, const std::string& name);
        static void uniform_buffer_object_update(ShaderId shader_id, void* sub_data, u32 size, u32 offset);
        static void uniform_buffer_objects_pass_to_gpu(ShaderId shader_id);
        
    private:
        std::unordered_map<std::string, ShaderSPtr> shaders_data;
        // unifrom buffers to pass to the shader
        std::unordered_map<std::string, UniformBufferSPtr> global_uniform_buffer_objects;
        // uniform buffers to pass to GPU
        std::vector<std::string> global_uniform_buffers_to_pass_to_gpu;
        u32 u_id {0};
        std::vector<u32> free_ids;
    };
}