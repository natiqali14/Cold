#include "ShaderSystem.h"
#include <glad/glad.h>
namespace Cold
{
#define CHECK_FOR_GLOBAL_UNIFORM(instance_map, name) { \
    COLD_ASSERT(instance_map.count(name) > 0, "Failed to have a global uniform buffer of that name") \
}

#define CHECK_FOR_SHADER(shader_id, map) { \
    COLD_ASSERT(map.count(shader_id) > 0, "Failed to find shader"); \
}

#define UNBIND_UNIFORM_BUFFER() glBindBuffer(GL_UNIFORM_BUFFER, 0);

#define ANY_CAST(val, type) std::any_cast<type>(val)

    ShaderSystem* instance = nullptr;


    // static private functions

    static u32 create_gl_shader(ShaderEnum e, GLenum shader_type) 
    {
        const char* shader_code = shader_map[e].c_str();
        u32 shader = glCreateShader(shader_type);
        glShaderSource(shader, 1, &shader_code, NULL);
        glCompileShader(shader);
        i32 s;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &s);
        if (!s)
        {
            char info_log[1024];
            glGetShaderInfoLog(shader, 1024, NULL, info_log);
            COLD_ERROR("Shader compilation failed %s", info_log);
            std::cout << "Shader compilation failed. \n"
                    << info_log << "\n";
        }
        return shader;
    }

    static u32 create_program(u32 vertex, u32 fragment) 
    {
        u32 program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        glLinkProgram(program);
        i32 s;
        glGetProgramiv(program, GL_LINK_STATUS, &s);
        if (!s)
        {
            char info[512];
            glGetProgramInfoLog(program, 512, NULL, info);
            std::cout << "program linking failed. \n"
                    << info << "\n";
        }
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return program;
    }

    static void pass_uniform_to_shader(u32 program, const std::string& name, const std::any& value, EUniformType type)
    {
        glUseProgram(program);
        i32 loc = glGetUniformLocation(program, name.c_str());
        switch (type)
        {
        case EUT_FLOAT_1:
            glUniform1f(loc, ANY_CAST(value, f32));
            break;
        case EUT_FLOAT_2: {
            auto vec2 = ANY_CAST(value, glm::vec2);
            glUniform2f(loc,vec2.x, vec2.y);
            break;
        }
        case EUT_FLOAT_3: {
            auto vec3 = ANY_CAST(value, glm::vec3);
            glUniform3f(loc,vec3.x, vec3.y, vec3.z);
            break;
        }
        
        case EUT_FLOAT_4: {
            auto vec4 = ANY_CAST(value, glm::vec4);
            glUniform4f(loc,vec4.x, vec4.y, vec4.z, vec4.w);
            break;
        }
        case EUT_INT_1:
            glUniform1i(loc, ANY_CAST(value, i32));
            break;

        case EUT_MAT_3x3: {
            auto mat3 = ANY_CAST(value, glm::mat3);
            glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(mat3));
            break;
        }

        case EUT_MAT_4x4: {
            auto mat4 = ANY_CAST(value, glm::mat4);
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat4));
            break;
        }

        default:
            break;
        }
    }



    // others
    void ShaderSystem::initiate()
    {
        instance = new ShaderSystem;
        COLD_TRACE("Shader System Initialse");
    }
    void ShaderSystem::shut_down()
    {
        delete instance;
        instance = nullptr;
        COLD_TRACE("Shader System Shut Down");
    }

    void ShaderSystem::global_uniform_buffer_object_add(void *data, u32 size, const std::string &name)
    {
        UniformBufferSPtr buffer =  std::make_shared<UniformBuffer>();
        buffer->data = data;
        buffer->size = size;
        instance->global_uniform_buffer_objects.insert({name, buffer});
    }

    void ShaderSystem::global_uniform_buffer_object_update(const std::string& name, void *sub_data, u32 size, u32 offset)
    {
        CHECK_FOR_GLOBAL_UNIFORM(instance->global_uniform_buffer_objects, name);
        UniformBufferSPtr buffer = instance->global_uniform_buffer_objects[name];
        buffer->add_sub_data_to_pass_to_gpu(sub_data, size, offset);
    }

    void ShaderSystem::global_uniform_buffer_objects_pass_to_gpu(ShaderId shader_id)
    {
        CHECK_FOR_SHADER(shader_id, instance->shaders_data);
        for (auto& ubo : instance->global_uniform_buffer_objects) 
        {
            auto uniform_buffer_obj = ubo.second;
            if(ubo.second->uniform_buffer_u_id == 0) 
            {
                // ubo is not initialised
                // buffer all data
                glGenBuffers(1, &uniform_buffer_obj->uniform_buffer_u_id);
                glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_obj->uniform_buffer_u_id);
                glBufferData(GL_UNIFORM_BUFFER, uniform_buffer_obj->size, uniform_buffer_obj->data, GL_STATIC_DRAW);
            }

            // checking for update of ubo data
            for (auto& sub_data : uniform_buffer_obj->data_to_update) 
            {
                u64 address = (u64) uniform_buffer_obj->data;
                memcpy((void*)address + sub_data.offset, sub_data.sub_data, sub_data.size);
                glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_obj->uniform_buffer_u_id);
                glBufferSubData(GL_UNIFORM_BUFFER, sub_data.offset, sub_data.size, sub_data.sub_data);   
                free(sub_data.sub_data);
            }
            uniform_buffer_obj->data_to_update.clear();

            // bindind this ubo to the shader program currently running.
            glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_obj->uniform_buffer_u_id);
            auto& shader = instance->shaders_data[shader_id];
            uniform_buffer_obj->current_binding_point = shader->get_next_binding_point();
            glBindBufferBase(GL_UNIFORM_BUFFER,
                             uniform_buffer_obj->current_binding_point,
                             uniform_buffer_obj->uniform_buffer_u_id);
            
            u32 block_index = glGetUniformBlockIndex(shader->program_id, ubo.first.c_str());
            glUniformBlockBinding(shader->program_id, block_index, uniform_buffer_obj->current_binding_point);

            UNBIND_UNIFORM_BUFFER();
        }
    }

    void ShaderSystem::create_shader(const std::string& name)
    {
        if(instance->shaders_data.count(name)) {
            COLD_ERROR("Trying to make shader with same name");
        }
        ShaderSPtr shader = std::make_shared<Shader>();
        instance->shaders_data.insert({name, shader});
    }
    void ShaderSystem::delete_shader(ShaderId shader_id)
    {
        CHECK_FOR_SHADER(shader_id, instance->shaders_data);
        instance->shaders_data[shader_id]->ref_count--;
        if(instance->shaders_data[shader_id]->ref_count <= 0) {
            instance->shaders_data[shader_id].reset();
        }
    }

    void ShaderSystem::shaders_compile(ShaderId shader_id, ShaderEnum vertex_shader, ShaderEnum fragment_shader)
    {
        CHECK_FOR_SHADER(shader_id, instance->shaders_data);
        auto& shader = instance->shaders_data[shader_id];
        if(shader->fragment_shader == SE_NONE || shader->vertex_shader == SE_NONE
        || shader->fragment_shader != fragment_shader || shader->vertex_shader != vertex_shader) 
        {
            // compile and create program
            if(shader->program_id != 0) {
                glDeleteProgram(shader->program_id);
                shader->program_id = 0;
            }

            u32 vertex = create_gl_shader(vertex_shader, GL_VERTEX_SHADER);
            u32 fragment = create_gl_shader(fragment_shader, GL_FRAGMENT_SHADER);
            shader->program_id = create_program(vertex, fragment);
        }

    }

    void ShaderSystem::uniform_object_add(ShaderId shader_id, const std::string &uniform, const std::any &value, EUniformType type)
    {
        CHECK_FOR_SHADER(shader_id, instance->shaders_data);
        auto& shader = instance->shaders_data[shader_id];
        shader->uniform_objects[uniform] = {value, type};
        shader->uniform_buffers_to_pass_to_gpu.push_back(uniform);
    }

    void ShaderSystem::uniform_object_update(ShaderId shader_id, const std::string &uniform, const std::any &value, EUniformType type)
    {
        CHECK_FOR_SHADER(shader_id, instance->shaders_data);
        auto& shader = instance->shaders_data[shader_id];
        shader->uniform_objects[uniform] = {value, type};
        shader->uniform_buffers_to_pass_to_gpu.push_back(uniform);

    }

    void ShaderSystem::uniform_objects_pass_to_gpu(ShaderId shader_id)
    {
        CHECK_FOR_SHADER(shader_id, instance->shaders_data);
        auto& shader = instance->shaders_data[shader_id];
        for(auto& uniform : shader->uniforms_to_pass_to_gpu) 
        {
            pass_uniform_to_shader(shader->program_id, uniform, shader->uniform_objects[uniform].value,
                                    shader->uniform_objects[uniform].type);
        }
    }

    void ShaderSystem::local_uniform_object_add(ShaderId shader_id, const std::string &uniform, const std::any &value, EUniformType type)
    {
        CHECK_FOR_SHADER(shader_id, instance->shaders_data);
        auto& shader = instance->shaders_data[shader_id];
        shader->local_uniform_objects[uniform] = {value, type};
    }

    void ShaderSystem::local_uniform_object_update(ShaderId shader_id, const std::string &uniform, const std::any &value, EUniformType type)
    {
        CHECK_FOR_SHADER(shader_id, instance->shaders_data);
        auto& shader = instance->shaders_data[shader_id];
        shader->local_uniform_objects[uniform] = {value, type};
    }

    void ShaderSystem::local_uniform_objects_pass_to_gpu(ShaderId shader_id)
    {
        CHECK_FOR_SHADER(shader_id, instance->shaders_data);
        auto& shader = instance->shaders_data[shader_id];
        for(auto& uniforms : shader->local_uniform_objects)
        {
            auto& obj = uniforms.second;
            pass_uniform_to_shader(shader->program_id,uniforms.first, obj.value, obj.type);
        }
    }

    void ShaderSystem::uniform_buffer_object_add(ShaderId shader_id, void *data, u32 size, const std::string &name)
    {
        UniformBufferSPtr buffer =  std::make_shared<UniformBuffer>();
        buffer->data = data;
        buffer->size = size;
        instance->global_uniform_buffer_objects.insert({name, buffer});
    }

    void ShaderSystem::uniform_buffer_object_update(ShaderId shader_id, void *sub_data, u32 size, u32 offset)
    {
    }

    void ShaderSystem::uniform_buffer_objects_pass_to_gpu(ShaderId shader_id)
    {
    }

}
