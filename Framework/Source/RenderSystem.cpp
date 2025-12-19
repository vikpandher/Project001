// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-19

#include "RenderSystem.h"

#include "Components/Camera.h"
#include "Components/LightSource.h"
#include "Components/RenderedModel.h"
#include "Utilities/Overlap3D.h"
#include "ComponentStores.h"
#include "Logger.h"

#include <algorithm>



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

                if (currentCamera.GetTurnedOn())
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

            bool rendererPreviousDepthTesting = rendererPtr->GetDepthTesting();

            s_cameraEntityIdToRenderedMeshCount_.clear();

            for (size_t i = 0; i < s_cameraPtrs_.size(); ++i)
            {
                Camera& currentCamera = *s_cameraPtrs_[i];
                if (currentCamera.GetTurnedOn())
                {
                    const Project001::Camera::CameraProjection& cameraProjection = currentCamera.GetProjection();

                    const bool& currentCameraDepthTestEnabled = currentCamera.GetDepthTestEnabled();
                    rendererPtr->SetDepthTesting(currentCameraDepthTestEnabled);

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

                    for (size_t j = 0; j < lightSourceCount; ++j)
                    {
                        LightSource& currentLightSource = lightSourceArray[j];
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

                    for (size_t j = 0; j < renderedMeshCount; ++j)
                    {
                        GroupMeshPtr(&renderedMeshArrayPtr[j], &currentCamera, &currentCameraFrustumPlanes);
                    }

                    RenderedModel* renderedModelArrayPtr = nullptr;
                    size_t renderedModelCount = 0;
                    componentStoresPtr->GetAllComponents<RenderedModel>(renderedModelArrayPtr, renderedModelCount);

                    for (size_t j = 0; j < renderedModelCount; ++j)
                    {
                        RenderedModel& currentRenderedModel = renderedModelArrayPtr[j];

                        if (!currentRenderedModel.GetVisible() ||
                            !(currentCamera.GetCameraMask() & currentRenderedModel.GetCameraMask()))
                        {
                            continue;
                        }

                        if (!currentRenderedModel.TransformedMeshesUpToDate())
                        {
                            currentRenderedModel.CalculateTransformedMeshes();
                        }

                        const std::vector<RenderedMesh>& transformedMeshes = currentRenderedModel.GetTransformedRenderedMeshes();
                        for (size_t k = 0; k < transformedMeshes.size(); ++k)
                        {
                            GroupMeshPtr(&transformedMeshes[k], &currentCamera, &currentCameraFrustumPlanes);
                        }
                    }

                    unsigned int currentCameraId = static_cast<unsigned int>(-1);
                    if (componentStoresPtr->GetComponentEntityId<Camera>(currentCameraId, &currentCamera))
                    {
                        s_cameraEntityIdToRenderedMeshCount_[currentCameraId] = s_renderedMeshPtrs_.size();
                    }

                    // Sorting Meshes
                    // ---------------------------------------------------------

                    // Order:
                    // * Meshes with a lower Mesh Priority Override value are
                    //   drawn first. This overrides everything else.
                    // 
                    // * Non-Translucent Meshes are drawn first.
                    //   * Instanced Non-Translucent Meshes are drawn before
                    //     the Batched ones.
                    //     * These are grouped by Id. The smallest Ids are
                    //       drawn first.
                    //     * Within an Id group (all the same Id) the closest
                    //       are drawn first.
                    //   * Batched Non-Translucent Meshes are drawn after the
                    //     Instanced ones.
                    //     * These are ordered so the closest are drawn first.
                    // 
                    // * Translucent Meshes are drawn last.
                    //   * These are orderedso the farthest are drawn first.
                    // 
                    // * If the camera projection is perspective, distances are
                    //   calculated from the camera position point.
                    // * If the camera projection is orthographic, distances are
                    //   calculated from the camera's near cuttoff plane.
                    // 
                    // | Drawn First ---------------------------------------------------- Drawn Last |
                    // | Lower Mesh Priority Override ---------------- Higher Mesh Priority Override |
                    // |                  Non-Translucent                  |       Translucent       |
                    // |        Instanced        |         Batched         |                         |
                    // | Small Id ----- Large Id |                         |                         |
                    // | Closer -------- Farther | Closer -------- Farther | Farther -------- Closer |

                    std::sort(s_renderedMeshPtrs_.begin(), s_renderedMeshPtrs_.end(),
                        [cameraProjection, cameraPosition, currentCameraFrustumPlanes](const RenderedMesh* a, const RenderedMesh* b)->bool // true means "a" comes first
                        {
                            const int& aRenderPriorityOverride = a->GetRenderPriorityOverride();
                            const int& bRenderPriorityOverride = b->GetRenderPriorityOverride();
                            if (aRenderPriorityOverride == bRenderPriorityOverride) // priority override values are the same
                            {
                                if (a->GetTranslucent())
                                {
                                    if (b->GetTranslucent()) // both are translucent
                                    {
                                        float distanceA;
                                        Get3D_Point_Plane_Distance(
                                            a->GetPosition(),
                                            currentCameraFrustumPlanes.nearPlaneNormal_,
                                            currentCameraFrustumPlanes.nearPlaneDistance_,
                                            distanceA
                                        );
                                        float distanceB;
                                        Get3D_Point_Plane_Distance(
                                            b->GetPosition(),
                                            currentCameraFrustumPlanes.nearPlaneNormal_,
                                            currentCameraFrustumPlanes.nearPlaneDistance_,
                                            distanceB
                                        );
                                        return distanceA > distanceB; // the farthest one is drawn first
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
                                            float distanceA;
                                            Get3D_Point_Plane_Distance(
                                                a->GetPosition(),
                                                currentCameraFrustumPlanes.nearPlaneNormal_,
                                                currentCameraFrustumPlanes.nearPlaneDistance_,
                                                distanceA
                                            );
                                            float distanceB;
                                            Get3D_Point_Plane_Distance(
                                                b->GetPosition(),
                                                currentCameraFrustumPlanes.nearPlaneNormal_,
                                                currentCameraFrustumPlanes.nearPlaneDistance_,
                                                distanceB
                                            );
                                            return distanceA < distanceB; // the closest one is drawn first
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
                                            float distanceA;
                                            Get3D_Point_Plane_Distance(
                                                a->GetPosition(),
                                                currentCameraFrustumPlanes.nearPlaneNormal_,
                                                currentCameraFrustumPlanes.nearPlaneDistance_,
                                                distanceA
                                            );
                                            float distanceB;
                                            Get3D_Point_Plane_Distance(
                                                b->GetPosition(),
                                                currentCameraFrustumPlanes.nearPlaneNormal_,
                                                currentCameraFrustumPlanes.nearPlaneDistance_,
                                                distanceB
                                            );
                                            return distanceA < distanceB; // the closest one is drawn first
                                        }
                                    }
                                }
                            }
                            else  // priority override values are different
                            {
                                return aRenderPriorityOverride < bRenderPriorityOverride; // lower render priority override is drawn first
                            }
                        });

                    // Rendering Meshes
                    // ---------------------------------------------------------

                    if (!s_renderedMeshPtrs_.empty())
                    {
                        const RenderedMesh* previousRenderedMeshPtr = s_renderedMeshPtrs_[0];
                        RenderedMesh::RenderedMeshType previousRenderedMeshType = previousRenderedMeshPtr->GetRenderedMeshType();

                        for (unsigned int j = 0; j < s_renderedMeshPtrs_.size(); ++j)
                        {
                            const RenderedMesh*& currentRenderedMeshPtr = s_renderedMeshPtrs_[j];
                            const RenderedMesh::RenderedMeshType& currentRenderedMeshType = currentRenderedMeshPtr->GetRenderedMeshType();

                            if (currentRenderedMeshType == RenderedMesh::RenderedMeshType::RENDERED_MESH_TYPE_LOADED_GPU_SIDE) // current mesh is instanced
                            {
                                if (currentRenderedMeshType != previousRenderedMeshType) // previous mesh was batched
                                {
                                    rendererPtr->RenderBatch();
                                }
                                else if (currentRenderedMeshPtr->GetMeshId() != previousRenderedMeshPtr->GetMeshId()) // previous and current mesh ids are different
                                {
                                    FAIL_CHECK(rendererPtr->RenderMesh(
                                        previousRenderedMeshPtr->GetMeshId(),
                                        s_meshInstanceDataArray_.data(),
                                        static_cast<unsigned int>(s_meshInstanceDataArray_.size())
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
                                    currentRenderedMeshPtr->GetUseLighting()
                                );
                            }
                            else // current mesh is batched
                            {
                                if (currentRenderedMeshType != previousRenderedMeshType) // previous mesh was instanced
                                {
                                    FAIL_CHECK(rendererPtr->RenderMesh(
                                        previousRenderedMeshPtr->GetMeshId(),
                                        s_meshInstanceDataArray_.data(),
                                        static_cast<unsigned int>(s_meshInstanceDataArray_.size())
                                    ));

                                    s_meshInstanceDataArray_.clear();
                                }

                                const MeshData* currentMeshDataPtr = currentRenderedMeshPtr->GetMeshDataPtr();
                                FAIL_CHECK(rendererPtr->AddMeshToBatch(
                                    currentMeshDataPtr->meshVertexArray.data(),
                                    static_cast<unsigned int>(currentMeshDataPtr->meshVertexArray.size()),
                                    currentMeshDataPtr->meshIndexArray.data(),
                                    static_cast<unsigned int>(currentMeshDataPtr->meshIndexArray.size()),
                                    currentRenderedMeshPtr->GetTextureId(),
                                    currentRenderedMeshPtr->GetSpecularId(),
                                    currentRenderedMeshPtr->GetPosition(),
                                    currentRenderedMeshPtr->GetOrientation(),
                                    currentRenderedMeshPtr->GetScale(),
                                    currentRenderedMeshPtr->GetColor(),
                                    currentRenderedMeshPtr->GetShininess(),
                                    currentRenderedMeshPtr->GetUseLighting()
                                ));
                            }

                            previousRenderedMeshPtr = currentRenderedMeshPtr;
                            previousRenderedMeshType = currentRenderedMeshType;
                        }

                        if (previousRenderedMeshType == RenderedMesh::RenderedMeshType::RENDERED_MESH_TYPE_LOADED_GPU_SIDE) // last mesh was instanced
                        {
                            FAIL_CHECK(rendererPtr->RenderMesh(
                                previousRenderedMeshPtr->GetMeshId(),
                                s_meshInstanceDataArray_.data(),
                                static_cast<unsigned int>(s_meshInstanceDataArray_.size())
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

            rendererPtr->SetDepthTesting(rendererPreviousDepthTesting);
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
            if (renderedMeshPtr->GetMeshDataPtr() == nullptr)
            {
                LOG_ERROR_F("Expected a valid MeshDataPtr, but it was nullptr");
                return;
            }

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

    std::unordered_map<unsigned int, size_t> RenderSystem::s_cameraEntityIdToRenderedMeshCount_;
}