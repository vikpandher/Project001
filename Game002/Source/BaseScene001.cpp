// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-08-18

#include "BaseScene001.h"

#include "Components/Camera.h"
#include "ComponentStores.h"
#include "Logger.h"
#include "RenderSystem.h"
#include "Window.h"



// public ----------------------------------------------------------------------

BaseScene001::BaseScene001(Project001::Application* applicationPtr)
    : Scene(applicationPtr)
{
    sharedDataPtr_ = GetSharedDataPtr<SharedApplicationData>();
}

BaseScene001::~BaseScene001()
{}

void BaseScene001::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&BaseScene001::ProcessDeinitializeEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&BaseScene001::ProcessRenderEvent, this, std::placeholders::_1));
}

// protected -------------------------------------------------------------------

void BaseScene001::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    skipRenderingOnce_ = true;
}

void BaseScene001::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    if (skipRenderingOnce_) // Makes sure that UpdateEvent occurs once before Rendering
    {
        skipRenderingOnce_ = false;
    }
    else
    {
        GetRenderSystemPtr()->Render();
    }
}

void BaseScene001::CreateMainCameraEntity(unsigned int& entityId, const uint32_t cameraMask, int priorityValue)
{
    int aspectRatioNumerator;
    int aspectRatioDenominator;
    GetWindowPtr()->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);

    float aspectRatio = static_cast<float>(aspectRatioNumerator) / static_cast<float>(aspectRatioDenominator);
    constexpr float mainCameraHalfHeight = 320.0f;
    float mainCameraHalfWidth = aspectRatio * mainCameraHalfHeight;

    constexpr float mainCameraNearCutoff = mainCameraHalfHeight * 0.1f;
    constexpr float mainCameraFarCutoff = mainCameraHalfHeight * 24.0f;

    constexpr float mainCameraYaw = glm::pi<float>();

    constexpr Project001::Camera::CameraProjection mainCameraProjection =
        Project001::Camera::CameraProjection::CAMERA_PROJECTION_PERSPECTIVE;

    GetComponentStoresPtr()->CreateEntity(entityId);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::Camera>(entityId));
    Project001::Camera* cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, entityId));
    if (cameraPtr != nullptr)
    {
        if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
        {
            cameraPtr->SetAspectRatio(aspectRatio);
            cameraPtr->SetTopCutoff(mainCameraHalfHeight);
            cameraPtr->SetBottomCutoff(-mainCameraHalfHeight);
            cameraPtr->SetLeftCutoff(-mainCameraHalfWidth);
            cameraPtr->SetRightCutoff(mainCameraHalfWidth);
            cameraPtr->SetNearCutoff(mainCameraNearCutoff);
            cameraPtr->SetFarCutoff(mainCameraFarCutoff);
        }

        cameraPtr->AddPitch(sharedDataPtr_->mainCameraPitch);
        cameraPtr->AddYaw(mainCameraYaw);
        cameraPtr->SetProjection(mainCameraProjection);
        cameraPtr->SetCameraMask(cameraMask);
        cameraPtr->SetPriorityValue(priorityValue);
    }
}

void BaseScene001::CreateUiCameraEntity(unsigned int& entityId, const uint32_t cameraMask, int priorityValue)
{
    GetComponentStoresPtr()->CreateEntity(entityId);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::Camera>(entityId));
    Project001::Camera* cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, entityId));
    if (cameraPtr != nullptr)
    {
        int aspectRatioNumerator;
        int aspectRatioDenominator;
        GetWindowPtr()->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);
        if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
        {
            float aspectRatio = static_cast<float>(aspectRatioNumerator) / static_cast<float>(aspectRatioDenominator);
            float uiCameraHalfHeight = 320.0f;
            float uiCameraHalfWidth = aspectRatio * uiCameraHalfHeight;
            cameraPtr->SetAspectRatio(aspectRatio);
            cameraPtr->SetTopCutoff(uiCameraHalfHeight);
            cameraPtr->SetBottomCutoff(-uiCameraHalfHeight);
            cameraPtr->SetLeftCutoff(-uiCameraHalfWidth);
            cameraPtr->SetRightCutoff(uiCameraHalfWidth);
            cameraPtr->SetNearCutoff(-32.0f);
            cameraPtr->SetFarCutoff(32.0f);
        }
        cameraPtr->AddYaw(glm::pi<float>());
        cameraPtr->SetProjection(Project001::Camera::CameraProjection::CAMERA_PROJECTION_ORTHOGRAPHIC);
        cameraPtr->SetDepthTestEnabled(false);
        cameraPtr->SetCameraMask(cameraMask);
        cameraPtr->SetPriorityValue(priorityValue);
    }
}