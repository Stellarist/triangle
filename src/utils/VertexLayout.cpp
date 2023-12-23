#include "VertexLayout.hpp"

#include <cassert>
#include <glad/glad.h>

template <typename T>
void VertexLayout::push(unsigned int count)
{
    assert(false);
}

template<>
void VertexLayout::push<float>(unsigned int count)
{
    unsigned int size=count*sizeof(float);
    elements.push_back({GL_FLOAT, count, size, false});
    stride+=size;
}

template<>
void VertexLayout::push<unsigned int>(unsigned int count)
{
    unsigned int size=count*sizeof(unsigned int);
    elements.push_back({GL_UNSIGNED_INT, count, size, false});
    stride+=size;
}

template<>
void VertexLayout::push<unsigned char>(unsigned int count)
{
    unsigned int size=count*sizeof(unsigned char);
    elements.push_back({GL_UNSIGNED_BYTE, count, size, false});
    stride+=size;
}
