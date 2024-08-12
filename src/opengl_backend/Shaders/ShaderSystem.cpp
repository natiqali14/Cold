#include "ShaderSystem.h"
#include <Logger.h>
namespace Cold
{
#define CHECK_FOR_GLOBAL_UNIFORM(instance_map, name) { \
    COLD_ASSERT(instance_map.count(name) > 0, "Failed to have a global uniform buffer of that name") \
}

    ShaderSystem* instance = nullptr;
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

    void ShaderSystem::global_uniform_buffer_objects_pass_to_gpu()
    {
        
    }

    ShaderId ShaderSystem::create_shader()
    {
        return ShaderId();
    }
    void ShaderSystem::shaders_compile(ShaderId shader_id, const char *vertex_shader, const char *fragment_shader)
    {
    }
    void ShaderSystem::uniform_object_add(ShaderId shader_id, const std::string &uniform, const std::any &value)
    {
    }
    void ShaderSystem::uniform_object_update(ShaderId shader_id, const std::string &uniform, const std::any &value)
    {
    }
    void ShaderSystem::uniform_objects_pass_to_gpu(ShaderId shader_id)
    {
    }
    void ShaderSystem::uniform_buffer_object_add(ShaderId shader_id, void *data, u32 size, const std::string &name)
    {
    }
    void ShaderSystem::uniform_buffer_object_update(ShaderId shader_id, void *sub_data, u32 size, u32 offset)
    {
    }
    void ShaderSystem::uniform_buffer_objects_pass_to_gpu(ShaderId shader_id)
    {
    }
}
