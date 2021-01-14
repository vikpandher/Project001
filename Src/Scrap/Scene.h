#pragma once

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

// temp
#include "ModelUtilities.h"



namespace Project001
{	
	class Scene
	{
	public:
		Scene();
		~Scene();

		Scene(Scene& other) = delete;
		void operator=(const Scene&) = delete;

		virtual void Initialize(Application* applicationPtr) = 0;

		virtual void OnEvent(Event& event) = 0;

		virtual void OnUpdate(double frameTimestep) = 0;

		const glm::mat4& GetViewMatrix() const;
		const glm::vec3& GetViewPosition() const;
		const glm::mat4& GetProjectionMatrix() const;

		const std::vector<VertexData>& GetVertexBuffer() const;
		const std::vector<glm::uint> GetIndexBuffer() const;

		const DirectionalLight& GetDirectionalLight() const;
		const std::vector<PointLight>& GetPointLights() const;
		const std::vector<SpotLight>& GetSpotLights() const;

	protected:
	
	private:
		glm::mat4 viewMatrix_;
		glm::vec3 viewPosition_;

		glm::mat4 projectionMatrix_;

		DirectionalLight directionalLight_;
		std::vector<PointLight> pointLights_;
		std::vector<SpotLight> spotLights_;

		std::vector<VertexData> vertexBuffer_;
		std::vector<glm::uint> indexBuffer_;
	};

	inline const glm::mat4& Scene::GetViewMatrix() const
	{
		return viewMatrix_;
	}

	inline const glm::vec3& Scene::GetViewPosition() const
	{
		return viewPosition_;
	}

	inline const glm::mat4& Scene::GetProjectionMatrix() const
	{
		return projectionMatrix_;
	}

	inline const std::vector<VertexData>& Scene::GetVertexBuffer() const
	{
		return vertexBuffer_;
	}

	inline const std::vector<glm::uint> Scene::GetIndexBuffer() const
	{
		return indexBuffer_;
	}

	inline const DirectionalLight& Scene::GetDirectionalLight() const
	{
		return directionalLight_;
	}

	inline const std::vector<PointLight>& Scene::GetPointLights() const
	{
		return pointLights_;
	}

	inline const std::vector<SpotLight>& Scene::GetSpotLights() const
	{
		return spotLights_;
	}
}