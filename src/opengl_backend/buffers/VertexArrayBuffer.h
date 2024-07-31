#pragma once
#include "../../includes.h"
#include "Vertexbuffer.h"
#include <vector>
#include <memory>
namespace Cold {

    class VertexBuffer;
    class VertexArrayBuffer;
    using VertexArrayBufferSPtr = std::shared_ptr<VertexArrayBuffer>;

    class VertexArrayBuffer {
            
    public:
        static VertexArrayBufferSPtr create_vertex_array_buffer();
        void push_vertex_buffers(const std::vector<VertexBufferSPtr> vertex_buffers);
        void bind();
        void un_bind();
        ~VertexArrayBuffer();
    private:
        u32 m_vao_id;
        std::vector<VertexBufferSPtr> vertex_buffers;
        // TODO add index buffer shared ptr here too.
    };
}