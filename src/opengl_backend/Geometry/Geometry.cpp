#include "Geometry.h"
#include <Logger.h>
#include <cstddef>
#include <TextureSystem.h>
#include <comman_data.h>
#include <BackendConfigData.h>
namespace Cold {
    Geometry::Geometry(GLenum geometry_usage)
    : ref_count(1)
    , total_vertex_count(0)
    , vao(nullptr)
    , transform(std::make_shared<Transform>())
    , usage(geometry_usage)
    , ebo(0)
    , shader(default_data.default_shader_name)
    {
        ShaderSystem::apply_shader(shader);
    }

    void Geometry::push_vertex(const Vertex &vertex_data)
    {
        verticies.push_back(std::move(vertex_data));

    }

    void Geometry::push_index(u32 index)
    {
        indicies.push_back(index);
    }

    void Geometry::push_vertex_data(aiVector3D *vertices, u32 vertex_count, aiVector3D *normals, u32 normal_count, aiVector3D *tex_coords, u32 tex_coords_count)
    {
        total_vertex_count = vertex_count;
        for(u32 i = 0; i < vertex_count; i++) {
            Vertex v;
            v.position = {vertices[i].x, vertices[i].y, vertices[i].z};
            v.normals = {normals[i].x, normals[i].y, normals[i].z};
            v.tex_coord = {tex_coords[i].x, tex_coords[i].y};
            this->verticies.push_back(std::move(v));
        }
    }

    void Geometry::push_vertex_data(const std::vector<Vertex> &vert_data)
    {
        if(!verticies.empty()) verticies.clear();
        total_vertex_count = vert_data.size();
        this->verticies = std::move(vert_data);
    }

    void Geometry::push_indicies(aiFace *faces, u32 face_count)
    {   
        for(u32 i = 0; i < face_count; i++) {
            aiFace face = faces[i];
            indicies.push_back(face.mIndices[0]);
            indicies.push_back(face.mIndices[1]);
            indicies.push_back(face.mIndices[2]);
        }
    }

    void Geometry::push_inicies(const std::vector<u32> &index_data)
    {
        if(!indicies.empty()) indicies.clear();
        indicies = std::move(index_data);
    }

    void Geometry::buffer_data_to_gpu()
    {
        if(!vao) {
            auto vbo_data = VertexBuffer::create_vertex_buffer(verticies.data(), verticies.size() * sizeof(Vertex), usage,
            {
                {"aPos", 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, position)},
                {"aNormal", 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, normals)},
                {"aTexCoord", 2, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, tex_coord)},
                {"aTangent", 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, tangents)},
                {"aBitangent", 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, bitangents)}
            }
            );
            vao = VertexArrayBuffer::create_vertex_array_buffer();
            vao->push_vertex_buffers({vbo_data});
            if(indicies.size() == 0) {
                ebo = 0; 
            }
            else {
                glGenBuffers(1, &ebo);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(u32), indicies.data(), usage);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            }
           

        } 
       
        buffer_material_data();
    }

    void Geometry::buffer_material_data()
    {
       
        TextureProps props;
        props.image_data_type = GL_UNSIGNED_BYTE;
        props.internal_format = GL_RGBA;
        props.magnification_fillter = GL_LINEAR;
        props.minification_filter = GL_LINEAR_MIPMAP_LINEAR;
        props.should_generate_mipmap = true;
        props.wrap_x_axis = GL_REPEAT;
        props.wrap_y_axis = GL_REPEAT;
        geometry_material.diff_tex_id = TextureSystem::texture_2D_immutable_create(geometry_material.diff_tex,(props));
        if(geometry_material.shininess > 0) 
        {
            TextureProps props2;
            props2.image_data_type = GL_UNSIGNED_BYTE;
            props2.internal_format = GL_RGBA;
            props2.magnification_fillter = GL_LINEAR;
            props2.minification_filter = GL_LINEAR_MIPMAP_LINEAR;
            props2.should_generate_mipmap = true;
            props2.wrap_x_axis = GL_REPEAT;
            props2.wrap_y_axis = GL_REPEAT;
            geometry_material.specular_texture_id = 
                TextureSystem::texture_2D_immutable_create(geometry_material.specular_texure, props2);
        } 
        if(geometry_material.normal_texture.length() > 0)
        {
            TextureProps props2;
            props2.image_data_type = GL_UNSIGNED_BYTE;
            props2.internal_format = GL_RGBA;
            props2.magnification_fillter = GL_LINEAR;
            props2.minification_filter = GL_LINEAR_MIPMAP_LINEAR;
            props2.should_generate_mipmap = true;
            props2.wrap_x_axis = GL_REPEAT;
            props2.wrap_y_axis = GL_REPEAT;
            geometry_material.normal_texture_id = 
                TextureSystem::texture_2D_immutable_create(geometry_material.normal_texture, std::move(props2));
        }  
    }

    void Geometry::delete_data_from_gpu()
    {
        glDeleteBuffers(1, &ebo);
        vao.reset();
    }


    void Geometry::render()
    {
        // TODO change this
        ShaderSystem::pass_sampler_to_gpu(shader, geometry_material.diff_tex_id, 0, "frameTexture");
        if(geometry_material.shininess > 0) {
            ShaderSystem::pass_sampler_to_gpu(shader, geometry_material.specular_texture_id, 1, "frameTexture");
        }
        if(geometry_material.normal_texture.length() > 0) {
            ShaderSystem::pass_sampler_to_gpu(shader, geometry_material.normal_texture_id, 2, "frameTexture");   
        }
        ShaderSystem::local_uniform_object_add(shader, "shininess", geometry_material.shininess, EUT_FLOAT_1);
        ShaderSystem::local_uniform_object_add(shader, "hasNormal", geometry_material.normal_texture.length() > 0 ? 1: 0, EUT_INT_1);
            
        ShaderSystem::local_uniform_object_add(shader, "model", transform->get_world_model(), EUT_MAT_4x4);
        ShaderSystem::local_uniform_object_add(shader, "diffuse_color", geometry_material.diffuse_color, EUT_FLOAT_3);
        ShaderSystem::pass_all_shader_data_to_gpu(shader);
        vao->bind();
        if(ebo != 0) 
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glDrawElements(GL_TRIANGLES, total_vertex_count, GL_UNSIGNED_INT ,0);
        }
        else {
            glDrawArrays(GL_TRIANGLES, 0, total_vertex_count);
        }
        
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Geometry::set_material(const Material &material)
    {
        geometry_material = std::move(material);
    }

    void Geometry::change_material(const Material &material)
    {
        TextureSystem::delete_texture(geometry_material.diff_tex_id);
        TextureSystem::delete_texture(geometry_material.specular_texture_id);
        geometry_material = material;
        buffer_material_data();;
    }

    TransformSPtr Geometry::get_transform()
    {
        return transform;
    }

    Geometry::~Geometry()
    {
        glDeleteBuffers(1, &ebo);
    }

}