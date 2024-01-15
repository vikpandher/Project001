#pragma once

#include "glad/glad.h"

#include "Logger.h"


#ifndef NDEBUG
#define glLogError() Project001::LogOpenGLError(__FILENAME__ , __LINE__)
#elif
#define glLogError()
#endif


namespace Project001
{
    inline void LogOpenGLError(const char* file, int line)
    {
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            Logger::Error("OpenGL ErrorCode: %s %d %d", file, line, error);
        }
    }
}