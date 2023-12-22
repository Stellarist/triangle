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

void glClearError()
{
    while(glGetError()!=GL_NO_ERROR);
}

bool glLogCall(const char* function, const char* file, int line)
{
    while(GLenum error=glGetError()){
        std::println("Error:{} {} {}: {}", error, function, file, line);
        return false;
    }
    return true;
}

void glPrintError(std::string function)
{
    GLenum error=glGetError();
    // if(error!=GL_NO_ERROR)
    std::println("Error: {} in {}", error, function);
}
