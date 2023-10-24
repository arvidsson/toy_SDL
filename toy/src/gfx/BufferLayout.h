#pragma once

#include "Core.h"
#include "ShaderData.h"

namespace toy
{

struct BufferElement
{
	ShaderData::Type type;
	String name;
	u32 size;
	u32 offset;
	bool normalized;

	BufferElement(ShaderData::Type type, const String& name, bool normalized = false) : 
		type(type), name(name), size(ShaderData::get_size(type)), offset(0), normalized(normalized) {}
};

class BufferLayout
{
public:
	BufferLayout(std::initializer_list<BufferElement> elements) : elements(elements)
	{
		calculate_offsets_and_stride();
	}

	u32 get_stride() const { return stride; }
	const Vector<BufferElement>& get_elements() const { return elements; }

	Vector<BufferElement>::iterator begin() { return elements.begin(); }
	Vector<BufferElement>::iterator end() { return elements.end(); }
	Vector<BufferElement>::const_iterator begin() const { return elements.begin(); }
	Vector<BufferElement>::const_iterator end() const { return elements.end(); }

private:
	std::vector<BufferElement> elements;
	u32 stride = 0;

	BufferLayout() = default;

	void calculate_offsets_and_stride()
	{
		u32 offset = 0;
		stride = 0;

		for (auto& element : elements) {
			element.offset = offset;
			offset += element.size;
			stride += element.size;
		}
	}
};

}