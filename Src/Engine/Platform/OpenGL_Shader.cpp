#include "OpenGL_Shader.h"

#include "glad/glad.h"

#include "Engine/Logger.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    OpenGL_Shader::OpenGL_Shader(const char* vertexShaderCode, const char* fragmentShaderCode)
    {
        GLuint vertexShaderId = CreateShader(GL_VERTEX_SHADER, vertexShaderCode);
        GLuint fragmentShaderId = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderCode);

        programId_ = glCreateProgram();
        glAttachShader(programId_, vertexShaderId);
        glAttachShader(programId_, fragmentShaderId);
        glLinkProgram(programId_);
        GLint success;
        GLchar infoLog[512];
        glGetProgramiv(programId_, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(programId_, 512, NULL, infoLog);
            _LOG_ERROR("SHADER::PROGRAM::LINKING_FAILED");
            _LOG_ERROR(infoLog);
        }
        glValidateProgram(programId_);
        glGetProgramiv(programId_, GL_VALIDATE_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(programId_, 512, NULL, infoLog);
            _LOG_ERROR("SHADER::PROGRAM::VALIDATE_FAILED");
            _LOG_ERROR(infoLog);
        }

        glDetachShader(programId_, vertexShaderId);
        glDetachShader(programId_, fragmentShaderId);

        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
    }

    OpenGL_Shader::OpenGL_Shader(const char* vertexShaderCode, const char* geometryShaderCode, const char* fragmentShaderCode)
    {
        GLuint vertexShaderId = CreateShader(GL_VERTEX_SHADER, vertexShaderCode);
        GLuint geometryShaderId = CreateShader(GL_GEOMETRY_SHADER, geometryShaderCode);
        GLuint fragmentShaderId = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderCode);

        programId_ = glCreateProgram();
        glAttachShader(programId_, vertexShaderId);
        glAttachShader(programId_, geometryShaderId);
        glAttachShader(programId_, fragmentShaderId);
        glLinkProgram(programId_);
        GLint success;
        GLchar infoLog[512];
        glGetProgramiv(programId_, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(programId_, 512, NULL, infoLog);
            _LOG_ERROR("SHADER::PROGRAM::LINKING_FAILED");
            _LOG_ERROR(infoLog);
        }
        glValidateProgram(programId_);
        glGetProgramiv(programId_, GL_VALIDATE_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(programId_, 512, NULL, infoLog);
            _LOG_ERROR("SHADER::PROGRAM::VALIDATE_FAILED");
            _LOG_ERROR(infoLog);
        }

        glDetachShader(programId_, vertexShaderId);
        glDetachShader(programId_, geometryShaderId);
        glDetachShader(programId_, fragmentShaderId);

        glDeleteShader(vertexShaderId);
        glDeleteShader(geometryShaderId);
        glDeleteShader(fragmentShaderId);
    }

    OpenGL_Shader::~OpenGL_Shader()
    {
        glDeleteProgram(programId_);
    }

    void OpenGL_Shader::Use() const
    {
        glUseProgram(programId_);
    }

    void OpenGL_Shader::SetBool(const char* name, bool value) const
    {
        glUniform1i(glGetUniformLocation(programId_, name), (int)value);
    }

    void OpenGL_Shader::SetInt(const char* name, int value) const
    {
        glUniform1i(glGetUniformLocation(programId_, name), value);
    }

    void OpenGL_Shader::SetFloat(const char* name, float value) const
    {
        glUniform1f(glGetUniformLocation(programId_, name), value);
    }

    void OpenGL_Shader::SetVec2(const char* name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(programId_, name), 1, &value[0]);
    }

    void OpenGL_Shader::SetVec2(const char* name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(programId_, name), x, y);
    }

    void OpenGL_Shader::SetVec3(const char* name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(programId_, name), 1, &value[0]);
    }

    void OpenGL_Shader::SetVec3(const char* name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(programId_, name), x, y, z);
    }

    void OpenGL_Shader::SetVec4(const char* name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(programId_, name), 1, &value[0]);
    }

    void OpenGL_Shader::SetVec4(const char* name, float x, float y, float z, float w)
    {
        glUniform4f(glGetUniformLocation(programId_, name), x, y, z, w);
    }

    void OpenGL_Shader::SetMat2(const char* name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(programId_, name), 1, GL_FALSE, &mat[0][0]);
    }

    void OpenGL_Shader::SetMat3(const char* name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(programId_, name), 1, GL_FALSE, &mat[0][0]);
    }

    void OpenGL_Shader::SetMat4(const char* name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(programId_, name), 1, GL_FALSE, &mat[0][0]);
    }

    // protected ---------------------------------------------------------------

    // private -----------------------------------------------------------------

    glm::uint OpenGL_Shader::CreateShader(glm::uint shaderType, const char* shaderSource)
    {
        GLuint shaderId = glCreateShader(shaderType);
        glShaderSource(shaderId, 1, &shaderSource, NULL);
        glCompileShader(shaderId);
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
            if (shaderType == GL_VERTEX_SHADER)
            {
                _LOG_ERROR("VERTEX_SHADER::COMPILATION_FAILED");
            }
            else if (shaderType == GL_GEOMETRY_SHADER)
            {
                _LOG_ERROR("GEOMETRY_SHADER::COMPILATION_FAILED");
            }
            else if (shaderType == GL_FRAGMENT_SHADER)
            {
                _LOG_ERROR("FRAGMET_SHADER::COMPILATION_FAILED");
            }
            else
            {
                _LOG_ERROR("UNKNOWN_SHADER::COMPILATION_FAILED");
            }
            _LOG_ERROR(infoLog);
        }
        return shaderId;
    }

    void OpenGL_Shader::OutputActiveAttributesAndUniforms()
    {
        GLint i;
        GLint count;

        GLint size; // size of the variable
        GLenum type; // type of the variable (float, vec3 or mat4, etc)

        const GLsizei bufferCapacity = 128; // maximum name length
        GLchar name[bufferCapacity]; // variable name in GLSL
        GLsizei length; // name length

        // ATTRIBUTES
        glGetProgramiv(programId_, GL_ACTIVE_ATTRIBUTES, &count);
        _LOG_MESSAGE("Active Attributes: %d", count);

        for (i = 0; i < count; i++)
        {
            glGetActiveAttrib(programId_, (GLuint)i, bufferCapacity, &length, &size, &type, name);

            _LOG_MESSAGE("Attribute #%d Type: %u Name: %s", i, type, name);
        }

        // UNIFORMS
        glGetProgramiv(programId_, GL_ACTIVE_UNIFORMS, &count);
        _LOG_MESSAGE("Active Uniforms: %d", count);

        for (i = 0; i < count; i++)
        {
            glGetActiveUniform(programId_, (GLuint)i, bufferCapacity, &length, &size, &type, name);

            _LOG_MESSAGE("Uniform #%d Type: %u Name: %s", i, type, name);
        }

        _LOG_MESSAGE("");
    }
}