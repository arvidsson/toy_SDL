#pragma once

#include "SDL.h"

namespace toy
{

struct ShaderData
{
	enum class Type
	{
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};

	static u32 get_size(Type type)
	{
		switch (type) {
			case ShaderData::Type::Float:    return 4;
			case ShaderData::Type::Float2:   return 4 * 2;
			case ShaderData::Type::Float3:   return 4 * 3;
			case ShaderData::Type::Float4:   return 4 * 4;
			case ShaderData::Type::Mat3:     return 4 * 3 * 3;
			case ShaderData::Type::Mat4:     return 4 * 4 * 4;
			case ShaderData::Type::Int:      return 4;
			case ShaderData::Type::Int2:     return 4 * 2;
			case ShaderData::Type::Int3:     return 4 * 3;
			case ShaderData::Type::Int4:     return 4 * 4;
			case ShaderData::Type::Bool:     return 1;
			default:
				assert(false && "Unknown shader data type!");
		}

		return 0;
	}

	static u32 get_count(Type type)
	{
		switch (type) {
			case ShaderData::Type::Float:   return 1;
			case ShaderData::Type::Float2:  return 2;
			case ShaderData::Type::Float3:  return 3;
			case ShaderData::Type::Float4:  return 4;
			case ShaderData::Type::Mat3:    return 3; // 3 * float3
			case ShaderData::Type::Mat4:    return 4; // 4 * float4
			case ShaderData::Type::Int:     return 1;
			case ShaderData::Type::Int2:    return 2;
			case ShaderData::Type::Int3:    return 3;
			case ShaderData::Type::Int4:    return 4;
			case ShaderData::Type::Bool:    return 1;
			default:
				assert(false && "Unknown shader data type!");
		}

		return 0;
	}

	static GLenum get_gl_type(Type type)
	{
		switch (type) {
			case ShaderData::Type::Float:    return GL_FLOAT;
			case ShaderData::Type::Float2:   return GL_FLOAT;
			case ShaderData::Type::Float3:   return GL_FLOAT;
			case ShaderData::Type::Float4:   return GL_FLOAT;
			case ShaderData::Type::Mat3:     return GL_FLOAT;
			case ShaderData::Type::Mat4:     return GL_FLOAT;
			case ShaderData::Type::Int:      return GL_INT;
			case ShaderData::Type::Int2:     return GL_INT;
			case ShaderData::Type::Int3:     return GL_INT;
			case ShaderData::Type::Int4:     return GL_INT;
			case ShaderData::Type::Bool:     return GL_BOOL;
			default:
				assert(false && "Unknown shader data type!");
		}

		return 0;
	}
};

}