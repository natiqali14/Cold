#pragma once
#include "../../includes.h"
#include <vector>
#include <memory>
namespace Cold {
    class VertexBuffer;

    class VertexArrayBuffer {
    public:
        static VertexArrayBuffer* create_vertex_array_buffer();
        void push_vertex_buffer(const std::shared_ptr<VertexBuffer> &vertex_buffer);
        void bind();
        void un_bind();
    private:
        u32 m_vao_id;
        std::vector<std::shared_ptr<VertexBuffer>> vertex_buffers;
        // TODO add index buffer shared ptr here too.
    };
}