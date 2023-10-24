#pragma once

#include "Core.h"
#include "SDL.h"

namespace toy
{

class Shader final
{
public:
    void compile(const String& vertex_source, const String& fragment_source);
    void destroy();
    void bind();
    void recompile(const String& vertex_source, const String& fragment_source);

    void set_float(const String& name, f32 value);
    void set_int(const String& name, i32 value);
    void set_ints(const String& name, i32* values, i32 count);
    void set_vec2(const String& name, f32 x, f32 y);
    void set_vec2(const String& name, const v2& v);
    void set_vec3(const String& name, f32 x, f32 y, f32 z);
    void set_vec3(const String& name, const v3& v);
    void set_vec4(const String& name, f32 x, f32 y, f32 z, f32 w);
    void set_vec4(const String& name, const v4& v);
    void set_mat4(const String& name, const mat4& m);

private:
    u32 id = 0;
    Dictionary<String, i32> uniform_locations;

    u32 compile_program(const String& vertex_source, const String& fragment_source);
    bool compile_shader(const String& source, GLuint shader);
    i32 get_uniform_location(const String& name);
};

}