// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-13

#include "OpenGL_Shader.h"

#include "glad/glad.h"

#include "Logger.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    OpenGL_Shader::OpenGL_Shader(
        const char* vertexShaderCode,
        const char* fragmentShaderCode)
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
            LOG_ERROR_F("Shader program (" << programId_ << ") linking failed:\n" << INDENT << infoLog);
        }
        glValidateProgram(programId_);
        glGetProgramiv(programId_, GL_VALIDATE_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(programId_, 512, NULL, infoLog);
            LOG_ERROR_F("Shader program (" << programId_ << ") validation failed:\n" << INDENT << infoLog);
        }

        glDetachShader(programId_, vertexShaderId);
        glDetachShader(programId_, fragmentShaderId);

        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
    }

    OpenGL_Shader::OpenGL_Shader(
        const char* vertexShaderCode,
        const char* geometryShaderCode,
        const char* fragmentShaderCode)
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
            LOG_ERROR_F("Shader program (" << programId_ << ") linking failed:\n" << INDENT << infoLog);
        }
        glValidateProgram(programId_);
        glGetProgramiv(programId_, GL_VALIDATE_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(programId_, 512, NULL, infoLog);
            LOG_ERROR_F("Shader program (" << programId_ << ") validation failed:\n" << INDENT << infoLog);
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

    void OpenGL_Shader::SetBool(
        const char* name,
        bool value) const
    {
        glUniform1i(glGetUniformLocation(programId_, name), static_cast<int>(value));
    }

    void OpenGL_Shader::SetInt(
        const char* name,
        int value) const
    {
        glUniform1i(glGetUniformLocation(programId_, name), value);
    }

    void OpenGL_Shader::SetFloat(
        const char* name,
        float value) const
    {
        glUniform1f(glGetUniformLocation(programId_, name), value);
    }

    void OpenGL_Shader::SetVec2(
        const char* name,
        const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(programId_, name), 1, &value[0]);
    }

    void OpenGL_Shader::SetVec2(
        const char* name,
        float x,
        float y) const
    {
        glUniform2f(glGetUniformLocation(programId_, name), x, y);
    }

    void OpenGL_Shader::SetVec3(
        const char* name,
        const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(programId_, name), 1, &value[0]);
    }

    void OpenGL_Shader::SetVec3(
        const char* name,
        float x,
        float y,
        float z) const
    {
        glUniform3f(glGetUniformLocation(programId_, name), x, y, z);
    }

    void OpenGL_Shader::SetVec4(
        const char* name,
        const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(programId_, name), 1, &value[0]);
    }

    void OpenGL_Shader::SetVec4(
        const char* name,
        float x,
        float y,
        float z,
        float w)
    {
        glUniform4f(glGetUniformLocation(programId_, name), x, y, z, w);
    }

    void OpenGL_Shader::SetMat2(
        const char* name,
        const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(programId_, name), 1, GL_FALSE, &mat[0][0]);
    }

    void OpenGL_Shader::SetMat3(
        const char* name,
        const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(programId_, name), 1, GL_FALSE, &mat[0][0]);
    }

    void OpenGL_Shader::SetMat4(const char* name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(programId_, name), 1, GL_FALSE, &mat[0][0]);
    }

    // private -----------------------------------------------------------------

    glm::uint OpenGL_Shader::CreateShader(
        glm::uint shaderType,
        const char* shaderSource)
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
                LOG_ERROR_F("Vertex shader (" << shaderId << ") compilation failed:\n" << INDENT << infoLog);
            }
            else if (shaderType == GL_GEOMETRY_SHADER)
            {
                LOG_ERROR_F("Geometry shader (" << shaderId << ") compilation failed:\n" << INDENT << infoLog);
            }
            else if (shaderType == GL_FRAGMENT_SHADER)
            {
                LOG_ERROR_F("Fragment shader (" << shaderId << ") compilation failed:\n" << INDENT << infoLog);
            }
            else
            {
                LOG_ERROR_F("Unknown shader (" << shaderId << ") compilation failed:\n" << INDENT << infoLog);
            }
        }
        return shaderId;
    }

    void OpenGL_Shader::OutputActiveAttributesAndUniforms()
    {
        GLuint i;
        GLint count;

        GLint size; // size of the variable
        GLenum type; // type of the variable (float, vec3 or mat4, etc)

        const GLsizei bufferCapacity = 128; // maximum name length
        GLchar name[bufferCapacity]; // variable name in GLSL
        GLsizei length; // name length

        // ATTRIBUTES
        glGetProgramiv(programId_, GL_ACTIVE_ATTRIBUTES, &count);

        Logger log(LOG_LEVEL_INFO);
        log << "Active Attributes: " << count;

        for (i = 0; i < count; ++i)
        {
            glGetActiveAttrib(programId_, i, bufferCapacity, &length, &size, &type, name);

            log << "\nAttribute: " << i << " Type: " << type << " Name: " << name;
        }

        // UNIFORMS
        glGetProgramiv(programId_, GL_ACTIVE_UNIFORMS, &count);
        log << "\nActive Uniforms: " << count;

        for (i = 0; i < count; ++i)
        {
            glGetActiveUniform(programId_, i, bufferCapacity, &length, &size, &type, name);

            log << "\nUniform: " << i << " Type: " << type << " Name: " << name;
        }

        log << Logger::endl;
    }
}