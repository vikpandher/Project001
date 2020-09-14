#include "OpenGLShader.h"

#include "glad/glad.h"

#include "../Engine/Logger.h"



namespace Project001
{
	// public ------------------------------------------------------------------
	
	OpenGLShader::OpenGLShader(const char* vertexShaderCode, const char* fragmentShaderCode)
	{
		GLuint vertexShaderId = createShader(GL_VERTEX_SHADER, &vertexShaderCode);
		GLuint fragmentShaderId = createShader(GL_FRAGMENT_SHADER, &fragmentShaderCode);

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
			Project001::Logger::Error("ERROR::SHADER::PROGRAM::LINKING_FAILED");
			Project001::Logger::Error(infoLog);
		}

		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);
	}

	OpenGLShader::OpenGLShader(const char* vertexShaderCode, const char* geometryShaderCode, const char* fragmentShaderCode)
	{
		GLuint vertexShaderId = createShader(GL_VERTEX_SHADER, &vertexShaderCode);
		GLuint geometryShaderId = createShader(GL_GEOMETRY_SHADER, &geometryShaderCode);
		GLuint fragmentShaderId = createShader(GL_FRAGMENT_SHADER, &fragmentShaderCode);

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
			Project001::Logger::Error("ERROR::SHADER::PROGRAM::LINKING_FAILED");
			Project001::Logger::Error(infoLog);
		}

		glDeleteShader(vertexShaderId);
		glDeleteShader(geometryShaderId);
		glDeleteShader(fragmentShaderId);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(programId_);
	}

	void OpenGLShader::Use() const
	{
		glUseProgram(programId_);
	}

	void OpenGLShader::SetBool(const char* name, bool value) const
	{
		glUniform1i(glGetUniformLocation(programId_, name), (int)value);
	}

	void OpenGLShader::SetInt(const char* name, int value) const
	{
		glUniform1i(glGetUniformLocation(programId_, name), value);
	}

	void OpenGLShader::SetFloat(const char* name, float value) const
	{
		glUniform1f(glGetUniformLocation(programId_, name), value);
	}

	void OpenGLShader::SetVec2(const char* name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(programId_, name), 1, &value[0]);
	}

	void OpenGLShader::SetVec2(const char* name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(programId_, name), x, y);
	}

	void OpenGLShader::SetVec3(const char* name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(programId_, name), 1, &value[0]);
	}

	void OpenGLShader::SetVec3(const char* name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(programId_, name), x, y, z);
	}

	void OpenGLShader::SetVec4(const char* name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(programId_, name), 1, &value[0]);
	}

	void OpenGLShader::SetVec4(const char* name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(programId_, name), x, y, z, w);
	}

	void OpenGLShader::SetMat2(const char* name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(programId_, name), 1, GL_FALSE, &mat[0][0]);
	}

	void OpenGLShader::SetMat3(const char* name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(programId_, name), 1, GL_FALSE, &mat[0][0]);
	}

	void OpenGLShader::SetMat4(const char* name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(programId_, name), 1, GL_FALSE, &mat[0][0]);
	}

	// protected ---------------------------------------------------------------

	// private -----------------------------------------------------------------

	glm::uint OpenGLShader::createShader(glm::uint shaderType, const char** shaderSource)
	{
		GLuint shaderId = glCreateShader(shaderType);
		glShaderSource(shaderId, 1, shaderSource, NULL);
		glCompileShader(shaderId);
		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
			if (shaderType == GL_VERTEX_SHADER)
			{
				Project001::Logger::Error("VERTEX_SHADER::COMPILATION_FAILED");
			}
			else if (shaderType == GL_GEOMETRY_SHADER)
			{
				Project001::Logger::Error("GEOMETRY_SHADER::COMPILATION_FAILED");
			}
			else if (shaderType == GL_FRAGMENT_SHADER)
			{
				Project001::Logger::Error("FRAGMET_SHADER::COMPILATION_FAILED");
			}
			else
			{
				Project001::Logger::Error("UNKNOWN_SHADER::COMPILATION_FAILED");
			}
			Project001::Logger::Error(infoLog);
		}
		return shaderId;
	}

}