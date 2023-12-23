#include "VertexLayout.hpp"

#include <cassert>
#include <glad/glad.h>

template <typename T>
void VertexLayout::push(unsigned int count, unsigned int offset)
{
    assert(false);
}

template<>
void VertexLayout::push<float>(unsigned int count, unsigned int offset)
{
    if(offset==-1){
        elements.push_back({GL_FLOAT, count, static_cast<unsigned int>(count*sizeof(float)), false});
        stride+=count*sizeof(float);
    }else{
        elements.push_back({GL_FLOAT, count, offset, false});
        stride+=offset;
    }
}

template<>
void VertexLayout::push<unsigned int>(unsigned int count, unsigned int offset)
{
    if(offset==-1){
        elements.push_back({GL_UNSIGNED_INT, count, static_cast<unsigned int>(count*sizeof(unsigned int)), false});
        stride+=count*sizeof(unsigned int);
    }else{
        elements.push_back({GL_UNSIGNED_INT, count, offset, false});
        stride+=offset;
    }
}

template<>
void VertexLayout::push<unsigned char>(unsigned int count, unsigned int offset)
{
    if(offset==-1){
        elements.push_back({GL_UNSIGNED_BYTE, count, static_cast<unsigned int>(count*sizeof(unsigned char)), true});
        stride+=count*sizeof(unsigned char);
    }else{
        elements.push_back({GL_UNSIGNED_BYTE, count, offset, true});
        stride+=offset;
    }
}

template<>
void VertexLayout::push<int>(unsigned int count, unsigned int offset)
{
    if(offset==-1){
        elements.push_back({GL_INT, count, static_cast<unsigned int>(count*sizeof(int)), false});
        stride+=count*sizeof(int);
    }else{
        elements.push_back({GL_INT, count, offset, false});
        stride+=offset;
    }
}
