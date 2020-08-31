#pragma once

#include "glad/glad.h""

#include "../Engine/Shader.h"



namespace Project001
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader();

		unsigned int GetId() const override;

		void Bind() const override;
		void Unbind() const override;

		void SetBool(const std::string& name, bool value) const override;
		void SetInt(const std::string& name, int value) const override;
		void SetFloat(const std::string& name, float value) const override;
		void SetVec2(const std::string& name, const glm::vec2& value) const override;
		void SetVec3(const std::string& name, const glm::vec3& value) const override;
		void SetVec4(const std::string& name, const glm::vec4& value) const override;
		void SetMat2(const std::string& name, const glm::mat2& mat) const override;
		void SetMat3(const std::string& name, const glm::mat3& mat) const override;
		void SetMat4(const std::string& name, const glm::mat4& mat) const override;

	protected:
		unsigned int id_;

	private:
		
	};

	inline unsigned int OpenGLShader::GetId() const
	{
		return id_;
	}

	inline void OpenGLShader::Bind() const
	{
		glUseProgram(id_);
	}

	inline void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	inline void OpenGLShader::SetBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(id_, name.c_str()), (int)value);
	}

	inline void OpenGLShader::SetInt(const std::string& name, int value) const
	{
		glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
	}

	inline void OpenGLShader::SetFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
	}

	inline void OpenGLShader::SetVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
	}

	inline void OpenGLShader::SetVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
	}

	inline void OpenGLShader::SetVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
	}

	inline void OpenGLShader::SetMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	inline void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	inline void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
}