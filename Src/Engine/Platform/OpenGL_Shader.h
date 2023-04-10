#pragma once

#include "glm/glm.hpp"



namespace Project001
{
    class OpenGL_Shader
    {
    public:
        OpenGL_Shader(
            const char* vertexShaderCode,
            const char* fragmentShaderCode);

        OpenGL_Shader(
            const char* vertexShaderCode,
            const char* geometryShaderCode,
            const char* fragmentShaderCode);

        ~OpenGL_Shader();

        OpenGL_Shader(OpenGL_Shader& other) = delete;
        void operator=(const OpenGL_Shader&) = delete;

        void Use() const;

        void SetBool(
            const char* name,
            bool value) const;

        void SetInt(
            const char* name,
            int value) const;

        void SetFloat(
            const char* name,
            float value) const;

        void SetVec2(
            const char* name,
            const glm::vec2& value) const;

        void SetVec2(
            const char* name,
            float x,
            float y) const;

        void SetVec3(
            const char* name,
            const glm::vec3& value) const;

        void SetVec3(
            const char* name,
            float x,
            float y,
            float z) const;

        void SetVec4(
            const char* name,
            const glm::vec4& value) const;

        void SetVec4(
            const char* name,
            float x,
            float y,
            float z,
            float w);

        void SetMat2(
            const char* name,
            const glm::mat2& mat) const;

        void SetMat3(
            const char* name,
            const glm::mat3& mat) const;

        void SetMat4(
            const char* name,
            const glm::mat4& mat) const;

    protected:
        glm::uint programId_;

    private:
        glm::uint CreateShader(
            glm::uint shaderType,
            const char* shaderSource);

        void OutputActiveAttributesAndUniforms();
    };
}