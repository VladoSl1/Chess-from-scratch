#pragma once

/*
Base.h:
    - includes all necesary files to work with OpenGL
    - defines basic macro that checks if OpenGL function call raised an error
    - defines prototypes of all core functions
*/


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ErrorHandler.h"

#include <iostream>

#ifdef _DEBUG
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define GLCall(x) x;
#endif

inline void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

inline bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function
                  << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}


namespace graphics
{
    namespace core
    {
        class Shader;
        class Collider;
        class IndexBuffer;
        class VertexArray;
        class VertexBuffer;
        class BaseRenderer;
        class VertexBufferLayout;
    }
}