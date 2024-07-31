#include "Geometry.h"
#include <Logger.h>
#include <cstddef>
#include <TextureSystem.h>
#include <comman_data.h>
namespace Cold {
    Geometry::Geometry(GLenum geometry_usage)
    : ref_count(1)
    , total_vertex_count(0)
    , vao(nullptr)
    , transform(std::make_shared<Transform>())
    , usage(geometry_usage)
    , ebo(0)
    {
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

    void Geometry::push_indicies(aiFace *faces, u32 face_count)
    {
        for(u32 i = 0; i < face_count; i++) {
            aiFace face = faces[i];
            indicies.push_back(face.mIndices[0]);
            indicies.push_back(face.mIndices[1]);
            indicies.push_back(face.mIndices[2]);
        }
    }

    void Geometry::buffer_data_to_gpu()
    {
        if(vao) return;
        auto vbo_data = VertexBuffer::create_vertex_buffer(verticies.data(), verticies.size() * sizeof(Vertex), usage,
        {
            {"aPos", 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, position)},
            {"aNormal", 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, normals)},
            {"aTexCoord", 2, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, tex_coord)}
        }
        );
        vao = VertexArrayBuffer::create_vertex_array_buffer();
        vao->push_vertex_buffers({vbo_data});
        
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(u32), indicies.data(), usage);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        TextureProps props;
        props.image_data_type = GL_UNSIGNED_BYTE;
        props.internal_format = GL_RGBA;
        props.magnification_fillter = GL_LINEAR;
        props.minification_filter = GL_LINEAR_MIPMAP_LINEAR;
        props.should_generate_mipmap = true;
        props.wrap_x_axis = GL_REPEAT;
        props.wrap_y_axis = GL_REPEAT;
        geometry_material.diff_tex_id = TextureSystem::texture_2D_immutable_create(geometry_material.diff_tex, std::move(props));
    }

    void Geometry::delete_data_from_gpu()
    {
        glDeleteBuffers(1, &ebo);
        vao.reset();
    }

    void Geometry::render()
    {
        // TODO change this
        glBindTexture(GL_TEXTURE_2D, 0);
        u32 p_id = v_data.program_id;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, geometry_material.diff_tex_id);

        u32 loci = glGetUniformLocation(p_id, "frameTexture");
        glUniform1i(loci, 0);
        auto camera = v_data.cs;
        glm::mat4 vM = camera->get_camera_view_space();
        glm::mat4 model_1 = transform->get_world_model();
        u32 loc_1 = glGetUniformLocation(p_id, "view");
        glUseProgram(p_id);
        glUniformMatrix4fv(loc_1, 1, GL_FALSE, glm::value_ptr(vM));

        u32 loc_2 = glGetUniformLocation(p_id, "model");
        glUseProgram(p_id);
        glUniformMatrix4fv(loc_2, 1, GL_FALSE, glm::value_ptr(transform->get_world_model()));

        glm::mat4 p = glm::perspective(45.0f, 800.f / 600.f, 0.1f, 1000.f);
        u32 loc3 = glGetUniformLocation(p_id, "p");
        glUseProgram(p_id);
        glUniformMatrix4fv(loc3, 1, GL_FALSE, glm::value_ptr(p));


       
        vao->bind();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, total_vertex_count, GL_UNSIGNED_INT ,0);
         glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Geometry::set_material(const Material &material)
    {
        geometry_material = std::move(material);
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