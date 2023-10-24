#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>

namespace toy
{

void Shader::compile(const String& vertex_source, const String& fragment_source)
{
    id = compile_program(vertex_source, fragment_source);
}

void Shader::destroy()
{
    glDeleteProgram(id);
}

void Shader::bind()
{
    glUseProgram(id);
}

void Shader::recompile(const String& vertex_source, const String& fragment_source)
{
    auto program = compile_program(vertex_source, fragment_source);

    if (program) {
        destroy();
        id = program;
    }
}

void Shader::set_float(const String& name, f32 value)
{
    glUniform1f(get_uniform_location(name), value);
}

void Shader::set_int(const String& name, i32 value)
{
    glUniform1i(get_uniform_location(name), value);
}

void Shader::set_ints(const String& name, i32* values, i32 count)
{
    glUniform1iv(get_uniform_location(name), count, values);
}

void Shader::set_vec2(const String& name, f32 x, f32 y)
{
    glUniform2f(get_uniform_location(name), x, y);
}

void Shader::set_vec2(const String& name, const v2& v)
{
    set_vec2(name, v.x, v.y);
}

void Shader::set_vec3(const String& name, f32 x, f32 y, f32 z)
{
    glUniform3f(get_uniform_location(name), x, y, z);
}

void Shader::set_vec3(const String& name, const v3& v)
{
    set_vec3(name, v.x, v.y, v.z);
}

void Shader::set_vec4(const String& name, f32 x, f32 y, f32 z, f32 w)
{
    glUniform4f(get_uniform_location(name), x, y, z, w);
}

void Shader::set_vec4(const String& name, const v4& v)
{
    set_vec4(name, v.x, v.y, v.z, v.w);
}

void Shader::set_mat4(const String& name, const mat4& m)
{
    glUniformMatrix4fv(get_uniform_location(name), 1, false, value_ptr(m));
}

u32 Shader::compile_program(const String& vertex_source, const String& fragment_source)
{
    assert(!vertex_source.empty() && !fragment_source.empty());

    GLuint program = glCreateProgram();
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    if (!compile_shader(vertex_source, vertex_shader)) goto cleanup;
    if (!compile_shader(fragment_source, fragment_shader)) goto cleanup;

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    // link program and check for errors
    {
        glLinkProgram(program);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        int result = -1;
        glGetProgramiv(program, GL_LINK_STATUS, &result);

        if (GL_TRUE != result) {
            char infoLog[2048];
            glGetProgramInfoLog(id, 1024, nullptr, infoLog);
            LOG_ERROR("Failed to compile shader program: {}", infoLog);
            glDeleteProgram(program);
            return 0;
        }
    }

    return program;

cleanup:
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glDeleteProgram(program);
    return 0;
}

bool Shader::compile_shader(const String& source, GLuint shader)
{
    const char* c_str = source.c_str();
    glShaderSource(shader, 1, &c_str, nullptr);
    glCompileShader(shader);
    int result = -1;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (GL_TRUE != result) {
        char infoLog[2048];
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        LOG_ERROR("Failed to compile shader: {}", infoLog);
        return false;
    }

    return true;
}

i32 Shader::get_uniform_location(const String& name)
{
    if (uniform_locations.find(name) != uniform_locations.end()) {
        return uniform_locations[name];
    }

    i32 location;
    location = glGetUniformLocation(id, name.c_str());
    uniform_locations[name] = location;
    return location;
}

}