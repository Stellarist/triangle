#include "VertexBufferLayout.hpp"

#include <cassert>
#include <glad/glad.h>

template <typename T>
void VertexBufferLayout::push(unsigned int count)
{
    assert(false);
}

template<>
void VertexBufferLayout::push<float>(unsigned int count)
{
    elements.push_back({GL_FLOAT, count, false});
    stride+=sizeof(GLfloat);
}

template<>
void VertexBufferLayout::push<unsigned int>(unsigned int count)
{
    elements.push_back({GL_UNSIGNED_INT, count, false});
    stride+=sizeof(GLuint);
}

template<>
void VertexBufferLayout::push<unsigned char>(unsigned int count)
{
    elements.push_back({GL_UNSIGNED_BYTE, count, false});
    stride+=sizeof(GLubyte);
}