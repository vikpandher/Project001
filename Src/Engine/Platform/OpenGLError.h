#pragma once

#include "glad/glad.h"

#include "Engine/Logger.h"



#ifdef _DEBUG
#define glChk Project001::LogOpenGLError()
#else
#define glChk
#endif



namespace Project001
{
    inline void LogOpenGLError()
    {
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            Logger::Error("OpenGL::ErrorCode: %d", glGetError());
        }
    }
}