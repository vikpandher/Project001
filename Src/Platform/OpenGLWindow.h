#pragma once

#include <string>

#include "Engine/Window.h"



struct GLFWwindow;

namespace Project001
{
	class OpenGLShader;
	class OpenGLTexture;
	
	class OpenGLWindow : public Window
	{
	public:
		OpenGLWindow(const char* title, int width, int height);
		~OpenGLWindow();

		OpenGLWindow(OpenGLWindow& other) = delete;
		void operator=(const OpenGLWindow&) = delete;

		void Render(const RenderData* renderData) const override;

		void AddTexture(const TextureData* textureData, unsigned int index) override;

		void PollEvents() override;

		void SetEventCallback(const std::function<void(Event&)>& callback) override;

		// Setting numerator and denominator to -1 unlocks the aspect ratio.
		void SetAspectRatio(int numerator, int denominator);

		void SetSize(int width, int height) override;

		// Time measurements are in seconds
		void SetTime(const double time) override;
		double GetTime() const override;

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	protected:
		static int s_glfwWindowCount_;

		// determines the size of the index and vertex buffers
		static const unsigned int s_bufferCapacity_ = 36 * 5;
		static const unsigned int s_numberOfTextureSlots_ = 16;

		static const unsigned int s_numberOfPointLights_ = 8;
		static const unsigned int s_numberOfSpotLights_ = 4;

		GLFWwindow* glfwWindowPtr_;

		struct WindowData
		{
			WindowData()
				: width(0)
				, height(0)
				, vSyncEnabled(false)
			{}

			std::string title;
			unsigned int width;
			unsigned int height;

			std::function<void(Event&)> EventCallback;

			bool vSyncEnabled;

		} windowData_;

		OpenGLShader* shaderPtr_;

		// this holds the buffer's id
		// the buffer holds the blob of data that will be displayed
		unsigned int vertexBufferId_;

		// this holds the index buffer's id
		unsigned int indexBufferId_;

		// this holds the vertex array's id
		// the vertex array holds information about the size, shape, and type of array
		unsigned int vertexArrayId_;

		OpenGLTexture* texturePtrs_[s_numberOfTextureSlots_];

	private:

	};

	// public: -----------------------------------------------------------------

	inline void OpenGLWindow::SetEventCallback(const std::function<void(Event&)>& callback)
	{
		windowData_.EventCallback = callback;
	}

	inline bool OpenGLWindow::IsVSync() const
	{
		return windowData_.vSyncEnabled;
	}
}