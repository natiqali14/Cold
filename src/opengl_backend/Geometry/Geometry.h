#pragma once
#include <includes.h>
#include <glad/glad.h>
#include <VertexBuffer.h>
#include <VertexArrayBuffer.h>
namespace Cold {

    class Geometry {
    public:
        Geometry(const std::initializer_list<VertexBufferSPtr>& vertex_data, void* index_data = nullptr, u32 index_size = 0);
        ~Geometry();
    private:
        /** 
         * gpu vertex data for a geometry
         * can have vertex data or some thing else like normal maps, texture co-oridnates etc
        */
        std::vector<VertexBufferSPtr> vbo_vector;

        /* unique VAO for this geometry  */
        VertexArrayBufferSPtr vao;
        /* EBO if any for this geometry's vertex data*/
        void* ebo_data;
        u32 ebo_size;
        u32 ebo;
    };



}