#include "../includes.h"
namespace Cold {
    struct VertexBufferLayout {
        std::string name;
        u32 number_of_elements;
        u32 _type;
        bool normalized;
        u32 _stride;
        u32 _offset;
        VertexBufferLayout(const std::string& n, u32 elements, u32 type, bool is_normalized, u32 stride, u32 offset )
            : name(std::move(n))
            , number_of_elements(elements)
            , _type(type)
            , normalized(is_normalized)
            , _stride(stride)
            , _offset(offset) {}
    };
    class VertexBuffer {
        u32 vbo_id;
        std::vector<VertexBufferLayout> buffer_layout_elements;
    public:
        static VertexBuffer* create_vertex_buffer(float* data, u64 size, const std::initializer_list<VertexBufferLayout>& buffer_layout);
        static void destroy_vertex_buffer(VertexBuffer* buffer);
    };
}
