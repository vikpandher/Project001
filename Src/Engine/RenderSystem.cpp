#include "RenderSystem.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/LightSource.h"
#include "Engine/Components/RenderedModel.h"
#include "Engine/Math/Overlap3D.h"
#include "Engine/ComponentStores.h"
#include "Engine/Logger.h"
#include "Engine/Renderer.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    void RenderSystem::Render(ComponentStores* componentStoresPtr, Renderer* rendererPtr)
    {
        unsigned int framebufferWidth;
        unsigned int framebufferHeight;
        rendererPtr->GetFramebufferSize(framebufferWidth, framebufferHeight);
        if (framebufferWidth > 0 && framebufferHeight > 0)
        {
            rendererPtr->BeginRendering();
            rendererPtr->Clear();

            // Get a Camera
            // -----------------------------------------------------------------

            Camera* cameraPtrs = nullptr;
            size_t cameraCount = 0;
            componentStoresPtr->GetAllComponents<Camera>(cameraPtrs, cameraCount);

            s_cameraPtrs_.clear();

            for (size_t i = 0; i < cameraCount; ++i)
            {
                Camera& currentCamera = cameraPtrs[i];

                if (currentCamera.IsTurnedOn())
                {
                    s_cameraPtrs_.push_back(&currentCamera);
                }
            }

            std::sort(s_cameraPtrs_.begin(), s_cameraPtrs_.end(),
                [](Camera* a, Camera* b)->bool
                {
                    const int& aPriorityValue = a->GetPriorityValue();
                    const int& bPriorityValue = b->GetPriorityValue();

                    return aPriorityValue < bPriorityValue;
                });

            for (size_t i = 0; i < s_cameraPtrs_.size(); ++i)
            {
                Camera& currentCamera = *s_cameraPtrs_[i];
                if (currentCamera.IsTurnedOn())
                {
                    glm::mat4 cameraViewMatrix = currentCamera.GetViewMatrix();
                    glm::vec3 cameraPosition = currentCamera.GetPosition();
                    glm::mat4 cameraProjectionMatrix = currentCamera.GetProjectionMatrix();

                    FrustumPlanes cameraFrustumPlanes;
                    currentCamera.GetProjectionFrustumPlanes(cameraFrustumPlanes);

                    rendererPtr->ClearDirectionalLight();
                    rendererPtr->ClearPointLights();
                    rendererPtr->ClearSpotLights();
                    rendererPtr->ClearDepthOnly();

                    rendererPtr->SetViewMatrix(cameraViewMatrix);
                    rendererPtr->SetViewPosition(cameraPosition);
                    rendererPtr->SetProjectionMatrix(cameraProjectionMatrix);

                    float cameraViewportX;
                    float cameraViewportY;
                    float cameraViewportWidth;
                    float cameraViewportHeight;
                    currentCamera.GetCameraViewport(
                        cameraViewportX,
                        cameraViewportY,
                        cameraViewportWidth,
                        cameraViewportHeight
                    );
                    rendererPtr->SetCameraViewport(
                        cameraViewportX,
                        cameraViewportY,
                        cameraViewportWidth,
                        cameraViewportHeight
                    );

                    // Apply Lights
                    // -------------------------------------------------------------

                    LightSource* lightSourceArray = nullptr;
                    size_t lightSourceCount = 0;
                    componentStoresPtr->GetAllComponents<LightSource>(lightSourceArray, lightSourceCount);

                    for (unsigned int i = 0; i < lightSourceCount; ++i)
                    {
                        LightSource& currentLightSource = lightSourceArray[i];
                        if (currentLightSource.IsTurnedOn() &&
                            (currentCamera.GetCameraMask() & currentLightSource.GetCameraMask()))
                        {
                            if (currentLightSource.IsLightTypeDirectional())
                            {
                                rendererPtr->SetDirectionalLight(
                                    currentLightSource.GetDirection(),
                                    currentLightSource.GetAmbientColor(),
                                    currentLightSource.GetDiffuseColor(),
                                    currentLightSource.GetSpecularColor()
                                );
                            }
                            else if (currentLightSource.IsLightTypePoint())
                            {
                                rendererPtr->AddPointLight(
                                    currentLightSource.GetPosition(),
                                    currentLightSource.GetAttenuationConstant(),
                                    currentLightSource.GetLinearAttenuation(),
                                    currentLightSource.GetQuadraticAttenuation(),
                                    currentLightSource.GetAmbientColor(),
                                    currentLightSource.GetDiffuseColor(),
                                    currentLightSource.GetSpecularColor()
                                );
                            }
                            else if (currentLightSource.IsLightTypeSpot())
                            {
                                rendererPtr->AddSpotLight(
                                    currentLightSource.GetPosition(),
                                    currentLightSource.GetDirection(),
                                    currentLightSource.GetCutoff(),
                                    currentLightSource.GetOuterCutoff(),
                                    currentLightSource.GetAttenuationConstant(),
                                    currentLightSource.GetLinearAttenuation(),
                                    currentLightSource.GetQuadraticAttenuation(),
                                    currentLightSource.GetAmbientColor(),
                                    currentLightSource.GetDiffuseColor(),
                                    currentLightSource.GetSpecularColor()
                                );
                            }
                        }
                    }

                    // Sort Models
                    // -------------------------------------------------------------

                    s_batchedRenderedModelPtrs_.clear();
                    s_instancedRenderedModelPtrs_.clear();
                    s_translucentInstancedRenderedModelPtrs_.clear();

                    RenderedModel* renderedModelArrayPtr = nullptr;
                    size_t renderedModelCount = 0;
                    componentStoresPtr->GetAllComponents<RenderedModel>(renderedModelArrayPtr, renderedModelCount);

                    for (size_t i = 0; i < renderedModelCount; ++i)
                    {
                        RenderedModel& currentRenderedModel = renderedModelArrayPtr[i];

                        if (!currentRenderedModel.IsVisible() ||
                            !(currentCamera.GetCameraMask() & currentRenderedModel.GetCameraMask()))
                        {
                            continue;
                        }

                        // Frustum Culling

                        const glm::vec3& renderedModelScale = currentRenderedModel.GetScale();

                        float largestScalingFactor = renderedModelScale.x;

                        if (largestScalingFactor < renderedModelScale.y)
                        {
                            largestScalingFactor = renderedModelScale.y;
                        }

                        if (largestScalingFactor < renderedModelScale.z)
                        {
                            largestScalingFactor = renderedModelScale.z;
                        }

                        float maxRadius = 0;
                        if (currentRenderedModel.GetRenderedModelType() == RenderedModel::RenderedModelType::RENDERED_MODEL_TYPE_LOADED_CPU_SIDE)
                        {
                            maxRadius = currentRenderedModel.GetMeshDataPtr()->maxRadius;
                        }
                        else if (currentRenderedModel.GetRenderedModelType() == RenderedModel::RenderedModelType::RENDERED_MODEL_TYPE_LOADED_GPU_SIDE)
                        {
                            maxRadius = currentRenderedModel.GetMaxRadius();
                        }

                        // This is an approximation. The true larest radius will be less then or equal to this value.
                        float scaledMaxRadius = maxRadius * largestScalingFactor;

                        bool inFrustum = Check3D_Sphere_Frustum_Overlap(
                            currentRenderedModel.GetPosition(),
                            scaledMaxRadius,
                            cameraFrustumPlanes.leftPlaneNormal_,
                            cameraFrustumPlanes.leftPlaneDistance_,
                            cameraFrustumPlanes.rightPlaneNormal_,
                            cameraFrustumPlanes.rightPlaneDistance_,
                            cameraFrustumPlanes.bottomPlaneNormal_,
                            cameraFrustumPlanes.bottomPlaneDistance_,
                            cameraFrustumPlanes.topPlaneNormal_,
                            cameraFrustumPlanes.topPlaneDistance_,
                            cameraFrustumPlanes.nearPlaneNormal_,
                            cameraFrustumPlanes.nearPlaneDistance_,
                            cameraFrustumPlanes.farPlaneNormal_,
                            cameraFrustumPlanes.farPlaneDistance_
                        );

                        if (!inFrustum)
                        {
                            continue;
                        }

                        if (currentRenderedModel.GetRenderedModelType() == RenderedModel::RenderedModelType::RENDERED_MODEL_TYPE_LOADED_CPU_SIDE)
                        {
                            s_batchedRenderedModelPtrs_.emplace_back(&currentRenderedModel);
                        }
                        else if (currentRenderedModel.GetRenderedModelType() == RenderedModel::RenderedModelType::RENDERED_MODEL_TYPE_LOADED_GPU_SIDE)
                        {
                            if (currentRenderedModel.GetTranslucent())
                            {
                                s_translucentInstancedRenderedModelPtrs_.emplace_back(&currentRenderedModel);
                            }
                            else
                            {
                                s_instancedRenderedModelPtrs_.emplace_back(&currentRenderedModel);
                            }
                        }
                    }

                    // Sorting Instanced Models
                    std::sort(s_instancedRenderedModelPtrs_.begin(), s_instancedRenderedModelPtrs_.end(),
                        [cameraPosition](RenderedModel* a, RenderedModel* b)->bool
                        {
                            unsigned int aMeshId = a->GetMeshId();
                            unsigned int bMeshId = b->GetMeshId();

                            if (aMeshId == bMeshId)
                            {
                                // same ids so the closer one is drawn first
                                glm::vec3 distanceA = cameraPosition - a->GetPosition();
                                glm::vec3 distanceB = cameraPosition - b->GetPosition();
                                float disatanceSquaredA = glm::dot(distanceA, distanceA);
                                float disatanceSquaredB = glm::dot(distanceB, distanceB);
                                return disatanceSquaredA < disatanceSquaredB;
                            }
                            else
                            {
                                // smaller id is drawn first (this groups the ids)
                                return aMeshId < bMeshId;
                            }
                        });

                    // Sorting Batched Models (Translucent and Non-Translucent)
                    std::sort(s_batchedRenderedModelPtrs_.begin(), s_batchedRenderedModelPtrs_.end(),
                        [cameraPosition](RenderedModel* a, RenderedModel* b)->bool
                        {
                            bool aTranslucent = a->GetTranslucent();
                            bool bTranslucent = b->GetTranslucent();
                            if (aTranslucent)
                            {
                                if (bTranslucent)
                                {
                                    // both are translucent so the farther is drawn first
                                    glm::vec3 distanceA = cameraPosition - a->GetPosition();
                                    glm::vec3 distanceB = cameraPosition - b->GetPosition();
                                    float disatanceSquaredA = glm::dot(distanceA, distanceA);
                                    float disatanceSquaredB = glm::dot(distanceB, distanceB);
                                    return disatanceSquaredA > disatanceSquaredB;
                                }
                                else
                                {
                                    // the one that is translucent is drawn last
                                    return false;
                                }
                            }
                            if (bTranslucent)
                            {
                                // the one that is translucent is drawn last
                                return true;
                            }
                            else
                            {
                                // both are not translucent so the closer is drawn first
                                glm::vec3 distanceA = cameraPosition - a->GetPosition();
                                glm::vec3 distanceB = cameraPosition - b->GetPosition();
                                float disatanceSquaredA = glm::dot(distanceA, distanceA);
                                float disatanceSquaredB = glm::dot(distanceB, distanceB);
                                return disatanceSquaredA < disatanceSquaredB;
                            }
                        });

                    // Sorting Translucent Instanced Models
                    std::sort(s_translucentInstancedRenderedModelPtrs_.begin(), s_translucentInstancedRenderedModelPtrs_.end(),
                        [cameraPosition](RenderedModel* a, RenderedModel* b)->bool
                        {
                            // the farther one is drawn first
                            glm::vec3 distanceA = cameraPosition - a->GetPosition();
                            glm::vec3 distanceB = cameraPosition - b->GetPosition();
                            float disatanceSquaredA = glm::dot(distanceA, distanceA);
                            float disatanceSquaredB = glm::dot(distanceB, distanceB);
                            return disatanceSquaredA > disatanceSquaredB;
                        });

                    // Rendering Instanced models
                    // -------------------------------------------------------------

                    if (!s_instancedRenderedModelPtrs_.empty())
                    {
                        unsigned int previousMeshId = s_instancedRenderedModelPtrs_[0]->GetMeshId();

                        for (unsigned int i = 0; i < s_instancedRenderedModelPtrs_.size(); ++i)
                        {
                            RenderedModel*& currentRenderedModelPtr = s_instancedRenderedModelPtrs_[i];

                            const unsigned int& meshId = s_instancedRenderedModelPtrs_[i]->GetMeshId();

                            if (meshId != previousMeshId)
                            {
                                _FAIL_CHECK(rendererPtr->RenderMesh(
                                    previousMeshId,
                                    s_meshInstanceDataArray_.data(),
                                    (unsigned int)s_meshInstanceDataArray_.size()));

                                previousMeshId = meshId;
                                s_meshInstanceDataArray_.clear();
                            }

                            MeshInstanceData meshInstanceData;
                            meshInstanceData.textureId = currentRenderedModelPtr->GetTextureId();
                            meshInstanceData.specularId = currentRenderedModelPtr->GetSpecularId();
                            meshInstanceData.position = currentRenderedModelPtr->GetPosition();
                            meshInstanceData.orientation = currentRenderedModelPtr->GetOrientation();
                            meshInstanceData.scale = currentRenderedModelPtr->GetScale();
                            meshInstanceData.color = currentRenderedModelPtr->GetColor();
                            meshInstanceData.shininess = currentRenderedModelPtr->GetShininess();
                            meshInstanceData.lit = currentRenderedModelPtr->GetLit();

                            s_meshInstanceDataArray_.push_back(meshInstanceData);
                        }

                        _FAIL_CHECK(rendererPtr->RenderMesh(
                            previousMeshId,
                            s_meshInstanceDataArray_.data(),
                            (unsigned int)s_meshInstanceDataArray_.size()));

                        s_meshInstanceDataArray_.clear();
                    }

                    // Rendering Batched Models (Translucent and Non-Translucent)
                    // -------------------------------------------------------------

                    for (unsigned int i = 0; i < s_batchedRenderedModelPtrs_.size(); ++i)
                    {
                        RenderedModel*& currentRenderedModelPtr = s_batchedRenderedModelPtrs_[i];

                        const MeshData* currentMeshDataPtr = currentRenderedModelPtr->GetMeshDataPtr();
                        if (currentMeshDataPtr != nullptr)
                        {
                            _FAIL_CHECK(rendererPtr->AddMeshToBatch(
                                currentMeshDataPtr->meshVertexArray.data(),
                                (unsigned int)currentMeshDataPtr->meshVertexArray.size(),
                                currentMeshDataPtr->meshIndexArray.data(),
                                (unsigned int)currentMeshDataPtr->meshIndexArray.size(),
                                currentRenderedModelPtr->GetTextureId(),
                                currentRenderedModelPtr->GetSpecularId(),
                                currentRenderedModelPtr->GetPosition(),
                                currentRenderedModelPtr->GetOrientation(),
                                currentRenderedModelPtr->GetScale(),
                                currentRenderedModelPtr->GetColor(),
                                currentRenderedModelPtr->GetShininess(),
                                currentRenderedModelPtr->GetLit()));
                        }
                    }

                    if (!s_batchedRenderedModelPtrs_.empty())
                    {
                        rendererPtr->RenderBatch();
                    }

                    // Rendering Translucent Instanced models
                    // -------------------------------------------------------------

                    if (!s_translucentInstancedRenderedModelPtrs_.empty())
                    {
                        unsigned int previousMeshId = s_translucentInstancedRenderedModelPtrs_[0]->GetMeshId();

                        for (unsigned int i = 0; i < s_translucentInstancedRenderedModelPtrs_.size(); ++i)
                        {
                            RenderedModel*& currentRenderedModelPtr = s_translucentInstancedRenderedModelPtrs_[i];

                            const unsigned int& meshId = s_translucentInstancedRenderedModelPtrs_[i]->GetMeshId();

                            if (meshId != previousMeshId)
                            {
                                _FAIL_CHECK(rendererPtr->RenderMesh(
                                    previousMeshId,
                                    s_meshInstanceDataArray_.data(),
                                    (unsigned int)s_meshInstanceDataArray_.size()));

                                previousMeshId = meshId;
                                s_meshInstanceDataArray_.clear();
                            }

                            MeshInstanceData meshInstanceData;
                            meshInstanceData.textureId = currentRenderedModelPtr->GetTextureId();
                            meshInstanceData.specularId = currentRenderedModelPtr->GetSpecularId();
                            meshInstanceData.position = currentRenderedModelPtr->GetPosition();
                            meshInstanceData.orientation = currentRenderedModelPtr->GetOrientation();
                            meshInstanceData.scale = currentRenderedModelPtr->GetScale();
                            meshInstanceData.color = currentRenderedModelPtr->GetColor();
                            meshInstanceData.shininess = currentRenderedModelPtr->GetShininess();
                            meshInstanceData.lit = currentRenderedModelPtr->GetLit();

                            s_meshInstanceDataArray_.push_back(meshInstanceData);
                        }

                        _FAIL_CHECK(rendererPtr->RenderMesh(
                            previousMeshId,
                            s_meshInstanceDataArray_.data(),
                            (unsigned int)s_meshInstanceDataArray_.size()));

                        s_meshInstanceDataArray_.clear();
                    }
                }
            }

            rendererPtr->FinishRendering();
            rendererPtr->SwapBuffers();
        }
    }

    // protected ---------------------------------------------------------------

    std::vector<Camera*> RenderSystem::s_cameraPtrs_;

    std::vector<RenderedModel*> RenderSystem::s_batchedRenderedModelPtrs_;
    std::vector<RenderedModel*> RenderSystem::s_instancedRenderedModelPtrs_;
    std::vector<RenderedModel*> RenderSystem::s_translucentInstancedRenderedModelPtrs_;

    std::vector<MeshInstanceData> RenderSystem::s_meshInstanceDataArray_;
}