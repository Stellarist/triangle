#pragma once

#include <vector>

struct VertexBufferElement{
    unsigned int type;
    unsigned int count;
    bool normalized;
};

class VertexBufferLayout{
private:
    std::vector<VertexBufferElement> elements;
    unsigned int stride;
public:
    VertexBufferLayout()
        : stride(0) {}
    ~VertexBufferLayout()=default;
    const std::vector<VertexBufferElement>& getElements() const;
    unsigned int getStride() const;
    template<typename T> void push(unsigned int count);
};

inline const std::vector<VertexBufferElement>& VertexBufferLayout::getElements() const
{
    return elements;
}

inline unsigned int VertexBufferLayout::getStride() const
{
    return stride;
}
