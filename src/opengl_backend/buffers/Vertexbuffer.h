#pragma once
#include "../../includes.h"
#include <glad/glad.h>
namespace Cold {
    struct VertexBufferLayout {
        std::string name;
        u32 number_of_elements;
        u32 _type;
        bool normalized;
        u32 _stride;
        u64 _offset;
        VertexBufferLayout(const std::string& n, u32 elements, u32 type, bool is_normalized, u64 stride, u64 offset )
            : name(std::move(n))
            , number_of_elements(elements)
            , _type(type)
            , normalized(is_normalized)
            , _stride(stride)
            , _offset(offset) {}
    };

    class VertexBuffer;
    using VertexBufferSPtr = std::shared_ptr<VertexBuffer>;
    class VertexBuffer {
        
    public:
        static VertexBufferSPtr create_vertex_buffer(void* data, u64 size, GLenum usage,
                                                  const std::initializer_list<VertexBufferLayout>& buffer_layout);
        void bind();
        void un_bind();
        const std::vector<VertexBufferLayout>& get_buffer_elements();

    private:
        u32 m_vbo_id;
        std::vector<VertexBufferLayout> m_buffer_layout_elements;
        void* data_ptr;
    };
}
