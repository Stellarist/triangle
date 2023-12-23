#pragma once

#include <print>
#include <string>
#include <glad/glad.h>

#define ASSERT(x) \
    if(!(x)) __builtin_trap();

#define GLCALL(x) \
    glClearError();\
    x;\
    ASSERT(glLogCall(#x, __FILE__, __LINE__));

inline void glClearError()
{
    while(glGetError()!=GL_NO_ERROR);
}

inline bool glLogCall(const char* function, const char* file, int line)
{
    while(GLenum error=glGetError()){
        std::println("Error:{} {} {}: {}", error, function, file, line);
        return false;
    }
    return true;
}

inline void glPrintError(std::string function)
{
    GLenum error=glGetError();
    std::println("Error: {} in {}", error, function);
}

inline void GLAPIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
    std::println("OpenGL Debug: {}", message);
}
