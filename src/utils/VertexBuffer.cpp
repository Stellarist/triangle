#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
: id(std::make_shared<unsigned int>())
{
  glGenBuffers(1, id.get());
  glBindBuffer(GL_ARRAY_BUFFER, *id);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(const VertexBuffer& other)
  : id(other.id)
{
}

VertexBuffer::~VertexBuffer()
{
  if(id.unique())
    glDeleteBuffers(1, id.get());
}

void VertexBuffer::bind() const
{
  glBindBuffer(GL_ARRAY_BUFFER, *id);
}

void VertexBuffer::unbind() const
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}