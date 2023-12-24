#pragma once

#include <glad/glad.h>
#include <memory>

class VertexBuffer{
private:
    std::shared_ptr<unsigned int> id;
    
public:
    VertexBuffer(const void* data, unsigned int size);
    VertexBuffer(const VertexBuffer& other);
    ~VertexBuffer();
    
    void bind() const;
    void unbind() const;
};
