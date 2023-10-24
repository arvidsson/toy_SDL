#pragma once

#include "Core.h"
#include "SDL.h"

namespace toy
{

class Buffer final
{
public:

    enum class Type
    {
        Index,
        Vertex
    };

    Type get_type() const { return type; }
    
    void bind() const
    {
        glBindBuffer(type == Type::Vertex ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER, id);
    }

    void unbind() const
    {
        glBindBuffer(type == Type::Vertex ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    static Buffer create_index_buffer(u32* indices, u32 count)
    {
        Buffer buffer{};
        buffer.type = Type::Index;
        glCreateBuffers(1, &buffer.id);
        glBindBuffer(GL_ARRAY_BUFFER, buffer.id);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    static Buffer create_vertex_buffer(f32* vertices, u32 size)
    {
        Buffer buffer{};
        buffer.type = Type::Vertex;
        glCreateBuffers(1, &buffer.id);
        glBindBuffer(GL_ARRAY_BUFFER, buffer.id);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        return buffer;
    }

private:
    Type type;
    u32 id;
};

}