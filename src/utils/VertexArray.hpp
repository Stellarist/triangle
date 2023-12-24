#pragma once

#include <memory>

#include "VertexBuffer.hpp"
#include "VertexLayout.hpp"

class VertexArray {
private:
    std::shared_ptr<unsigned int> id;

public:
    VertexArray();
    VertexArray(const VertexArray& other);
    ~VertexArray();
    
    void bind() const;
    void unbind() const;
    void addBuffer(const VertexBuffer& vbo, const VertexLayout& layout);
};
