//[INCLUDES]
#include "resources.h"
#include <cerrno>

#include "log.h"

//[IMPLEMENTATION]
Resources::Shader::Shader(const char* _vertex, const char* _fragment)
{
    int _compile;

    GLuint _vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(_vertex_shader, 1, &_vertex, NULL);
    glCompileShader(_vertex_shader);
    glGetShaderiv(_vertex_shader, GL_COMPILE_STATUS, &_compile);
    if(_compile != GL_TRUE)
    {
        Log::Message("Error compiling vertex shader...");
    }

    GLuint _fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(_fragment_shader, 1, &_fragment, NULL);
    glCompileShader(_fragment_shader);
    glGetShaderiv(_fragment_shader, GL_COMPILE_STATUS, &_compile);
    if (_compile != GL_TRUE)
    {
        Log::Message("Error compiling fragment shader...");
    }

    shader_id = glCreateProgram();
    glAttachShader(shader_id, _vertex_shader);
    glAttachShader(shader_id, _fragment_shader);
    glLinkProgram(shader_id);

    glDeleteShader(_vertex_shader);
    glDeleteShader(_fragment_shader);
    Log::Message("New shader with id %d", shader_id);
}
Resources::Shader::~Shader()
{
    Delete();
}

void Resources::Shader::Activate()
{
    glUseProgram(shader_id);
}
void Resources::Shader::Delete()
{
    glDeleteProgram(shader_id);
    Log::Message("Shader with id %d deleted", shader_id);
}
