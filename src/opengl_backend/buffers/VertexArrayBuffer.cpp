#include "VertexArrayBuffer.h"


#include <Logger.h>
namespace Cold {
    VertexArrayBufferSPtr VertexArrayBuffer::create_vertex_array_buffer()
    {
        COLD_INFO("Vertex Array Buffer Constructed");
        VertexArrayBufferSPtr object = std::make_shared<VertexArrayBuffer>();
        glGenVertexArrays(1,&object->m_vao_id);
        glBindVertexArray(object->m_vao_id);
        return object;
    }
    void VertexArrayBuffer::push_vertex_buffers(const std::vector<VertexBufferSPtr> vertex_buffers)
    {
        glBindVertexArray(m_vao_id);
        for(auto& vertex_buf : vertex_buffers) 
        {
            vertex_buf->bind();
            u8 index = 0;
            for(const auto& element : vertex_buf->get_buffer_elements()) 
            {
                glEnableVertexAttribArray(index);
                GLboolean norm = element.normalized ? GL_TRUE : GL_FALSE;
                glVertexAttribPointer(index, element.number_of_elements, 
                                    element._type, norm, element._stride, (void*) element._offset);
                index++;  
            }
            vertex_buf->un_bind();
        }
        

        glBindVertexArray(0);
        this->vertex_buffers = std::move(vertex_buffers);

    }
    
    void VertexArrayBuffer::bind()
    {
        glBindVertexArray(m_vao_id);
    }
    void VertexArrayBuffer::un_bind()
    {   
        glBindVertexArray(0);
    }

    VertexArrayBuffer::~VertexArrayBuffer()
    {
        COLD_INFO("Vertex Array buffer destrcuted");
        glDeleteVertexArrays(1, &m_vao_id);
    }
}
