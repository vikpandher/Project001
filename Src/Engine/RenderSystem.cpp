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
            rendererPtr->ClearDirectionalLight();
            rendererPtr->ClearPointLights();
            rendererPtr->ClearSpotLights();

            rendererPtr->BeginRendering();
            rendererPtr->Clear();

            Camera* cameraPtrs = nullptr;
            size_t cameraCount = 0;
            componentStoresPtr->GetAllComponents<Camera>(cameraPtrs, cameraCount);

            Camera* mainCameraPtr = nullptr;
            for (size_t i = 0; i < cameraCount; ++i)
            {
                Camera& currentCamera = cameraPtrs[i];
                if (currentCamera.IsTurnedOn())
                {
                    mainCameraPtr = &currentCamera;
                    break;
                }
            }

            if (mainCameraPtr != nullptr)
            {
                glm::mat4 cameraViewMatrix = mainCameraPtr->GetViewMatrix();
                glm::vec3 cameraPosition = mainCameraPtr->GetPosition();
                glm::mat4 cameraProjectionMatrix = mainCameraPtr->GetProjectionMatrix();

                FrustumPlanes cameraFrustumPlanes;
                mainCameraPtr->GetProjectionFrustumPlanes(cameraFrustumPlanes);

                rendererPtr->SetViewMatrix(cameraViewMatrix);
                rendererPtr->SetViewPosition(cameraPosition);
                rendererPtr->SetProjectionMatrix(cameraProjectionMatrix);

                LightSource* lightSourceArray = nullptr;
                size_t lightSourceCount = 0;
                componentStoresPtr->GetAllComponents<LightSource>(lightSourceArray, lightSourceCount);

                for (unsigned int i = 0; i < lightSourceCount; ++i)
                {
                    LightSource& currentLightSource = lightSourceArray[i];
                    if (currentLightSource.IsTurnedOn())
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

                RenderedModel* renderedModelArrayPtr = nullptr;
                size_t renderedModelCount = 0;
                componentStoresPtr->GetAllComponents<RenderedModel>(renderedModelArrayPtr, renderedModelCount);

                std::vector<RenderedModel*> renderedModelPtrs;
                renderedModelPtrs.reserve(renderedModelCount);
                for (size_t i = 0; i < renderedModelCount; ++i)
                {
                    // Frustum Culling

                    RenderedModel& currentRenderedModel = renderedModelArrayPtr[i];

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

                    // This is an approximation. The true larest radius will be less then or equal to this value.
                    float scaledMaxRadius = currentRenderedModel.GetMeshDataPtr()->maxRadius * largestScalingFactor;

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

                    if (inFrustum)
                    {
                        renderedModelPtrs.emplace_back(&currentRenderedModel);
                    }
                }
                std::sort(renderedModelPtrs.begin(), renderedModelPtrs.end(),
                    [cameraPosition](RenderedModel* a, RenderedModel* b)->bool
                    {
                        bool aTranslucent = a->GetTranslucent();
                        bool bTranslucent = b->GetTranslucent();
                        if (aTranslucent)
                        {
                            if (bTranslucent)
                            {
                                // both are translucent so the farther is drawn first
                                float disatanceSquaredA = glm::dot(cameraPosition - a->GetPosition(), cameraPosition - a->GetPosition());
                                float disatanceSquaredB = glm::dot(cameraPosition - b->GetPosition(), cameraPosition - b->GetPosition());
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
                            float disatanceSquaredA = glm::dot(cameraPosition - a->GetPosition(), cameraPosition - a->GetPosition());
                            float disatanceSquaredB = glm::dot(cameraPosition - b->GetPosition(), cameraPosition - b->GetPosition());
                            return disatanceSquaredA < disatanceSquaredB;
                        }
                    });

                for (unsigned int i = 0; i < renderedModelPtrs.size(); ++i)
                {
                    RenderedModel*& currentRenderedModelPtr = renderedModelPtrs[i];

                    if (currentRenderedModelPtr->IsVisible())
                    {
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
                                currentRenderedModelPtr->GetTranslucent(),
                                currentRenderedModelPtr->GetLit()));
                        }
                    }
                }

                rendererPtr->RenderBatch();
            }

            rendererPtr->FinishRendering();
            rendererPtr->SwapBuffers();
        }
    }
}