#pragma once

#include <string>

#include "glm/glm.hpp"



namespace Project001
{
	class Shader
	{
	public:
		virtual unsigned int GetId() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetBool(const std::string& name, bool value) const = 0;
		virtual void SetInt(const std::string& name, int value) const = 0;
		virtual void SetFloat(const std::string& name, float value) const = 0;
		virtual void SetVec2(const std::string& name, const glm::vec2& value) const = 0;
		virtual void SetVec3(const std::string& name, const glm::vec3& value) const = 0;
		virtual void SetVec4(const std::string& name, const glm::vec4& value) const = 0;
		virtual void SetMat2(const std::string& name, const glm::mat2& mat) const = 0;
		virtual void SetMat3(const std::string& name, const glm::mat3& mat) const = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& mat) const = 0;

	protected:

	private:
		
	};
}