#include "Geometry.h"
#include <Logger.h>
#include <cstddef>
namespace Cold {

    Geometry::Geometry(GLenum geometry_usage)
    : total_vertex_count(0)
    , ref_count(0)
    , vao(nullptr)
    {
        COLD_TRACE("Geometry Created");
        usage = geometry_usage;
    }

    void Geometry::push_vertex(const Vertex &vertex_data)
    {
        verticies.push_back(std::move(vertex_data));

    }

    void Geometry::push_index(u32 index)
    {
        indicies.push_back(index);
    }

    void Geometry::push_vertex_data(aiVector3D *vertices, u32 vertex_count, aiVector3D *normals, u32 normal_count, aiVector2D *tex_coords, u32 tex_coords_count)
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
            for(u8 j = 0; j < faces[i].mNumIndices; j++)
            indicies.push_back(faces[i].mIndices[j]);
        }
    }

    void Geometry::buffer_data_to_gpu()
    {
        if(vao) return;
        auto vbo_data = VertexBuffer::create_vertex_buffer(verticies.data(), verticies.size() * sizeof(float), usage,
        {
            {"aPos", 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, position)},
            {"aNormal", 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, normals)},
            {"aTex", 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, tex_coord)}
        }
        );
        vao = VertexArrayBuffer::create_vertex_array_buffer();
        vao->push_vertex_buffers({vbo_data});
        
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(u32), indicies.data(), usage);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Geometry::delete_data_from_gpu()
    {
        // TODO fill this
    }

    void Geometry::render()
    {
        vao->bind();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, total_vertex_count, GL_UNSIGNED_INT ,0);
    }

    Geometry::~Geometry()
    {
        // TODO fill this
    }

}