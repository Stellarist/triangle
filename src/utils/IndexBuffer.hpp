#pragma once

class IndexBuffer{
private:
    unsigned int id;
    unsigned int count;
public:
    IndexBuffer(const void* data, unsigned int count);
    ~IndexBuffer();
    void bind() const;
    void unbind() const;
    unsigned int getCount() const;
};

inline unsigned int IndexBuffer::getCount() const
{
    return count;
}
