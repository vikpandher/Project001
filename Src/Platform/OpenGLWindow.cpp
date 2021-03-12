#include "OpenGLWindow.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/RenderData.h"
#include "Engine/TextureData.h"

#include "Platform/OpenGLShader.h"
#include "Platform/OpenGLTexture.h"
#include "Platform/Shaders.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    OpenGLWindow::OpenGLWindow(const char* title, int width, int height)
        : glfwWindowPtr_(nullptr)
    {
        windowData_.title = title;
        windowData_.width = width;
        windowData_.height = height;

        if (s_glfwWindowCount_ == 0)
        {
            if (!glfwInit())
            {
                Logger::Error("Could not initalize GLFW!");
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 4
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 6
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            glfwSetErrorCallback([](int errorCode, const char* description)
                {
                    Logger::Error("GLFW Error: (%d) %s", errorCode, description);
                });
        }

        glfwWindowPtr_ = glfwCreateWindow(width, height, title, NULL, NULL);
        s_glfwWindowCount_++;

        glfwSetWindowUserPointer(glfwWindowPtr_, &windowData_);

        glfwMakeContextCurrent(glfwWindowPtr_);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            Logger::Error("Failed to initialize Glad!");
        }

        Logger::Message("OpenGL Info:");
        Logger::Message("    Vendor: %s", glGetString(GL_VENDOR));
        Logger::Message("    Renderer: %s", glGetString(GL_RENDERER));
        Logger::Message("    Version: %s", glGetString(GL_VERSION));

        SetVSync(true);

        // This is enabled so when glViewport is used to set the viewportt size,
        // glScissor can be used to limit drawing to within the viewport.
        glEnable(GL_SCISSOR_TEST);

        // NOTES:
        // GLFW Callback Functions:
        // ---------------------------------------------------------------------
        // ERROR CALLBACKS: https://www.glfw.org/docs/3.3/group__init.html
        // GLFWerrorfun glfwSetErrorCallback (GLFWerrorfun callback) ---implemented---
        //
        // INPUT CALLBACKS: https://www.glfw.org/docs/3.3/group__input.html
        // GLFWkeyfun glfwSetKeyCallback (GLFWwindow *window, GLFWkeyfun callback) ---implemented---
        // GLFWcharfun glfwSetCharCallback (GLFWwindow *window, GLFWcharfun callback)
        // GLFWcharmodsfun glfwSetCharModsCallback (GLFWwindow *window, GLFWcharmodsfun callback)
        // GLFWmousebuttonfun glfwSetMouseButtonCallback (GLFWwindow *window, GLFWmousebuttonfun callback) ---implemented---
        // GLFWcursorposfun glfwSetCursorPosCallback (GLFWwindow *window, GLFWcursorposfun callback) ---implemented---
        // GLFWcursorenterfun glfwSetCursorEnterCallback (GLFWwindow *window, GLFWcursorenterfun callback) ---implemented---
        // GLFWscrollfun glfwSetScrollCallback (GLFWwindow *window, GLFWscrollfun callback) ---implemented---
        // GLFWdropfun glfwSetDropCallback (GLFWwindow *window, GLFWdropfun callback)
        // GLFWjoystickfun glfwSetJoystickCallback (GLFWjoystickfun callback)
        //
        // MONITOR CALLBACKS: https://www.glfw.org/docs/3.3/group__monitor.html
        // GLFWmonitorfun glfwSetMonitorCallback (GLFWmonitorfun callback)
        //
        // WINDOW CALLBACKS: https://www.glfw.org/docs/3.3/group__window.html
        // GLFWwindowposfun glfwSetWindowPosCallback (GLFWwindow *window, GLFWwindowposfun callback)
        // GLFWwindowsizefun glfwSetWindowSizeCallback (GLFWwindow *window, GLFWwindowsizefun callback)
        // GLFWwindowclosefun glfwSetWindowCloseCallback (GLFWwindow *window, GLFWwindowclosefun callback) ---implemented---
        // GLFWwindowrefreshfun glfwSetWindowRefreshCallback (GLFWwindow *window, GLFWwindowrefreshfun callback)
        // GLFWwindowfocusfun glfwSetWindowFocusCallback (GLFWwindow *window, GLFWwindowfocusfun callback) ---implemented---
        // GLFWwindowiconifyfun glfwSetWindowIconifyCallback (GLFWwindow *window, GLFWwindowiconifyfun callback)
        // GLFWwindowmaximizefun glfwSetWindowMaximizeCallback (GLFWwindow *window, GLFWwindowmaximizefun callback)
        // GLFWframebuffersizefun glfwSetFramebufferSizeCallback (GLFWwindow *window, GLFWframebuffersizefun callback) ---implemented---
        // GLFWwindowcontentscalefun glfwSetWindowContentScaleCallback (GLFWwindow *window, GLFWwindowcontentscalefun callback)

        glfwSetKeyCallback(glfwWindowPtr_, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                KeyEvent event((KeyCode)key, (ButtonAction)action, (KeyModifier)mods);
                data.EventCallback(event);
            });

        glfwSetMouseButtonCallback(glfwWindowPtr_, [](GLFWwindow* window, int button, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                MouseButtonEvent event((MouseButton)button, (ButtonAction)action, (KeyModifier)mods);
                data.EventCallback(event);
            });

        glfwSetCursorPosCallback(glfwWindowPtr_, [](GLFWwindow* window, double xpos, double ypos)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                CursorPositionEvent event((float)xpos, (float)ypos);
                data.EventCallback(event);
            });

        glfwSetCursorEnterCallback(glfwWindowPtr_, [](GLFWwindow* window, int entered)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                CursorEnterEvent event((bool)entered);
                data.EventCallback(event);
            });

        glfwSetScrollCallback(glfwWindowPtr_, [](GLFWwindow* window, double xoffset, double yoffset)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                ScrollEvent event((float)xoffset, (float)yoffset);
                data.EventCallback(event);
            });

        glfwSetWindowCloseCallback(glfwWindowPtr_, [](GLFWwindow* window)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                WindowCloseEvent event;
                data.EventCallback(event);
            });

        glfwSetWindowFocusCallback(glfwWindowPtr_, [](GLFWwindow* window, int focused)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                WindowFocusEvent event((bool)focused);
                data.EventCallback(event);
            });

        glfwSetFramebufferSizeCallback(glfwWindowPtr_, [](GLFWwindow* window, int width, int height)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                FrameBufferSizeEvent event(width, height);
                data.EventCallback(event);
            });

        // configure global opengl state
        // ---------------------------------------------------------------------
        // enable using the z buffer
        glEnable(GL_DEPTH_TEST);

        // enable writing to the depth buffer
        glDepthMask(GL_TRUE);

        // draw as wireframe
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // cull backfaces
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        shaderPtr_ = new OpenGLShader(g_vertexShaderSource01_, g_fragmentShaderSource01_);

        // NOTE:
        // glBindVertex Array doesn't ALWAYS need to come before glBindBuffer,
        // but there are situations when it does.
        //
        // OpenGL calls are executred in the order they are issued. So
        // glVertexArrtribPointer interprets parameters relative to the
        // currenlty bound buffer.
        //
        // There is an exception to this. Binding the GL_ELEMENT_ARRAY_BUFFER is
        // saved to the current Vertex Array Object at the time it is bound. The
        // VAO must be bound first.

        // ---------------------------------------------------------------------
        // generate an id (name) for a new vertex array object
        glGenVertexArrays(1, &vertexArrayId_);

        // generate an id (name) for a vertex buffer object
        glGenBuffers(1, &vertexBufferId_);

        // generate an id (name) for the index buffer
        glGenBuffers(1, &indexBufferId_);

        // make the vertex arrat object active, create it if necessary
        glBindVertexArray(vertexArrayId_);

        // make the buffer the active array buffer, creating it if necessary
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId_);

        // set the size of the active array buffer
        // (target, size, data, usage)
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * s_bufferCapacity_, NULL, GL_DYNAMIC_DRAW);

        // attach the active buffer to the active array with the given attributes
        // (index, size, type, normalized, stride, pointer)

        GLuint positionAttributeIndex = 0;
        GLuint normalAttributeIndex = 1;
        GLuint colorAttributeIndex = 2;
        GLuint textureCoordinateAttributeIndex = 3;
        GLuint textureIndexAttributeIndex = 4;
        GLuint specularIndexAttributeIndex = 5;
        GLuint shininessAttributeIndex = 6;
        GLuint translationAttributeIndex = 7;
        GLuint orientationAttributeIndex = 8;

        unsigned long long attributeOffset = 0;

        glVertexAttribPointer(positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(positionAttributeIndex);
        attributeOffset += sizeof(glm::vec3);

        glVertexAttribPointer(normalAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(normalAttributeIndex);
        attributeOffset += sizeof(glm::vec3);

        glVertexAttribPointer(colorAttributeIndex, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(colorAttributeIndex);
        attributeOffset += sizeof(glm::vec4);

        glVertexAttribPointer(textureCoordinateAttributeIndex, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(textureCoordinateAttributeIndex);
        attributeOffset += sizeof(glm::vec2);

        glVertexAttribPointer(textureIndexAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(textureIndexAttributeIndex);
        attributeOffset += sizeof(float);

        glVertexAttribPointer(specularIndexAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(specularIndexAttributeIndex);
        attributeOffset += sizeof(float);

        glVertexAttribPointer(shininessAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(shininessAttributeIndex);
        attributeOffset += sizeof(float);

        glVertexAttribPointer(translationAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(translationAttributeIndex);
        attributeOffset += sizeof(glm::vec3);

        glVertexAttribPointer(orientationAttributeIndex, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(orientationAttributeIndex);
        // attributeOffset += sizeof(glm::quat);

        // make the buffer the active element array buffer, create it if necessary
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId_);

        // set the size of the active element array buffer
        // (target, size, data, usage)
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uint) * s_bufferCapacity_, NULL, GL_DYNAMIC_DRAW);

        // unbind the array buffer and the vertex array
        ///glBindBuffer(GL_ARRAY_BUFFER, 0);
        ///glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        ///glBindVertexArray(0);

        shaderPtr_->Use();

        for (int i = 0; i < s_numberOfTextureSlots_; ++i)
        {
            texturePtrs_[i] = nullptr;

            std::string uniformName;
            uniformName.append("textures[");
            uniformName.append(std::to_string(i));
            uniformName.append("]");
            shaderPtr_->SetInt(uniformName.c_str(), i);
        }
    }

    OpenGLWindow::~OpenGLWindow()
    {
        glfwDestroyWindow(glfwWindowPtr_);

        delete shaderPtr_;

        glDeleteBuffers(1, &vertexBufferId_);
        glDeleteVertexArrays(1, &vertexArrayId_);
        glDeleteVertexArrays(1, &indexBufferId_);

        for (int i = 0; i < s_numberOfTextureSlots_; ++i)
        {
            delete texturePtrs_[i];
        }

        if (s_glfwWindowCount_ == 0)
        {
            glfwTerminate();
        }
    }

    void OpenGLWindow::Render(const RenderData* renderData) const
    {
        glfwMakeContextCurrent(glfwWindowPtr_);

        const glm::mat4& viewMatrix = renderData->viewMatrix;
        const glm::vec3& viewPosition = renderData->viewPosition;
        const glm::mat4& projectionMatrix_ = renderData->projectionMatrix;
        const DirectionalLight& directionalLight = renderData->directionalLight;
        const std::vector<PointLight>& pointLights = renderData->pointLights;
        const std::vector<SpotLight>& spotLights = renderData->spotLights;
        const std::vector<VertexData>& vertexBuffer = renderData->vertexBuffer;
        const std::vector<glm::uint>& indexBuffer = renderData->indexBuffer;

        shaderPtr_->SetMat4("view", viewMatrix);
        shaderPtr_->SetMat4("projection", projectionMatrix_);

        shaderPtr_->SetVec3("viewPosition", viewPosition);

        // DirectionalLight
        shaderPtr_->SetVec3("directionalLight.direction", directionalLight.direction);
        shaderPtr_->SetVec3("directionalLight.ambient", directionalLight.ambient);
        shaderPtr_->SetVec3("directionalLight.diffuse", directionalLight.diffuse);
        shaderPtr_->SetVec3("directionalLight.specular", directionalLight.specular);

        // PointLights
        PointLight emptyPointLight;
        for (int i = 0; i < s_numberOfPointLights_; ++i)
        {
            std::string uniformName;
            uniformName.append("pointLights[");
            uniformName.append(std::to_string(i));
            uniformName.append("]");

            if (i < pointLights.size())
            {
                shaderPtr_->SetVec3((uniformName + ".position").c_str(), pointLights[i].position);
                shaderPtr_->SetFloat((uniformName + ".constant").c_str(), pointLights[i].constant);
                shaderPtr_->SetFloat((uniformName + ".linear").c_str(), pointLights[i].linear);
                shaderPtr_->SetFloat((uniformName + ".quadratic").c_str(), pointLights[i].quadratic);
                shaderPtr_->SetVec3((uniformName + ".ambient").c_str(), pointLights[i].ambient);
                shaderPtr_->SetVec3((uniformName + ".diffuse").c_str(), pointLights[i].diffuse);
                shaderPtr_->SetVec3((uniformName + ".specular").c_str(), pointLights[i].specular);
            }
            else
            {
                shaderPtr_->SetVec3((uniformName + ".position").c_str(), emptyPointLight.position);
                shaderPtr_->SetFloat((uniformName + ".constant").c_str(), emptyPointLight.constant);
                shaderPtr_->SetFloat((uniformName + ".linear").c_str(), emptyPointLight.linear);
                shaderPtr_->SetFloat((uniformName + ".quadratic").c_str(), emptyPointLight.quadratic);
                shaderPtr_->SetVec3((uniformName + ".ambient").c_str(), emptyPointLight.ambient);
                shaderPtr_->SetVec3((uniformName + ".diffuse").c_str(), emptyPointLight.diffuse);
                shaderPtr_->SetVec3((uniformName + ".specular").c_str(), emptyPointLight.specular);
            }
        }

        // SpotLights
        SpotLight emptySpotLight;
        for (int i = 0; i < s_numberOfSpotLights_; ++i)
        {
            std::string uniformName;
            uniformName.append("spotLights[");
            uniformName.append(std::to_string(i));
            uniformName.append("]");

            if (i < spotLights.size())
            {
                shaderPtr_->SetVec3((uniformName + ".position").c_str(), spotLights[i].position);
                shaderPtr_->SetVec3((uniformName + ".direction").c_str(), spotLights[i].direction);
                shaderPtr_->SetFloat((uniformName + ".cutOff").c_str(), spotLights[i].cutOff);
                shaderPtr_->SetFloat((uniformName + ".outerCutOff").c_str(), spotLights[i].outerCutOff);
                shaderPtr_->SetFloat((uniformName + ".constant").c_str(), spotLights[i].constant);
                shaderPtr_->SetFloat((uniformName + ".linear").c_str(), spotLights[i].linear);
                shaderPtr_->SetFloat((uniformName + ".quadratic").c_str(), spotLights[i].quadratic);
                shaderPtr_->SetVec3((uniformName + ".ambient").c_str(), spotLights[i].ambient);
                shaderPtr_->SetVec3((uniformName + ".diffuse").c_str(), spotLights[i].diffuse);
                shaderPtr_->SetVec3((uniformName + ".specular").c_str(), spotLights[i].specular);
            }
            else
            {
                shaderPtr_->SetVec3((uniformName + ".position").c_str(), emptySpotLight.position);
                shaderPtr_->SetVec3((uniformName + ".direction").c_str(), emptySpotLight.direction);
                shaderPtr_->SetFloat((uniformName + ".cutOff").c_str(), emptySpotLight.cutOff);
                shaderPtr_->SetFloat((uniformName + ".outerCutOff").c_str(), emptySpotLight.outerCutOff);
                shaderPtr_->SetFloat((uniformName + ".constant").c_str(), emptySpotLight.constant);
                shaderPtr_->SetFloat((uniformName + ".linear").c_str(), emptySpotLight.linear);
                shaderPtr_->SetFloat((uniformName + ".quadratic").c_str(), emptySpotLight.quadratic);
                shaderPtr_->SetVec3((uniformName + ".ambient").c_str(), emptySpotLight.ambient);
                shaderPtr_->SetVec3((uniformName + ".diffuse").c_str(), emptySpotLight.diffuse);
                shaderPtr_->SetVec3((uniformName + ".specular").c_str(), emptySpotLight.specular);
            }
        }

        // Bind buffers to make them active
        ///glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId_);
        ///glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId_);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        unsigned int numberOfVerticiesLeftToDraw = (unsigned int)vertexBuffer.size();
        unsigned int numberOfVerticiesDrawn = 0;
        while (numberOfVerticiesDrawn < vertexBuffer.size())
        {
            unsigned int numberOfVerticiesThatWillBeDrawn = s_bufferCapacity_;
            if (numberOfVerticiesLeftToDraw < numberOfVerticiesThatWillBeDrawn)
            {
                numberOfVerticiesThatWillBeDrawn = numberOfVerticiesLeftToDraw;
            }

            // upload the vertex data and index data into their respective buffers
            // (target, offset, size, data)
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VertexData) * numberOfVerticiesThatWillBeDrawn, &vertexBuffer[numberOfVerticiesDrawn]);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(glm::uint) * numberOfVerticiesThatWillBeDrawn, &indexBuffer[numberOfVerticiesDrawn]);

            /// glBindVertexArray(vertexArrayId_);
            glDrawElements(GL_TRIANGLES, numberOfVerticiesThatWillBeDrawn, GL_UNSIGNED_INT, 0);

            numberOfVerticiesDrawn += numberOfVerticiesThatWillBeDrawn;
        }

        glfwSwapBuffers(glfwWindowPtr_);
    }

    void OpenGLWindow::AddTexture(const TextureData* textureData, unsigned int index)
    {
        glfwMakeContextCurrent(glfwWindowPtr_);

        if (index >= 0 && index < s_numberOfTextureSlots_)
        {
            OpenGLTexture*& texturePtr = texturePtrs_[index];
            if (texturePtr != nullptr)
            {
                delete texturePtr;
            }
            texturePtr = new OpenGLTexture(index, textureData->data, textureData->width, textureData->height, textureData->numberOfComponents);
        }
    }

    void OpenGLWindow::PollEvents()
    {
        glfwMakeContextCurrent(glfwWindowPtr_);

        glfwPollEvents();
    }

    inline void OpenGLWindow::SetAspectRatio(int numerator, int denominator)
    {
        glfwSetWindowAspectRatio(glfwWindowPtr_, numerator, denominator);
    }

    inline void OpenGLWindow::SetSize(int width, int height)
    {
        glfwSetWindowSize(glfwWindowPtr_, width, height);
    }

    inline void OpenGLWindow::SetTime(const double time)
    {
        glfwSetTime(time);
    }

    inline double OpenGLWindow::GetTime() const
    {
        return glfwGetTime();
    }

    void OpenGLWindow::SetVSync(bool enabled)
    {
        glfwMakeContextCurrent(glfwWindowPtr_);

        if (enabled)
        {
            glfwSwapInterval(1);
        }
        else
        {
            glfwSwapInterval(0);
        }

        windowData_.vSyncEnabled = enabled;
    }

    bool OpenGLWindow::GetKeyPressed(KeyCode key) const
    {
        int result = glfwGetKey(glfwWindowPtr_, (int)key);
        return result == GLFW_PRESS;
    }

    bool OpenGLWindow::GetMouseButtonPressed(MouseButton mouseButton) const
    {
        int result = glfwGetMouseButton(glfwWindowPtr_, (int)mouseButton);
        return result == GLFW_PRESS;
    }

    void OpenGLWindow::GetCursorPosition(float& xPosition, float& yPosition) const
    {
        double xPositionDouble, yPositionDouble;
        glfwGetCursorPos(glfwWindowPtr_, &xPositionDouble, &yPositionDouble);
        xPosition = (float)xPositionDouble;
        yPosition = (float)yPositionDouble;
    }

    bool OpenGLWindow::GetJoystickPresent(unsigned int index) const
    {
        int result = glfwJoystickPresent(index);
        return result == GL_TRUE;
    }

    void OpenGLWindow::GetJoystickAxis(unsigned int index, float*& values, unsigned int& count) const
    {
        int countInt;
        const float* axes = glfwGetJoystickAxes(index, &countInt);
        count = (unsigned int)countInt;
        for (unsigned int i = 0; i < countInt; ++i)
        {
            values[i] = axes[i];
        }
    }

    void OpenGLWindow::GetJoystickButtonsPressed(unsigned int index, bool*& values, unsigned int& count) const
    {
        int countInt;
        const unsigned char* buttons = glfwGetJoystickButtons(index, &countInt);
        count = (unsigned int)countInt;
        for (unsigned int i = 0; i < countInt; ++i)
        {
            values[i] = buttons[i] == GLFW_PRESS;
        }
    }

    // protected ---------------------------------------------------------------

    int OpenGLWindow::s_glfwWindowCount_ = 0;
}