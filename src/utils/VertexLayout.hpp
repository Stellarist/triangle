#pragma once

#include <vector>

struct VertexElement{
    unsigned int type;
    unsigned int count;
    unsigned int stride;
    bool normalized;
};

class VertexLayout{
private:
    std::vector<VertexElement> elements;
    unsigned int stride;

public:
    VertexLayout(): stride(0) {}
    ~VertexLayout()=default;

    const std::vector<VertexElement>& getElements() const;
    unsigned int getStride() const;
    template<typename T> void push(unsigned int count, unsigned int offset=-1);
};

inline const std::vector<VertexElement>& VertexLayout::getElements() const
{
    return elements;
}

inline unsigned int VertexLayout::getStride() const
{
    return stride;
}
