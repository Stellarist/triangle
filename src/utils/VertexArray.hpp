#pragma once

#include "VertexBuffer.hpp"
#include "VertexLayout.hpp"

class VertexArray {
private:
    unsigned int vao;

public:
    VertexArray();
    ~VertexArray();
    void bind() const;
    void unbind() const;
    void addBuffer(const VertexBuffer& vb, const VertexLayout& layout);
};
