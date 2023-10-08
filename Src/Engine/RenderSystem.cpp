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

    void RenderSystem::Render(
        ComponentStores* componentStoresPtr,
        Renderer* rendererPtr)
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

                    FrustumPlanes currentCameraFrustumPlanes;
                    currentCamera.GetProjectionFrustumPlanes(currentCameraFrustumPlanes);

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

                    // Sort Meshes
                    // -------------------------------------------------------------

                    s_batchedRenderedMeshPtrs_.clear();
                    s_instancedRenderedMeshPtrs_.clear();
                    s_translucentInstancedRenderedMeshPtrs_.clear();

                    RenderedMesh* renderedMeshArrayPtr = nullptr;
                    size_t renderedMeshCount = 0;
                    componentStoresPtr->GetAllComponents<RenderedMesh>(renderedMeshArrayPtr, renderedMeshCount);

                    for (size_t i = 0; i < renderedMeshCount; ++i)
                    {
                        GroupMeshPtr(&renderedMeshArrayPtr[i], &currentCamera, &currentCameraFrustumPlanes);
                    }

                    RenderedModel* renderedModelArrayPtr = nullptr;
                    size_t renderedModelCount = 0;
                    componentStoresPtr->GetAllComponents<RenderedModel>(renderedModelArrayPtr, renderedModelCount);

                    for (size_t i = 0; i < renderedModelCount; ++i)
                    {
                        RenderedModel& currentRenderedModel = renderedModelArrayPtr[i];

                        if (!currentRenderedModel.TransformedMeshesUpToDate())
                        {
                            currentRenderedModel.CalculateTransformedMeshes();
                        }

                        const std::vector<RenderedMesh>& transformedMeshes = currentRenderedModel.GetTransformedRenderedMeshes();
                        for (size_t j = 0; j < transformedMeshes.size(); ++j)
                        {
                            GroupMeshPtr(&transformedMeshes[j], &currentCamera, &currentCameraFrustumPlanes);
                        }
                    }

                    // Sorting Instanced Meshes
                    std::sort(s_instancedRenderedMeshPtrs_.begin(), s_instancedRenderedMeshPtrs_.end(),
                        [cameraPosition](const RenderedMesh* a, const RenderedMesh* b)->bool
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

                    // Sorting Batched Meshes (Translucent and Non-Translucent)
                    std::sort(s_batchedRenderedMeshPtrs_.begin(), s_batchedRenderedMeshPtrs_.end(),
                        [cameraPosition](const RenderedMesh* a, const RenderedMesh* b)->bool
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

                    // Sorting Translucent Instanced Meshes
                    std::sort(s_translucentInstancedRenderedMeshPtrs_.begin(), s_translucentInstancedRenderedMeshPtrs_.end(),
                        [cameraPosition](const RenderedMesh* a, const RenderedMesh* b)->bool
                        {
                            // the farther one is drawn first
                            glm::vec3 distanceA = cameraPosition - a->GetPosition();
                            glm::vec3 distanceB = cameraPosition - b->GetPosition();
                            float disatanceSquaredA = glm::dot(distanceA, distanceA);
                            float disatanceSquaredB = glm::dot(distanceB, distanceB);
                            return disatanceSquaredA > disatanceSquaredB;
                        });

                    // Rendering Instanced Meshes
                    // -------------------------------------------------------------

                    if (!s_instancedRenderedMeshPtrs_.empty())
                    {
                        unsigned int previousMeshId = s_instancedRenderedMeshPtrs_[0]->GetMeshId();

                        for (unsigned int i = 0; i < s_instancedRenderedMeshPtrs_.size(); ++i)
                        {
                            const RenderedMesh*& currentRenderedMeshPtr = s_instancedRenderedMeshPtrs_[i];

                            const unsigned int& meshId = s_instancedRenderedMeshPtrs_[i]->GetMeshId();

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
                            meshInstanceData.textureId = currentRenderedMeshPtr->GetTextureId();
                            meshInstanceData.specularId = currentRenderedMeshPtr->GetSpecularId();
                            meshInstanceData.position = currentRenderedMeshPtr->GetPosition();
                            meshInstanceData.orientation = currentRenderedMeshPtr->GetOrientation();
                            meshInstanceData.scale = currentRenderedMeshPtr->GetScale();
                            meshInstanceData.color = currentRenderedMeshPtr->GetColor();
                            meshInstanceData.shininess = currentRenderedMeshPtr->GetShininess();
                            meshInstanceData.lit = currentRenderedMeshPtr->GetLit();

                            s_meshInstanceDataArray_.push_back(meshInstanceData);
                        }

                        _FAIL_CHECK(rendererPtr->RenderMesh(
                            previousMeshId,
                            s_meshInstanceDataArray_.data(),
                            (unsigned int)s_meshInstanceDataArray_.size()));

                        s_meshInstanceDataArray_.clear();
                    }

                    // Rendering Batched Meshes (Translucent and Non-Translucent)
                    // -------------------------------------------------------------

                    for (unsigned int i = 0; i < s_batchedRenderedMeshPtrs_.size(); ++i)
                    {
                        const RenderedMesh*& currentRenderedMeshPtr = s_batchedRenderedMeshPtrs_[i];

                        const MeshData* currentMeshDataPtr = currentRenderedMeshPtr->GetMeshDataPtr();
                        if (currentMeshDataPtr != nullptr)
                        {
                            _FAIL_CHECK(rendererPtr->AddMeshToBatch(
                                currentMeshDataPtr->meshVertexArray.data(),
                                (unsigned int)currentMeshDataPtr->meshVertexArray.size(),
                                currentMeshDataPtr->meshIndexArray.data(),
                                (unsigned int)currentMeshDataPtr->meshIndexArray.size(),
                                currentRenderedMeshPtr->GetTextureId(),
                                currentRenderedMeshPtr->GetSpecularId(),
                                currentRenderedMeshPtr->GetPosition(),
                                currentRenderedMeshPtr->GetOrientation(),
                                currentRenderedMeshPtr->GetScale(),
                                currentRenderedMeshPtr->GetColor(),
                                currentRenderedMeshPtr->GetShininess(),
                                currentRenderedMeshPtr->GetLit()));
                        }
                    }

                    if (!s_batchedRenderedMeshPtrs_.empty())
                    {
                        rendererPtr->RenderBatch();
                    }

                    // Rendering Translucent Instanced Meshes
                    // -------------------------------------------------------------

                    if (!s_translucentInstancedRenderedMeshPtrs_.empty())
                    {
                        unsigned int previousMeshId = s_translucentInstancedRenderedMeshPtrs_[0]->GetMeshId();

                        for (unsigned int i = 0; i < s_translucentInstancedRenderedMeshPtrs_.size(); ++i)
                        {
                            const RenderedMesh*& currentRenderedMeshPtr = s_translucentInstancedRenderedMeshPtrs_[i];

                            const unsigned int& meshId = s_translucentInstancedRenderedMeshPtrs_[i]->GetMeshId();

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
                            meshInstanceData.textureId = currentRenderedMeshPtr->GetTextureId();
                            meshInstanceData.specularId = currentRenderedMeshPtr->GetSpecularId();
                            meshInstanceData.position = currentRenderedMeshPtr->GetPosition();
                            meshInstanceData.orientation = currentRenderedMeshPtr->GetOrientation();
                            meshInstanceData.scale = currentRenderedMeshPtr->GetScale();
                            meshInstanceData.color = currentRenderedMeshPtr->GetColor();
                            meshInstanceData.shininess = currentRenderedMeshPtr->GetShininess();
                            meshInstanceData.lit = currentRenderedMeshPtr->GetLit();

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

    void RenderSystem::GroupMeshPtr(
        const RenderedMesh* renderedMeshPtr,
        const Camera* cameraPtr,
        const FrustumPlanes* frustumPlanesPtr)
    {
        if (!renderedMeshPtr->GetVisible() ||
            !(cameraPtr->GetCameraMask() & renderedMeshPtr->GetCameraMask()))
        {
            return;
        }

        // Frustum Culling

        const glm::vec3& renderedMeshScale = renderedMeshPtr->GetScale();

        float largestScalingFactor = renderedMeshScale.x;

        if (largestScalingFactor < renderedMeshScale.y)
        {
            largestScalingFactor = renderedMeshScale.y;
        }

        if (largestScalingFactor < renderedMeshScale.z)
        {
            largestScalingFactor = renderedMeshScale.z;
        }

        float maxRadius = 0;
        if (renderedMeshPtr->GetRenderedMeshType() == RenderedMesh::RenderedMeshType::RENDERED_MESH_TYPE_LOADED_CPU_SIDE)
        {
            maxRadius = renderedMeshPtr->GetMeshDataPtr()->maxRadius;
        }
        else if (renderedMeshPtr->GetRenderedMeshType() == RenderedMesh::RenderedMeshType::RENDERED_MESH_TYPE_LOADED_GPU_SIDE)
        {
            maxRadius = renderedMeshPtr->GetMaxRadius();
        }

        // This is an approximation. The true larest radius will be less then or equal to this value.
        float scaledMaxRadius = maxRadius * largestScalingFactor;

        bool inFrustum = Check3D_Sphere_Frustum_Overlap(
            renderedMeshPtr->GetPosition(),
            scaledMaxRadius,
            frustumPlanesPtr->leftPlaneNormal_,
            frustumPlanesPtr->leftPlaneDistance_,
            frustumPlanesPtr->rightPlaneNormal_,
            frustumPlanesPtr->rightPlaneDistance_,
            frustumPlanesPtr->bottomPlaneNormal_,
            frustumPlanesPtr->bottomPlaneDistance_,
            frustumPlanesPtr->topPlaneNormal_,
            frustumPlanesPtr->topPlaneDistance_,
            frustumPlanesPtr->nearPlaneNormal_,
            frustumPlanesPtr->nearPlaneDistance_,
            frustumPlanesPtr->farPlaneNormal_,
            frustumPlanesPtr->farPlaneDistance_
        );

        if (!inFrustum)
        {
            return;
        }

        if (renderedMeshPtr->GetRenderedMeshType() == RenderedMesh::RenderedMeshType::RENDERED_MESH_TYPE_LOADED_CPU_SIDE)
        {
            s_batchedRenderedMeshPtrs_.emplace_back(renderedMeshPtr);
        }
        else if (renderedMeshPtr->GetRenderedMeshType() == RenderedMesh::RenderedMeshType::RENDERED_MESH_TYPE_LOADED_GPU_SIDE)
        {
            if (renderedMeshPtr->GetTranslucent())
            {
                s_translucentInstancedRenderedMeshPtrs_.emplace_back(renderedMeshPtr);
            }
            else
            {
                s_instancedRenderedMeshPtrs_.emplace_back(renderedMeshPtr);
            }
        }
    }

    std::vector<Camera*> RenderSystem::s_cameraPtrs_;

    std::vector<const RenderedMesh*> RenderSystem::s_batchedRenderedMeshPtrs_;
    std::vector<const RenderedMesh*> RenderSystem::s_instancedRenderedMeshPtrs_;
    std::vector<const RenderedMesh*> RenderSystem::s_translucentInstancedRenderedMeshPtrs_;

    std::vector<MeshInstanceData> RenderSystem::s_meshInstanceDataArray_;
}