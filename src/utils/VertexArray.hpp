#pragma once

#include "VertexBuffer.hpp"
#include "VertexLayout.hpp"

class VertexArray {
private:
    unsigned int id;

public:
    VertexArray();
    ~VertexArray();
    
    void bind() const;
    void unbind() const;
    void addBuffer(const VertexBuffer& vbo, const VertexLayout& layout);
};
