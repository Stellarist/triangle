#pragma once

#include <memory>

class IndexBuffer{
private:
    std::shared_ptr<unsigned int> id;
    unsigned int count;

public:
    IndexBuffer(const void* data, unsigned int count);
    IndexBuffer(const IndexBuffer& other);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;
    unsigned int getCount() const;
};

inline unsigned int IndexBuffer::getCount() const
{
    return count;
}
