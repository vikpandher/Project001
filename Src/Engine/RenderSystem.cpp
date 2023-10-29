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

                    // Amassing Meshes
                    // -------------------------------------------------------------

                    s_renderedMeshPtrs_.clear();

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

                    // Sorting Meshes
                    // ---------------------------------------------------------

                    // Order:
                    // * Non-Translucent Meshes are drawn first.
                    //   * Instanced Non-Translucent Meshes are drawn before the Batched ones.
                    //     * These are grouped by Id. The smallest Ids are drawn first.
                    //     * Within an Id group (all the same Id) the closest are drawn first.
                    //   * Batched Non-Translucent Meshes are drawn after the Instanced ones.
                    //     * These are ordered so the closest are drawn first.
                    // * Translucent Meshes are drawn last.
                    //   * These are orthered so the farthest are drawn first.
                    // 
                    // | Drawn First ---------------------------------------------- Drawn Last |
                    // |                 Non-Translucent                 |     Translucent     |
                    // |        Instanced        |        Batched        |                     |
                    // | Small Id ----- Large Id |                       |                     |
                    // | Closer -------- Farther | Closer ------ Farther | Farther ---- Closer |

                    std::sort(s_renderedMeshPtrs_.begin(), s_renderedMeshPtrs_.end(),
                        [cameraPosition](const RenderedMesh* a, const RenderedMesh* b)->bool // true means "a" comes first
                        {
                            if (a->GetTranslucent())
                            {
                                if (b->GetTranslucent()) // both are translucent
                                {
                                    glm::vec3 distanceA = cameraPosition - a->GetPosition();
                                    glm::vec3 distanceB = cameraPosition - b->GetPosition();
                                    float disatanceSquaredA = glm::dot(distanceA, distanceA);
                                    float disatanceSquaredB = glm::dot(distanceB, distanceB);
                                    return disatanceSquaredA > disatanceSquaredB; // the farther one is drawn first
                                }
                                else // only "a" is translucent
                                {
                                    return false; // the translucent one is drawn last
                                }
                            }
                            else if (b->GetTranslucent()) // only "b" is translucent
                            {
                                return true; // the translucent one is drawn last
                            }
                            else // both are not translucent
                            {
                                if (a->GetRenderedMeshType() == RenderedMesh::RenderedMeshType::RENDERED_MESH_TYPE_LOADED_CPU_SIDE)
                                {
                                    if (b->GetRenderedMeshType() == RenderedMesh::RenderedMeshType::RENDERED_MESH_TYPE_LOADED_CPU_SIDE) // both are batched
                                    {
                                        glm::vec3 distanceA = cameraPosition - a->GetPosition();
                                        glm::vec3 distanceB = cameraPosition - b->GetPosition();
                                        float disatanceSquaredA = glm::dot(distanceA, distanceA);
                                        float disatanceSquaredB = glm::dot(distanceB, distanceB);
                                        return disatanceSquaredA < disatanceSquaredB; // the closest one is drawn first
                                    }
                                    else // only "a" is batched
                                    {
                                        return false; // the batched one is drawn last
                                    }
                                }
                                else if (b->GetRenderedMeshType() == RenderedMesh::RenderedMeshType::RENDERED_MESH_TYPE_LOADED_CPU_SIDE) // only "b" is batched
                                {
                                    return true; // the batched one is drawn last
                                }
                                else // both are instanced
                                {
                                    unsigned int aMeshId = a->GetMeshId();
                                    unsigned int bMeshId = b->GetMeshId();

                                    if (aMeshId != bMeshId) // both have different Ids
                                    {
                                        return aMeshId < bMeshId; // smaller id is drawn first (this groups the ids)
                                    }
                                    else // both have the same id
                                    {
                                        glm::vec3 distanceA = cameraPosition - a->GetPosition();
                                        glm::vec3 distanceB = cameraPosition - b->GetPosition();
                                        float disatanceSquaredA = glm::dot(distanceA, distanceA);
                                        float disatanceSquaredB = glm::dot(distanceB, distanceB);
                                        return disatanceSquaredA < disatanceSquaredB; // the closer one is drawn first
                                    }
                                }
                            }
                        });

                    // Rendering Meshes
                    // ---------------------------------------------------------

                    if (!s_renderedMeshPtrs_.empty())
                    {
                        const RenderedMesh* previousRenderedMeshPtr = s_renderedMeshPtrs_[0];
                        RenderedMesh::RenderedMeshType previousRenderedMeshType = previousRenderedMeshPtr->GetRenderedMeshType();

                        for (unsigned int i = 0; i < s_renderedMeshPtrs_.size(); ++i)
                        {
                            const RenderedMesh*& currentRenderedMeshPtr = s_renderedMeshPtrs_[i];
                            const RenderedMesh::RenderedMeshType& currentRenderedMeshType = currentRenderedMeshPtr->GetRenderedMeshType();

                            if (currentRenderedMeshType == RenderedMesh::RenderedMeshType::RENDERED_MESH_TYPE_LOADED_GPU_SIDE) // current mesh is instanced
                            {
                                if (currentRenderedMeshType != previousRenderedMeshType) // previous mesh was batched
                                {
                                    rendererPtr->RenderBatch();
                                }
                                else if (currentRenderedMeshPtr->GetMeshId() != previousRenderedMeshPtr->GetMeshId()) // previous and current mesh ids are different
                                {
                                    _FAIL_CHECK(rendererPtr->RenderMesh(
                                        previousRenderedMeshPtr->GetMeshId(),
                                        s_meshInstanceDataArray_.data(),
                                        (unsigned int)s_meshInstanceDataArray_.size()
                                    ));

                                    s_meshInstanceDataArray_.clear();
                                }

                                s_meshInstanceDataArray_.emplace_back(
                                    currentRenderedMeshPtr->GetTextureId(),
                                    currentRenderedMeshPtr->GetSpecularId(),
                                    currentRenderedMeshPtr->GetPosition(),
                                    currentRenderedMeshPtr->GetOrientation(),
                                    currentRenderedMeshPtr->GetScale(),
                                    currentRenderedMeshPtr->GetColor(),
                                    currentRenderedMeshPtr->GetShininess(),
                                    currentRenderedMeshPtr->GetLit()
                                );
                            }
                            else // current mesh is batched
                            {
                                if (currentRenderedMeshType != previousRenderedMeshType) // previous mesh was instanced
                                {
                                    _FAIL_CHECK(rendererPtr->RenderMesh(
                                        previousRenderedMeshPtr->GetMeshId(),
                                        s_meshInstanceDataArray_.data(),
                                        (unsigned int)s_meshInstanceDataArray_.size()
                                    ));

                                    s_meshInstanceDataArray_.clear();
                                }

                                const MeshData* currentMeshDataPtr = currentRenderedMeshPtr->GetMeshDataPtr();
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
                                    currentRenderedMeshPtr->GetLit()
                                ));
                            }

                            previousRenderedMeshPtr = currentRenderedMeshPtr;
                            previousRenderedMeshType = currentRenderedMeshType;
                        }

                        if (previousRenderedMeshType == RenderedMesh::RenderedMeshType::RENDERED_MESH_TYPE_LOADED_GPU_SIDE) // last mesh was instanced
                        {
                            _FAIL_CHECK(rendererPtr->RenderMesh(
                                previousRenderedMeshPtr->GetMeshId(),
                                s_meshInstanceDataArray_.data(),
                                (unsigned int)s_meshInstanceDataArray_.size()
                            ));

                            s_meshInstanceDataArray_.clear();
                        }
                        else // last mesh was batched
                        {
                            rendererPtr->RenderBatch();
                        }
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

        float maxBoundingRadius = 0;
        if (renderedMeshPtr->GetRenderedMeshType() == RenderedMesh::RenderedMeshType::RENDERED_MESH_TYPE_LOADED_CPU_SIDE)
        {
            maxBoundingRadius = renderedMeshPtr->GetMeshDataPtr()->maxBoundingRadius;
        }
        else if (renderedMeshPtr->GetRenderedMeshType() == RenderedMesh::RenderedMeshType::RENDERED_MESH_TYPE_LOADED_GPU_SIDE)
        {
            maxBoundingRadius = renderedMeshPtr->GetMaxBoundingRadius();
        }

        // This is an approximation. The true larest radius will be less then or equal to this value.
        float scaledMaxBoundingRadius = maxBoundingRadius * largestScalingFactor;

        bool inFrustum = Check3D_Sphere_Frustum_Overlap(
            renderedMeshPtr->GetPosition(),
            scaledMaxBoundingRadius,
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

        if (renderedMeshPtr->GetRenderedMeshType() != RenderedMesh::RenderedMeshType::RENDERED_MESH_TYPE_NOT_LOADED)
        {
            s_renderedMeshPtrs_.emplace_back(renderedMeshPtr);
        }
    }

    std::vector<Camera*> RenderSystem::s_cameraPtrs_;

    std::vector<const RenderedMesh*> RenderSystem::s_renderedMeshPtrs_;

    std::vector<MeshInstanceData> RenderSystem::s_meshInstanceDataArray_;
}