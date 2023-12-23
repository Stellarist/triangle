#include "VertexArray.hpp"

#include <glad/glad.h>

VertexArray::VertexArray() 
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &vao);
}

void VertexArray::bind() const
{
    glBindVertexArray(vao);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::addBuffer(const VertexBuffer& vbo, const VertexLayout& layout)
{
    bind();
    vbo.bind();
    const auto& elements = layout.getElements();
    unsigned int offset = 0;
    for(unsigned int i=0; i<elements.size(); i++){
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), reinterpret_cast<void*>(offset));
        offset+=element.stride;
    }
}
