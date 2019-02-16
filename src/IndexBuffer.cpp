#include <pch.h>

#include "Renderer.hpp"
#include "IndexBuffer.hpp"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
: m_Count(count) {
    
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));
    
    // Create a new buffer
    GLCALL(glGenBuffers(1, &m_RendererID));
    // Bind means select the buffer
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    // Populate the buffer with data (type, size, pointer, draw_type)
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
    GLCALL(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(unsigned int), data));
    if(glIsBuffer(this->m_RendererID) != GL_TRUE) std::cout << "[ ERROR ] - Index Buffer not generated\n";
    else std::cout << "Index Buffer successfully generated\n";
}

IndexBuffer::~IndexBuffer(){
    GLCALL(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const {
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); 
}

void IndexBuffer::Unbind() const {
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); 
}
