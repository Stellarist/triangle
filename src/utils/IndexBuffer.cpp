#include "IndexBuffer.hpp"

#include <glad/glad.h>

IndexBuffer::IndexBuffer(const void* data, unsigned int count)
: id(std::make_shared<unsigned int>()), count(count)
{
  glGenBuffers(1, id.get());
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(const IndexBuffer& other)
  : id(other.id), count(other.count)
{
}

IndexBuffer::~IndexBuffer()
{
  glDeleteBuffers(1, id.get());
}

void IndexBuffer::bind() const
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *id);
}

void IndexBuffer::unbind() const
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}