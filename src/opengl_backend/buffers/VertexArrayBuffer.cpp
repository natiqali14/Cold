#include "VertexArrayBuffer.h"
#include "Vertexbuffer.h"
#include <glad/glad.h>
namespace Cold {
    VertexArrayBuffer* VertexArrayBuffer::create_vertex_array_buffer()
    {
        VertexArrayBuffer* object = new VertexArrayBuffer;
        glGenVertexArrays(1,&object->m_vao_id);
        glBindVertexArray(object->m_vao_id);
        return object;
    }
    void VertexArrayBuffer::push_vertex_buffer(const std::shared_ptr<VertexBuffer> &vertex_buffer)
    {
        glBindVertexArray(m_vao_id);
        u8 index = 0;
        for(const auto& element : vertex_buffer->get_buffer_elements()) {
            glEnableVertexAttribArray(index);
            GLboolean norm = element.normalized ? GL_TRUE : GL_FALSE;
            glVertexAttribPointer(index, element.number_of_elements, 
                                  element._type, norm, element._stride, (void*) element._offset);
            index++;  
        }

        glBindVertexArray(0);

    }
    
    void VertexArrayBuffer::bind()
    {
        glBindVertexArray(m_vao_id);
    }
    void VertexArrayBuffer::un_bind()
    {   
        glBindVertexArray(0);
    }
}
