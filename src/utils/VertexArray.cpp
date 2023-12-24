#include "VertexArray.hpp"

#include <glad/glad.h>

VertexArray::VertexArray() 
: id(std::make_shared<unsigned int>())
{
    glGenVertexArrays(1, id.get());
    glBindVertexArray(*id);
}

VertexArray::VertexArray(const VertexArray& other)
: id(other.id)
{
}

VertexArray::~VertexArray()
{
    if(id.unique())
        glDeleteVertexArrays(1, id.get());
}

void VertexArray::bind() const
{
    glBindVertexArray(*id);
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
