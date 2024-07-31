#include "Vertexbuffer.h"
#include <Logger.h>
namespace Cold {

    VertexBufferSPtr 
    VertexBuffer::create_vertex_buffer(void *data, u64 size, GLenum usage,
                                       const std::initializer_list<VertexBufferLayout> &buffer_layout)
    {
        COLD_INFO("Vertex Buffer Created");
        VertexBufferSPtr object = std::make_shared<VertexBuffer>();
        glGenBuffers(1, &object->m_vbo_id);
        glBindBuffer(GL_ARRAY_BUFFER, object->m_vbo_id);
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
        object->m_buffer_layout_elements = buffer_layout ;
        glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind
        object->data_ptr = data;
        return object;
        
    }

    void VertexBuffer::bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);
    }
    void VertexBuffer::un_bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    const std::vector<VertexBufferLayout> &VertexBuffer::get_buffer_elements()
    {
        return m_buffer_layout_elements;
    }
    VertexBuffer::~VertexBuffer()
    {
        COLD_INFO("Vertex buffer Deleted");
        glDeleteBuffers(1, &m_vbo_id);
    }
}