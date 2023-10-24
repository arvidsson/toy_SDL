#include "VertexArray.h"
#include "Buffer.h"
#include "BufferLayout.h"

namespace toy
{

VertexArray VertexArray::create(const Buffer& vertex_buffer, const BufferLayout& layout)
{
	VertexArray vertex_array{};
	glCreateVertexArrays(1, &vertex_array.id);
	vertex_array.bind();
	vertex_buffer.bind();

	int index = 0;

	for (const auto& element : layout) {
		switch (element.type) {
			case ShaderData::Type::Float:
			case ShaderData::Type::Float2:
			case ShaderData::Type::Float3:
			case ShaderData::Type::Float4:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index,
					ShaderData::get_count(element.type),
					ShaderData::get_gl_type(element.type),
					element.normalized ? GL_TRUE : GL_FALSE,
					layout.get_stride(),
					(const void*)element.offset);
				index++;
				break;
			}
			case ShaderData::Type::Int:
			case ShaderData::Type::Int2:
			case ShaderData::Type::Int3:
			case ShaderData::Type::Int4:
			case ShaderData::Type::Bool:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribIPointer(index,
					ShaderData::get_count(element.type),
					ShaderData::get_gl_type(element.type),
					layout.get_stride(),
					(const void*)element.offset);
				index++;
				break;
			}
			case ShaderData::Type::Mat3:
			case ShaderData::Type::Mat4:
			{
				uint8_t count = ShaderData::get_count(element.type);
				for (uint8_t i = 0; i < count; i++) {
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(index,
						count,
						ShaderData::get_gl_type(element.type),
						element.normalized ? GL_TRUE : GL_FALSE,
						layout.get_stride(),
						(const void*)(element.offset + sizeof(float) * count * i));
					glVertexAttribDivisor(index, 1);
					index++;
				}
				break;
			}
			default:
				assert(false && "Unknown shader data type!");
		}
	}

	return vertex_array;
}

}