#pragma once

#include "Core.h"
#include "SDL.h"

namespace toy
{

class Buffer;
class BufferLayout;

class VertexArray final
{
public:
    void bind() const
    {
        glBindVertexArray(id);
    }

    void unbind() const
    {
        glBindVertexArray(0);
    }

    static VertexArray create(const Buffer& vertex_buffer, const BufferLayout& layout);

private:
    u32 id;
};

}