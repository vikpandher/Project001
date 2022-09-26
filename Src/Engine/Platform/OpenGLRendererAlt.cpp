#include "OpenGLRendererAlt.h"

// https://www.khronos.org/registry/OpenGL/specs/gl/

#include <algorithm>
#include <string>

#include "glad/glad.h"

#include "Engine/Logger.h"
#include "Engine/MeshVertex.h"

#include "Engine/Platform/OpenGLShader.h"
#include "Engine/Platform/OpenGLTexture.h"
#include "Engine/Platform/ShaderSource/BatchShaderSource.h"
#include "Engine/Platform/ShaderSource/GridShaderSource.h"
#include "Engine/Platform/ShaderSource/NormalShaderSource.h"
#include "Engine/Platform/ShaderSource/ScreenShaderSource.h"
#include "Engine/Platform/ShaderSource/WireFrameShaderSource.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    OpenGLRendererAlt::OpenGLRendererAlt(unsigned int width, unsigned int height)
        : redrawGrid_(true)
        , depthTesting_(true)
        , frameBufferWidth_(width)
        , frameBufferHeight_(height)
        , viewMatrix_(1.0f)
        , viewPosition_(0.0f, 0.0f, 0.0f)
        , projectionMatrix_(1.0f)
    {
        glGenVertexArrays(1, &vertexArrayId_);

        glGenBuffers(1, &vertexBufferId_);

        glGenBuffers(1, &indexBufferId_);

        glBindVertexArray(vertexArrayId_);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId_);

        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * s_vertexBufferCapacity_, NULL, GL_DYNAMIC_DRAW);

        GLuint positionAttributeIndex = 0;
        GLuint textureCoordinateAttributeIndex = 1;
        GLuint normalAttributeIndex = 2;
        GLuint colorAttributeIndex = 3;
        GLuint textureUnitAttributeIndex = 4;
        GLuint specularUnitAttributeIndex = 5;
        GLuint shininessAttributeIndex = 6;
        GLuint scaleAttributeIndex = 7;
        GLuint translationAttributeIndex = 8;
        GLuint orientationAttributeIndex = 9;
        GLuint litAttributeIndex = 10;

        unsigned long long attributeOffset = 0;

        glVertexAttribPointer(positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(positionAttributeIndex);
        attributeOffset += sizeof(glm::vec3);

        glVertexAttribPointer(textureCoordinateAttributeIndex, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(textureCoordinateAttributeIndex);
        attributeOffset += sizeof(glm::vec2);

        glVertexAttribPointer(normalAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(normalAttributeIndex);
        attributeOffset += sizeof(glm::vec3);

        glVertexAttribPointer(colorAttributeIndex, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(colorAttributeIndex);
        attributeOffset += sizeof(glm::vec4);

        glVertexAttribPointer(textureUnitAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(textureUnitAttributeIndex);
        attributeOffset += sizeof(float);

        glVertexAttribPointer(specularUnitAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(specularUnitAttributeIndex);
        attributeOffset += sizeof(float);

        glVertexAttribPointer(shininessAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(shininessAttributeIndex);
        attributeOffset += sizeof(float);

        glVertexAttribPointer(scaleAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(scaleAttributeIndex);
        attributeOffset += sizeof(glm::vec3);

        glVertexAttribPointer(translationAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(translationAttributeIndex);
        attributeOffset += sizeof(glm::vec3);

        glVertexAttribPointer(orientationAttributeIndex, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(orientationAttributeIndex);
        attributeOffset += sizeof(glm::quat);

        glVertexAttribPointer(litAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(litAttributeIndex);
        // attributeOffset += sizeof(float);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId_);

        if (s_indexBufferCapacity_ % 3 != 0)
        {
            _LOG_ERROR("Index Buffer Size Not Multiple Of 3");
        }

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * s_indexBufferCapacity_, NULL, GL_DYNAMIC_DRAW);

        // ---------------------------------------------------------------------

        struct GridVertexData
        {
            glm::vec3 position;
            glm::vec4 color;
        };
        std::vector<GridVertexData> gridVerticies;

        for (float i = s_gridHalfExtents_; i > 0; i -= s_minorGridIncement_)
        {
            if (i + 0.00001f > 0.0f && i - 0.00001f < 0.0f)
            {
                continue;
            }

            float alpha = 0.4f;

            float fmodRemainder = std::fmodf(i, s_majorGridIncement_);
            if ((fmodRemainder + 0.00001f > 0.0f && fmodRemainder - 0.00001f < 0.0f) || 
                (fmodRemainder + 0.00001f > s_majorGridIncement_ && fmodRemainder - 0.00001f < s_majorGridIncement_))
            {
                alpha = 1.0f;
            }

            // xy plane +x lines
            gridVerticies.push_back({
                glm::vec3(i, -s_gridHalfExtents_, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(i, -0.00001, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(i, 0.00001, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(i, s_gridHalfExtents_, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });

            // xy plane -x lines
            gridVerticies.push_back({
                glm::vec3(-i, -s_gridHalfExtents_, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(-i, -0.00001, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(-i, 0.00001, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(-i, s_gridHalfExtents_, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });

            // xy plane +y lines
            gridVerticies.push_back({
                glm::vec3(-s_gridHalfExtents_, i, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(-0.00001, i, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(0.00001, i, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(s_gridHalfExtents_, i, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });

            // xy plane -y lines
            gridVerticies.push_back({
                glm::vec3(-s_gridHalfExtents_, -i, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(-0.00001, -i, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(0.00001, -i, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(s_gridHalfExtents_, -i, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });

            // xz plane +x lines
            gridVerticies.push_back({
                glm::vec3(i, 0.0f, -s_gridHalfExtents_),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(i, 0.0f, -0.00001),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(i, 0.0f, 0.00001),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(i, 0.0f, s_gridHalfExtents_),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });

            // xz plane -x lines
            gridVerticies.push_back({
                glm::vec3(-i, 0.0f, -s_gridHalfExtents_),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(-i, 0.0f, -0.00001),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(-i, 0.0f, 0.00001),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(-i, 0.0f, s_gridHalfExtents_),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });

            // xz plane +z lines
            gridVerticies.push_back({
                glm::vec3(-s_gridHalfExtents_, 0.0f, i),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(-0.00001, 0.0f, i),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(0.00001, 0.0f, i),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(s_gridHalfExtents_, 0.0f, i),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });

            // xz plane -x lines
            gridVerticies.push_back({
                glm::vec3(-s_gridHalfExtents_, 0.0f, -i),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(-0.00001, 0.0f, -i),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(0.00001, 0.0f, -i),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(s_gridHalfExtents_, 0.0f, -i),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });

            // yz plane +y lines
            gridVerticies.push_back({
                glm::vec3(0.0f, i, -s_gridHalfExtents_),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(0.0f, i, -0.00001),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(0.0f, i, 0.00001),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(0.0f, i, s_gridHalfExtents_),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });

            // yz plane -y lines
            gridVerticies.push_back({
                glm::vec3(0.0f, -i, -s_gridHalfExtents_),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(0.0f, -i, -0.00001),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(0.0f, -i, 0.00001),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(0.0f, -i, s_gridHalfExtents_),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });

            // yz plane +z lines
            gridVerticies.push_back({
                glm::vec3(0.0f, -s_gridHalfExtents_, i),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(0.0f, -0.00001, i),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(0.0f, 0.00001, i),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(0.0f, s_gridHalfExtents_, i),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });

            // yz plane -z lines
            gridVerticies.push_back({
                glm::vec3(0.0f, -s_gridHalfExtents_, -i),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(0.0f, -0.00001, -i),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(0.0f, 0.00001, -i),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });
            gridVerticies.push_back({
                glm::vec3(0.0f, s_gridHalfExtents_, -i),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });
        }

        // x axis
        gridVerticies.push_back({
                glm::vec3(-s_gridHalfExtents_, 0.0f, 0.0f),
                glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
            });
        gridVerticies.push_back({
                glm::vec3(-0.00001, 0.0f, 0.0f),
                glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
            });
        gridVerticies.push_back({
                glm::vec3(0.00001, 0.0f, 0.0f),
                glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
            });
        gridVerticies.push_back({
                glm::vec3(s_gridHalfExtents_, 0.0f, 0.0f),
                glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
            });

        // y axis
        gridVerticies.push_back({
                glm::vec3(0.0f, -s_gridHalfExtents_, 0.0f),
                glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)
            });
        gridVerticies.push_back({
                glm::vec3(0.0f, -0.00001, 0.0f),
                glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)
            });
        gridVerticies.push_back({
                glm::vec3(0.0f, 0.00001, 0.0f),
                glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)
            });
        gridVerticies.push_back({
                glm::vec3(0.0f, s_gridHalfExtents_, 0.0f),
                glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)
            });

        // z axis
        gridVerticies.push_back({
                glm::vec3(0.0f, 0.0f, -s_gridHalfExtents_),
                glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)
            });
        gridVerticies.push_back({
                glm::vec3(0.0f, 0.0f, -0.00001),
                glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)
            });
        gridVerticies.push_back({
                glm::vec3(0.0f, 0.0f, 0.00001),
                glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)
            });
        gridVerticies.push_back({
            glm::vec3(0.0f, 0.0f, s_gridHalfExtents_),
            glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)
            });

        gridVertexCount_ = gridVerticies.size();

        glGenVertexArrays(1, &gridVertexArrayId_);
        glGenBuffers(1, &gridVertexBufferId_);
        glBindVertexArray(gridVertexArrayId_);
        glBindBuffer(GL_ARRAY_BUFFER, gridVertexBufferId_);
        glBufferData(GL_ARRAY_BUFFER, gridVertexCount_ * sizeof(GridVertexData), &gridVerticies.front(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GridVertexData), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GridVertexData), (void*)(sizeof(float) * 3));
        glEnableVertexAttribArray(1);

        // ---------------------------------------------------------------------

        float screenQuadVertices[] = {
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
        
            -1.0f,  1.0f,  0.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f
        };

        glGenVertexArrays(1, &screenVertexArrayId_);
        glGenBuffers(1, &screenVertexBufferId_);
        glBindVertexArray(screenVertexArrayId_);
        glBindBuffer(GL_ARRAY_BUFFER, screenVertexBufferId_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuadVertices), &screenQuadVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
        glEnableVertexAttribArray(1);

        primaryShaderPtr_ = new OpenGLShader(
            BatchShader::g_vertexShaderSource,
            BatchShader::g_fragmentShaderSource
        );
        primaryShaderPtr_->Use();

        for (int i = 0; i < s_numberOfTextureUnits_; ++i)
        {
            std::string uniformName;
            uniformName.append("u_Textures[");
            uniformName.append(std::to_string(i));
            uniformName.append("]");
            primaryShaderPtr_->SetInt(uniformName.c_str(), i);
        }

        gridShaderPtr_ = new OpenGLShader(
            GridShader::g_vertexShaderSource,
            GridShader::g_fragmentShaderSource
        );

        wireframeShaderPtr_ = new OpenGLShader(
            WireFrameShader::g_vertexShaderSource,
            WireFrameShader::g_geometryShaderSource,
            WireFrameShader::g_fragmentShaderSource
        );
        wireframeShaderPtr_->Use();

        wireframeShaderPtr_->SetVec4("u_Color", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

        normalShaderPtr_ = new OpenGLShader(
            NormalShader::g_vertexShaderSource,
            NormalShader::g_geometryShaderSource,
            NormalShader::g_fragmentShaderSource
        );
        normalShaderPtr_->Use();

        normalShaderPtr_->SetFloat("u_Magnitude", 0.16f);
        normalShaderPtr_->SetVec4("u_StartColor", glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
        normalShaderPtr_->SetVec4("u_EndColor", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

        screenShaderPtr_ = new OpenGLShader(
            ScreenShader::g_vertexShaderSource,
            ScreenShader::g_fragmentShaderSource
        );
        screenShaderPtr_->Use();

        screenShaderPtr_->SetInt("u_ScreenTexture", 0);

        CreateFramebuffer();

        GLint viewPortData[4];
        glGetIntegerv(GL_VIEWPORT, viewPortData);
        viewportX_ = viewPortData[0];
        viewportY_ = viewPortData[1];
        viewportWidth_ = viewPortData[2];
        viewportHeight_ = viewPortData[3];

        tectureUnitStalenessValues_.push_back(0);
        tectureUnitStalenessValues_.resize(s_numberOfTextureUnits_, 1);

        pointLights_.reserve(s_numberOfPointLights_);
        spotLights_.reserve(s_numberOfSpotLights_);
    }

    OpenGLRendererAlt::~OpenGLRendererAlt()
    {
        delete primaryShaderPtr_;
        delete wireframeShaderPtr_;
        delete normalShaderPtr_;
        delete screenShaderPtr_;

        DeleteAllTextures();

        glDeleteBuffers(1, &vertexBufferId_);
        glDeleteBuffers(1, &indexBufferId_);
        glDeleteVertexArrays(1, &vertexArrayId_);

        glDeleteRenderbuffers(1, &renderBufferId_);
        glDeleteTextures(1, &screenTextureColorBufferId_);
        glDeleteFramebuffers(1, &frameBufferId_);

        glDeleteBuffers(1, &gridVertexBufferId_);
        glDeleteVertexArrays(1, &gridVertexArrayId_);

        glDeleteBuffers(1, &screenVertexBufferId_);
        glDeleteVertexArrays(1, &screenVertexArrayId_);
    }

    void OpenGLRendererAlt::SetFramebufferSize(
        unsigned int width,
        unsigned int height)
    {
        frameBufferWidth_ = width;
        frameBufferHeight_ = height;

        glDeleteRenderbuffers(1, &renderBufferId_);
        glDeleteTextures(1, &screenTextureColorBufferId_);
        glDeleteFramebuffers(1, &frameBufferId_);

        CreateFramebuffer();
    }

    bool OpenGLRendererAlt::CreateTexture(
        unsigned int& textureId,
        unsigned int textureUnit,
        unsigned char* data,
        unsigned int width,
        unsigned int height,
        unsigned int bytesPerPixel)
    {
        if (textureUnit < s_numberOfTextureUnits_ && textureUnit > 0) // reserving 0 for the screenTexture
        {
            if (recycledTextureIds_.empty())
            {
                textureId = (unsigned int)texturePtrMap_.size();
            }
            else
            {
                textureId = recycledTextureIds_.front();
                recycledTextureIds_.pop_front();
            }

            texturePtrMap_[textureId] = new OpenGLTexture(textureUnit, data, width, height, bytesPerPixel);
            textureIdToUnitBiMap_.Add(textureId, textureUnit);

            return true;
        }
        else
        {
            return false;
        }
    }

    bool OpenGLRendererAlt::BindTexture(
        unsigned int textureId,
        unsigned int textureUnit)
    {
        if (textureUnit < s_numberOfTextureUnits_ && textureUnit > 0) // reserving 0 for the screenTexture
        {
            if (texturePtrMap_.find(textureId) != texturePtrMap_.end())
            {
                texturePtrMap_[textureId]->Bind(textureUnit);
                textureIdToUnitBiMap_.Add(textureId, textureUnit);
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    bool OpenGLRendererAlt::DeleteTexture(unsigned int textureId)
    {
        if (texturePtrMap_.find(textureId) != texturePtrMap_.end())
        {
            delete texturePtrMap_[textureId];
            textureIdToUnitBiMap_.Remove_Using_X(textureId);
            recycledTextureIds_.push_back(textureId);
            return true;
        }

        return false;
    }

    void OpenGLRendererAlt::DeleteAllTextures()
    {
        for (std::map<unsigned int, OpenGLTexture*>::iterator iter = texturePtrMap_.begin();
            iter != texturePtrMap_.end(); ++iter)
        {
            delete iter->second;
        }
        texturePtrMap_.clear();
        textureIdToUnitBiMap_.Clear();
        recycledTextureIds_.clear();
    }

    bool OpenGLRendererAlt::AddMesh(
        const MeshVertex* meshVerticies,
        unsigned int meshVertexCount,
        const unsigned int* meshIndicies,
        unsigned int meshIndexCount,
        unsigned int textureId,
        unsigned int specularId,
        const glm::vec3& position,
        const glm::quat& orientation,
        const glm::vec3& scale,
        const glm::vec4& color,
        float shininess,
        bool translucent,
        bool lit)
    {
        // if (translucent)
        // {
        //     _LOG_ERROR("OpenGLRendererAlt doesn't handle translucent meshes");
        // }

        if ((vertexBuffer_.size() + meshVertexCount) >= s_vertexBufferCapacity_ ||
            (indexBuffer_.size() + meshIndexCount) >= s_indexBufferCapacity_)
        {
            if (meshVertexCount >= s_vertexBufferCapacity_ ||
                meshIndexCount >= s_indexBufferCapacity_)
            {
                return false;
            }

            Render();
        }

        bool getTextureFailed = false;

        float textureUnit = -1.0f;
        if (textureId != (unsigned int)-1)
        {
            int getTextureUnitResult = GetTextureUnit(textureId, textureUnit);
            if (getTextureUnitResult == 1)
            {
                getTextureFailed = true;
            }
            else if (getTextureUnitResult == 2)
            {
                return false;
            }
        }

        float specularUnit = -1.0f;
        if (specularId != (unsigned int)-1)
        {
            int getTextureUnitResult = GetTextureUnit(specularId, specularUnit);
            if (getTextureUnitResult == 1)
            {
                getTextureFailed = true;
            }
            else if (getTextureUnitResult == 2)
            {
                return false;
            }
        }

        if (getTextureFailed)
        {
            Render();
            GetTextureUnit(textureId, textureUnit);
            GetTextureUnit(specularId, specularUnit);
        }

        unsigned int vertexBufferOffset = (unsigned int)vertexBuffer_.size();

        for (size_t j = 0; j < meshVertexCount; ++j)
        {
            const MeshVertex& currentMeshVertex = meshVerticies[j];

            VertexData newVertex;
            newVertex.position = currentMeshVertex.position;
            newVertex.textureCoordinate = currentMeshVertex.textureCoordinate;
            newVertex.normal = currentMeshVertex.normal;
            newVertex.color = color;
            newVertex.textureUnit = textureUnit;
            newVertex.specularUnit = specularUnit;
            newVertex.shininess = shininess;
            newVertex.scale = scale;
            newVertex.translation = position;
            newVertex.orientation.x = orientation.x;
            newVertex.orientation.y = orientation.y;
            newVertex.orientation.z = orientation.z;
            newVertex.orientation.w = orientation.w;
            newVertex.lit = lit;

            vertexBuffer_.push_back(newVertex);
        }

        for (unsigned int j = 0; j < meshIndexCount; ++j)
        {
            indexBuffer_.push_back(vertexBufferOffset + meshIndicies[j]);
        }

        return true;
    }

    void OpenGLRendererAlt::PrepareCapabilities()
    {
        glDepthMask(GL_TRUE);

        glEnable(GL_CULL_FACE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, screenTextureColorBufferId_);
    }

    void OpenGLRendererAlt::Clear()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId_);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        redrawGrid_ = true;
    }

    void OpenGLRendererAlt::Render()
    {
        // Render to texture frameBuffer
        // ---------------------------------------------------------------------

        glViewport(0, 0, frameBufferWidth_, frameBufferHeight_);

        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId_);

        if (depthTesting_)
        {
            // enable using the z buffer
            glEnable(GL_DEPTH_TEST);
        }

        if (s_drawGrid)
        {
            redrawGrid_ = false;

            gridShaderPtr_->Use();
            gridShaderPtr_->SetMat4("u_View", viewMatrix_);
            gridShaderPtr_->SetMat4("u_Projection", projectionMatrix_);
            gridShaderPtr_->SetVec3("u_ViewPosition", viewPosition_);

            glBindVertexArray(gridVertexArrayId_);
            glBindBuffer(GL_ARRAY_BUFFER, gridVertexBufferId_);

            glDrawArrays(GL_LINES, 0, gridVertexCount_);
        }

        primaryShaderPtr_->Use();

        // Camera
        // ---------------------------------------------------------------------

        primaryShaderPtr_->SetMat4("u_View", viewMatrix_);
        primaryShaderPtr_->SetMat4("u_Projection", projectionMatrix_);
        primaryShaderPtr_->SetVec3("u_ViewPosition", viewPosition_);

        // Directional Light
        // ---------------------------------------------------------------------

        primaryShaderPtr_->SetVec3("u_DirectionalLight.direction", directionalLight_.direction);
        primaryShaderPtr_->SetVec3("u_DirectionalLight.ambient", directionalLight_.ambient);
        primaryShaderPtr_->SetVec3("u_DirectionalLight.diffuse", directionalLight_.diffuse);
        primaryShaderPtr_->SetVec3("u_DirectionalLight.specular", directionalLight_.specular);

        // Point Lights
        // ---------------------------------------------------------------------

        std::string stringPrefix = "u_PointLights[";
        unsigned int i = 0;
        while (i < pointLights_.size())
        {
            std::string stringI = std::to_string(i);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI +  "].position").c_str(), pointLights_[i].position);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].constant").c_str(), pointLights_[i].constant);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].linear").c_str(), pointLights_[i].linear);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].quadratic").c_str(), pointLights_[i].quadratic);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].ambient").c_str(), pointLights_[i].ambient);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].diffuse").c_str(), pointLights_[i].diffuse);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].specular").c_str(), pointLights_[i].specular);
            ++i;
        }
        PointLight emptyPointLight;
        while (i < s_numberOfPointLights_)
        {
            std::string stringI = std::to_string(i);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].position").c_str(), emptyPointLight.position);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].constant").c_str(), emptyPointLight.constant);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].linear").c_str(), emptyPointLight.linear);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].quadratic").c_str(), emptyPointLight.quadratic);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].ambient").c_str(), emptyPointLight.ambient);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].diffuse").c_str(), emptyPointLight.diffuse);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].specular").c_str(), emptyPointLight.specular);
            ++i;
        }

        // Spot Lights
        // ---------------------------------------------------------------------

        stringPrefix = "u_SpotLights[";
        i = 0;
        while (i < spotLights_.size())
        {
            std::string stringI = std::to_string(i);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].position").c_str(), spotLights_[i].position);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].direction").c_str(), spotLights_[i].direction);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].cutoff").c_str(), spotLights_[i].cutoff);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].outerCutoff").c_str(), spotLights_[i].outerCutoff);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].constant").c_str(), spotLights_[i].constant);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].linear").c_str(), spotLights_[i].linear);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].quadratic").c_str(), spotLights_[i].quadratic);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].ambient").c_str(), spotLights_[i].ambient);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].diffuse").c_str(), spotLights_[i].diffuse);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].specular").c_str(), spotLights_[i].specular);
            ++i;
        }
        SpotLight emptySpotLight;
        while (i < s_numberOfSpotLights_)
        {
            std::string stringI = std::to_string(i);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].position").c_str(), emptySpotLight.position);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].direction").c_str(), emptySpotLight.direction);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].cutoff").c_str(), emptySpotLight.cutoff);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].outerCutoff").c_str(), emptySpotLight.outerCutoff);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].constant").c_str(), emptySpotLight.constant);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].linear").c_str(), emptySpotLight.linear);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].quadratic").c_str(), emptySpotLight.quadratic);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].ambient").c_str(), emptySpotLight.ambient);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].diffuse").c_str(), emptySpotLight.diffuse);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].specular").c_str(), emptySpotLight.specular);
            ++i;
        }

        glBindVertexArray(vertexArrayId_);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId_);

        if (!vertexBuffer_.empty() && !indexBuffer_.empty())
        {
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VertexData) * vertexBuffer_.size(), &vertexBuffer_[0]);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned int) * indexBuffer_.size(), &indexBuffer_[0]);
        }

        if (!s_cullBackface)
        {
            glCullFace(GL_FRONT);
            glDrawElements(GL_TRIANGLES, (GLsizei)indexBuffer_.size(), GL_UNSIGNED_INT, 0);
        }
        glCullFace(GL_BACK);
        glDrawElements(GL_TRIANGLES, (GLsizei)indexBuffer_.size(), GL_UNSIGNED_INT, 0);


        if (s_drawWireframe)
        {
            wireframeShaderPtr_->Use();

            wireframeShaderPtr_->SetMat4("u_View", viewMatrix_);
            wireframeShaderPtr_->SetMat4("u_Projection", projectionMatrix_);
            wireframeShaderPtr_->SetVec3("u_ViewPosition", viewPosition_);

            glDrawElements(GL_TRIANGLES, (GLsizei)indexBuffer_.size(), GL_UNSIGNED_INT, 0);
        }

        if (s_drawNormals)
        {
            normalShaderPtr_->Use();

            normalShaderPtr_->SetMat4("u_View", viewMatrix_);
            normalShaderPtr_->SetMat4("u_Projection", projectionMatrix_);

            glDrawElements(GL_TRIANGLES, (GLsizei)indexBuffer_.size(), GL_UNSIGNED_INT, 0);
        }

        // Render to screen frameBuffer
        // ---------------------------------------------------------------------

        glViewport(viewportX_, viewportY_, viewportWidth_, viewportHeight_);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        screenShaderPtr_->Use();

        glBindVertexArray(screenVertexArrayId_);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Clear local buffers
        // ---------------------------------------------------------------------

        IncreaseTectureUnitStaleness();

        vertexBuffer_.clear();
        indexBuffer_.clear();
    }

    // protected ---------------------------------------------------------------

    void OpenGLRendererAlt::CreateFramebuffer()
    {
        glGenFramebuffers(1, &frameBufferId_);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId_);

        glGenTextures(1, &screenTextureColorBufferId_);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, screenTextureColorBufferId_);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameBufferWidth_, frameBufferHeight_, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTextureColorBufferId_, 0);

        glGenRenderbuffers(1, &renderBufferId_);
        glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId_);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, frameBufferWidth_, frameBufferHeight_);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferId_);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            // Log Error
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    int OpenGLRendererAlt::GetTextureUnit(unsigned int textureId, float& textureUnit)
    {
        if (textureIdToUnitBiMap_.Find_X(textureId))
        {
            unsigned int textureUnit_uint = textureIdToUnitBiMap_.Get_Using_X(textureId);
            tectureUnitStalenessValues_[textureUnit_uint] = 0;
            textureUnit = (float)textureUnit_uint;
        }
        else
        {
            unsigned int newTextureUnit = 1;
            if (GetStalestTextureUnit(newTextureUnit))
            {
                if (BindTexture(textureId, newTextureUnit))
                {
                    textureUnit = (float)newTextureUnit;
                    tectureUnitStalenessValues_[newTextureUnit] = 0;
                }
                else
                {
                    // texture doesn't exist
                    return 2;
                }
            }
            else
            {
                // no more room for this texture
                return 1;
            }
        }

        return 0;
    }

    bool OpenGLRendererAlt::GetStalestTextureUnit(unsigned int& textureUnit) const
    {
        unsigned int stalestValue = 0;

        for (unsigned int i = 1; i < s_numberOfTextureUnits_; ++i) // reserving 0 for the screenTexture
        {
            if (tectureUnitStalenessValues_[i] > stalestValue)
            {
                textureUnit = i;
                stalestValue = tectureUnitStalenessValues_[i];
            }
        }

        return stalestValue > 0;
    }

    void OpenGLRendererAlt::IncreaseTectureUnitStaleness()
    {
        for (size_t i = 1 ; i < s_numberOfTextureUnits_; ++i) // reserving 0 for the screenTexture
        {
            tectureUnitStalenessValues_[i]++;
        }
    }

    const bool OpenGLRendererAlt::s_cullBackface = false;
    const bool OpenGLRendererAlt::s_drawWireframe = true;
    const bool OpenGLRendererAlt::s_drawNormals = true;
    const bool OpenGLRendererAlt::s_drawGrid = true;

    const unsigned int OpenGLRendererAlt::s_indexBufferCapacity_ = 4194304; // 8192;
    const unsigned int OpenGLRendererAlt::s_vertexBufferCapacity_ = 4194304; // 6144;

    const unsigned int OpenGLRendererAlt::s_numberOfTextureUnits_ = 16;

    const unsigned int OpenGLRendererAlt::s_numberOfPointLights_ = 8;
    const unsigned int OpenGLRendererAlt::s_numberOfSpotLights_ = 4;

    const float OpenGLRendererAlt::s_minorGridIncement_ = 0.1f;
    const float OpenGLRendererAlt::s_majorGridIncement_ = 1.0f;
    const float OpenGLRendererAlt::s_gridHalfExtents_ = 10.0f;
}