// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-09-01

#include "TestSceneBase002.h"

#include "TestApplicationData.h"
#include "TestResource_AntonioRegular_png.h"
#include "TestResource_AntonioRegular_ssf.h"

#include "Components/Camera.h"
#include "Components/CollisionBody2D.h"
#include "CollisionSystem2D.h"
#include "Components/RenderedMesh.h"
#include "Math/Overlap2D.h"
#include "CollisionSystem2D.h"
#include "ComponentStores.h"
#include "FontLoader.h"
#include "Logger.h"
#include "MeshLoader.h"
#include "RenderSystem.h"
#include "TextureLoader.h"
#include "Window.h"

#include <stack>



// public ----------------------------------------------------------------------

TestSceneBase002::TestSceneBase002(Project001::Application* applicationPtr)
    : Scene(applicationPtr)
    , font01_FontDataPtr_(nullptr)
    , font01_TextureDataPtr_(nullptr)
    , font01_TextureId_((unsigned int)-1)
    , text_dynamic_TextureId_((unsigned int)-1)
    , text_overlapOnly_TextureId_((unsigned int)-1)
    , text_rotationOnly_TextureId_((unsigned int)-1)
    , text_static_TextureId_((unsigned int)-1)
    , text_translationOnly_TextureId_((unsigned int)-1)
    , cursorLinePositions_()
    , cursorLineMeshDataPtr_(nullptr)
    , distanceTextMeshDataPtr_(nullptr)
    , massTextMeshDataPtr_(nullptr)
    , momentOfInertiaTextMeshDataPtr_(nullptr)
    , entityIdTextMeshDataPtr_(nullptr)
    , fps_MeshDataPtr_(nullptr)
    , energy_MeshDataPtr_(nullptr)
    , collisionBodyBorderMeshDataPtr_(nullptr)
    , collisionBodyQuadTreeMeshDataPtr_(nullptr)
    , collisionMarkerCollectionMeshDataPtr_(nullptr)
    , meshDataPtrArray_()
    , mainCameraEntityId_((unsigned int)-1)
    , uiCameraEntityId_((unsigned int)-1)
    , cursorEntityId_((unsigned int)-1)
    , distanceEntityId_((unsigned int)-1)
    , massEntityId_((unsigned int)-1)
    , momentOfInertiaEntityId_((unsigned int)-1)
    , entityIdTextEntityId_((unsigned int)-1)
    , fpsTextEntityId_((unsigned int)-1)
    , energyTextEntityId_((unsigned int)-1)
    , collisionBodyBorderEntityId_((unsigned int)-1)
    , collisionBodyQuadTreeEntityId_((unsigned int)-1)
    , collisionMarkerCollectionEntityId_((unsigned int)-1)
    , entityIds_()
    , cursorGrabbingEntity_(false)
    , previousWorldCursorPosition_()
    , selectedEntityIdIndex_((unsigned int)-1)
    , remainingTimeRecordingDuration_ns_(0)
    , positionBorderSize_(16.0f, 12.0f)
    , maxVelocity_(10.0f)
    , maxAngularVelocity_(4.0f * glm::pi<float>())
    , generateCursorLineAndDistanceTextMesh_(true)
    , generateMassTextMesh_(true)
    , generateMomentOfInertiaTextMesh_(true)
    , generateEntityIdTextMesh_(true)
    , generateFpsTextMesh_(true)
    , generateEnergyTextMesh_(true)
    , generateCollisionBodyQuadTreeMesh_(true)
    , generateCollisionMarkerCollectionMesh_(true)
    , applyGravity_(false)
    , updateCollisionBodyTexture_(true)
    , recolorOverlappingCollisionBodies_(true)
    , velocityBasedMovement_(false)
    , physicsStepsPerUpdate_(1)
    , useCollisionBodyQuadTree_(true)
    , gravity_(0.0f, -1.0f)
{}

TestSceneBase002::~TestSceneBase002()
{}

void TestSceneBase002::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestSceneBase002::ProcessInitializeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestSceneBase002::ProcessDeinitializeEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::CursorPositionEvent>(event, std::bind(&TestSceneBase002::ProcessCursorPositionEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestSceneBase002::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::MouseButtonEvent>(event, std::bind(&TestSceneBase002::ProcessMouseButtonEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestSceneBase002::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::ScrollEvent>(event, std::bind(&TestSceneBase002::ProcessScrollEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestSceneBase002::ProcessUpdateEvent, this, std::placeholders::_1));
}

// protected -------------------------------------------------------------------

void TestSceneBase002::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   TestSceneBase002:        " << GetId());

    // Load textures
    // -------------------------------------------------------------------------

    {
        font01_FontDataPtr_ = new Project001::FontData;
        FAIL_CHECK(Project001::FontLoader::LoadFontDataFromMemory(
            *font01_FontDataPtr_,
            g_AntonioRegular_ssf,
            sizeof(g_AntonioRegular_ssf)
        ));

        font01_TextureDataPtr_ = new Project001::TextureData;
        FAIL_CHECK(Project001::TextureLoader::LoadTextureFromMemory(
            *font01_TextureDataPtr_,
            g_AntonioRegular_png,
            sizeof(g_AntonioRegular_png)
        ));
        GetRendererPtr()->CreateTexture(
            font01_TextureId_,
            font01_TextureDataPtr_->data,
            font01_TextureDataPtr_->width,
            font01_TextureDataPtr_->height,
            font01_TextureDataPtr_->bytesPerPixel,
            true,
            false
        );
    }

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/Text_Dynamic.png"));
        GetRendererPtr()->CreateTexture(text_dynamic_TextureId_, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/Text_OverlapOnly.png"));
        GetRendererPtr()->CreateTexture(text_overlapOnly_TextureId_, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/Text_RotationOnly.png"));
        GetRendererPtr()->CreateTexture(text_rotationOnly_TextureId_, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/Text_Static.png"));
        GetRendererPtr()->CreateTexture(text_static_TextureId_, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/Text_TranslationOnly.png"));
        GetRendererPtr()->CreateTexture(text_translationOnly_TextureId_, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    // Generate meshes
    // -------------------------------------------------------------------------

    cursorLineMeshDataPtr_ = new Project001::MeshData();

    distanceTextMeshDataPtr_ = new Project001::MeshData();

    massTextMeshDataPtr_ = new Project001::MeshData();

    momentOfInertiaTextMeshDataPtr_ = new Project001::MeshData();

    entityIdTextMeshDataPtr_ = new Project001::MeshData();

    fps_MeshDataPtr_ = new Project001::MeshData();

    energy_MeshDataPtr_ = new Project001::MeshData();

    collisionBodyBorderMeshDataPtr_ = new Project001::MeshData();

    collisionBodyQuadTreeMeshDataPtr_ = new Project001::MeshData();

    collisionMarkerCollectionMeshDataPtr_ = new Project001::MeshData();

    // Main Camera Entity
    // -------------------------------------------------------------------------

    {
        GetComponentStoresPtr()->CreateEntity(mainCameraEntityId_);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::Camera>(mainCameraEntityId_));
        Project001::Camera* cameraPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraEntityId_));
        if (cameraPtr != nullptr)
        {
            int aspectRatioNumerator;
            int aspectRatioDenominator;
            GetWindowPtr()->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);
            if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
            {
                float aspectRatio = (float)aspectRatioNumerator / (float)aspectRatioDenominator;
                float secondaryCameraHalfHeight = 3.5f;
                float secondaryCameraHalfWidth = aspectRatio * secondaryCameraHalfHeight;
                cameraPtr->SetAspectRatio(aspectRatio);
                cameraPtr->SetTopCutoff(secondaryCameraHalfHeight);
                cameraPtr->SetBottomCutoff(-secondaryCameraHalfHeight);
                cameraPtr->SetLeftCutoff(-secondaryCameraHalfWidth);
                cameraPtr->SetRightCutoff(secondaryCameraHalfWidth);
                cameraPtr->SetNearCutoff(-1.0f);
                cameraPtr->SetFarCutoff(1.0f);
            }
            cameraPtr->AddYaw(glm::pi<float>());
            cameraPtr->SetProjection(Project001::Camera::CameraProjection::CAMERA_PROJECTION_ORTHOGRAPHIC);
            cameraPtr->SetDepthTestEnabled(false);
            cameraPtr->TurnOn();
            cameraPtr->SetCameraMask(s_mainCameraMask_);
        }
    }

    // UI Camera Entity
    // -------------------------------------------------------------------------

    float uiCameraHalfHeight = 0.0f;
    float uiCameraHalfWidth = 0.0f;

    {
        GetComponentStoresPtr()->CreateEntity(uiCameraEntityId_);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::Camera>(uiCameraEntityId_));
        Project001::Camera* cameraPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, uiCameraEntityId_));
        if (cameraPtr != nullptr)
        {
            int aspectRatioNumerator;
            int aspectRatioDenominator;
            GetWindowPtr()->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);
            if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
            {
                float aspectRatio = (float)aspectRatioNumerator / (float)aspectRatioDenominator;
                uiCameraHalfHeight = 3.5f;
                uiCameraHalfWidth = aspectRatio * uiCameraHalfHeight;
                cameraPtr->SetAspectRatio(aspectRatio);
                cameraPtr->SetTopCutoff(uiCameraHalfHeight);
                cameraPtr->SetBottomCutoff(-uiCameraHalfHeight);
                cameraPtr->SetLeftCutoff(-uiCameraHalfWidth);
                cameraPtr->SetRightCutoff(uiCameraHalfWidth);
                cameraPtr->SetNearCutoff(-1.0f);
                cameraPtr->SetFarCutoff(1.0f);
            }
            cameraPtr->AddYaw(glm::pi<float>());
            cameraPtr->SetProjection(Project001::Camera::CameraProjection::CAMERA_PROJECTION_ORTHOGRAPHIC);
            cameraPtr->SetDepthTestEnabled(false);
            cameraPtr->TurnOn();
            cameraPtr->SetCameraMask(s_uiCameraMask_);
            cameraPtr->SetPriorityValue(1000000);
        }
    }

    // Cursor Entity
    // -------------------------------------------------------------------------

    {
        GetComponentStoresPtr()->CreateEntity(cursorEntityId_);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(cursorEntityId_));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, cursorEntityId_));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetLit(false);
            renderedMeshPtr->SetMeshDataPtr(cursorLineMeshDataPtr_);
            renderedMeshPtr->SetColor(0.4f, 0.4f, 0.4f, 1.0f);
            renderedMeshPtr->SetRenderPriorityOverride(10);
        }

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(cursorEntityId_));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursorEntityId_));
        if (collisionBody2DPtr != nullptr)
        {
            std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
            collisionPoints.emplace_back(glm::vec3(), 1, true);
        }
    }

    // Distance Text Entity
    // -------------------------------------------------------------------------

    {
        GetComponentStoresPtr()->CreateEntity(distanceEntityId_);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(distanceEntityId_));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, distanceEntityId_));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_uiCameraMask_);
            renderedMeshPtr->SetLit(false);
            renderedMeshPtr->SetMeshDataPtr(distanceTextMeshDataPtr_);
            renderedMeshPtr->SetTextureId(font01_TextureId_);
            renderedMeshPtr->SetTranslucent(true);
            renderedMeshPtr->SetPositionY(uiCameraHalfHeight - 0.3f);
            renderedMeshPtr->SetColor(0.8f, 0.7f, 0.3f, 1.0f);
        }
    }

    // Mass Text Entity
    // -------------------------------------------------------------------------

    {
        GetComponentStoresPtr()->CreateEntity(massEntityId_);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(massEntityId_));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, massEntityId_));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_uiCameraMask_);
            renderedMeshPtr->SetLit(false);
            renderedMeshPtr->SetMeshDataPtr(massTextMeshDataPtr_);
            renderedMeshPtr->SetTextureId(font01_TextureId_);
            renderedMeshPtr->SetTranslucent(true);
            renderedMeshPtr->SetPositionY(uiCameraHalfHeight - 0.6f);
            renderedMeshPtr->SetColor(0.8f, 0.7f, 0.3f, 1.0f);
        }
    }

    // MomentOfInertia Text Entity
    // -------------------------------------------------------------------------

    {
        GetComponentStoresPtr()->CreateEntity(momentOfInertiaEntityId_);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(momentOfInertiaEntityId_));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, momentOfInertiaEntityId_));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_uiCameraMask_);
            renderedMeshPtr->SetLit(false);
            renderedMeshPtr->SetMeshDataPtr(momentOfInertiaTextMeshDataPtr_);
            renderedMeshPtr->SetTextureId(font01_TextureId_);
            renderedMeshPtr->SetTranslucent(true);
            renderedMeshPtr->SetPositionY(uiCameraHalfHeight - 0.9f);
            renderedMeshPtr->SetColor(0.8f, 0.7f, 0.3f, 1.0f);
        }
    }

    // Entity Id Text Entity
    // -------------------------------------------------------------------------

    {
        GetComponentStoresPtr()->CreateEntity(entityIdTextEntityId_);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(entityIdTextEntityId_));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, entityIdTextEntityId_));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_uiCameraMask_);
            renderedMeshPtr->SetLit(false);
            renderedMeshPtr->SetMeshDataPtr(entityIdTextMeshDataPtr_);
            renderedMeshPtr->SetTextureId(font01_TextureId_);
            renderedMeshPtr->SetTranslucent(true);
            renderedMeshPtr->SetPositionX(-uiCameraHalfWidth + 0.2f);
            renderedMeshPtr->SetPositionY(-uiCameraHalfHeight + 0.2f);
            renderedMeshPtr->SetColor(0.8f, 0.7f, 0.3f, 1.0f);
        }
    }

    // FPS Text Entity
    // -------------------------------------------------------------------------

    {
        GetComponentStoresPtr()->CreateEntity(fpsTextEntityId_);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(fpsTextEntityId_));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, fpsTextEntityId_));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_uiCameraMask_);
            renderedMeshPtr->SetLit(false);
            renderedMeshPtr->SetMeshDataPtr(fps_MeshDataPtr_);
            renderedMeshPtr->SetTextureId(font01_TextureId_);
            renderedMeshPtr->SetTranslucent(true);
            renderedMeshPtr->SetPositionX(uiCameraHalfWidth - 1.0f);
            renderedMeshPtr->SetPositionY(uiCameraHalfHeight - 0.3f);
            renderedMeshPtr->SetColor(0.8f, 0.7f, 0.3f, 1.0f);
        }
    }

    // Energy Text Entity
    // -------------------------------------------------------------------------

    {
        GetComponentStoresPtr()->CreateEntity(energyTextEntityId_);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(energyTextEntityId_));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, energyTextEntityId_));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_uiCameraMask_);
            renderedMeshPtr->SetLit(false);
            renderedMeshPtr->SetMeshDataPtr(energy_MeshDataPtr_);
            renderedMeshPtr->SetTextureId(font01_TextureId_);
            renderedMeshPtr->SetTranslucent(true);
            renderedMeshPtr->SetPositionX(uiCameraHalfWidth - 3.2f);
            renderedMeshPtr->SetPositionY(-uiCameraHalfHeight + 0.2f);
            renderedMeshPtr->SetColor(0.8f, 0.7f, 0.3f, 1.0f);
        }
    }

    // Collision Body Border Entity
    // -------------------------------------------------------------------------

    {
        GetComponentStoresPtr()->CreateEntity(collisionBodyBorderEntityId_);
        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(collisionBodyBorderEntityId_));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, collisionBodyBorderEntityId_));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetLit(false);
            renderedMeshPtr->SetMeshDataPtr(collisionBodyBorderMeshDataPtr_);
            renderedMeshPtr->SetColor(0.1f, 0.2f, 0.2f, 1.0f);
            renderedMeshPtr->SetRenderPriorityOverride(-99);
        }
    }

    // Collision Body Quad-Tree Entity
    // -------------------------------------------------------------------------

    {
        GetComponentStoresPtr()->CreateEntity(collisionBodyQuadTreeEntityId_);
        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(collisionBodyQuadTreeEntityId_));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, collisionBodyQuadTreeEntityId_));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetLit(false);
            renderedMeshPtr->SetMeshDataPtr(collisionBodyQuadTreeMeshDataPtr_);
            renderedMeshPtr->SetColor(0.2f, 0.2f, 0.2f, 1.0f);
            renderedMeshPtr->SetRenderPriorityOverride(-100);
        }
    }

    // Collision Marker Collection Entity
    // -------------------------------------------------------------------------
    {
        GetComponentStoresPtr()->CreateEntity(collisionMarkerCollectionEntityId_);
        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(collisionMarkerCollectionEntityId_));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, collisionMarkerCollectionEntityId_));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetLit(false);
            renderedMeshPtr->SetMeshDataPtr(collisionMarkerCollectionMeshDataPtr_);
            renderedMeshPtr->SetColor(0.2f, 0.8f, 0.6f, 0.4f);
            renderedMeshPtr->SetRenderPriorityOverride(2);
        }
    }
}

void TestSceneBase002::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    LOG_INFO("DEINITIALIZING: TestSceneBase002:        " << GetId());

    GetRendererPtr()->DeleteAllTextures();
    GetRendererPtr()->DeleteAllMeshes();
    GetComponentStoresPtr()->DeleteAllEntities();

    // Texture Data: -----------------------------------------------------------

    delete font01_FontDataPtr_;
    font01_FontDataPtr_ = nullptr;

    delete font01_TextureDataPtr_;
    font01_TextureDataPtr_ = nullptr;

    font01_TextureId_ = (unsigned int)-1;

    text_dynamic_TextureId_ = (unsigned int)-1;
    text_overlapOnly_TextureId_ = (unsigned int)-1;
    text_rotationOnly_TextureId_ = (unsigned int)-1;
    text_static_TextureId_ = (unsigned int)-1;
    text_translationOnly_TextureId_ = (unsigned int)-1;

    // Mesh Data ---------------------------------------------------------------

    delete cursorLineMeshDataPtr_;
    cursorLineMeshDataPtr_ = nullptr;

    delete distanceTextMeshDataPtr_;
    distanceTextMeshDataPtr_ = nullptr;

    delete massTextMeshDataPtr_;
    massTextMeshDataPtr_ = nullptr;

    delete momentOfInertiaTextMeshDataPtr_;
    momentOfInertiaTextMeshDataPtr_ = nullptr;

    delete entityIdTextMeshDataPtr_;
    entityIdTextMeshDataPtr_ = nullptr;

    delete fps_MeshDataPtr_;
    fps_MeshDataPtr_ = nullptr;

    delete energy_MeshDataPtr_;
    energy_MeshDataPtr_ = nullptr;

    delete collisionBodyBorderMeshDataPtr_;
    collisionBodyBorderMeshDataPtr_ = nullptr;

    delete collisionBodyQuadTreeMeshDataPtr_;
    collisionBodyQuadTreeMeshDataPtr_ = nullptr;

    delete collisionMarkerCollectionMeshDataPtr_;
    collisionMarkerCollectionMeshDataPtr_ = nullptr;

    for (size_t i = 0; i < meshDataPtrArray_.size(); ++i)
    {
        delete meshDataPtrArray_[i];
    }
    meshDataPtrArray_.clear();

    // Entity Ids --------------------------------------------------------------

    mainCameraEntityId_ = (unsigned int)-1;
    uiCameraEntityId_ = (unsigned int)-1;

    cursorEntityId_ = (unsigned int)-1;

    distanceEntityId_ = (unsigned int)-1;

    massEntityId_ = (unsigned int)-1;

    momentOfInertiaEntityId_ = (unsigned int)-1;

    entityIdTextEntityId_ = (unsigned int)-1;

    fpsTextEntityId_ = (unsigned int)-1;

    energyTextEntityId_ = (unsigned int)-1;

    collisionBodyBorderEntityId_ = (unsigned int)-1;

    collisionBodyQuadTreeEntityId_ = (unsigned int)-1;

    collisionMarkerCollectionEntityId_ = (unsigned int)-1;

    entityIds_.clear();

    // -------------------------------------------------------------------------

    cursorGrabbingEntity_ = false;
    previousWorldCursorPosition_ = glm::vec2(0.0f, 0.0f);
    selectedEntityIdIndex_ = (unsigned int)-1;

    if (remainingTimeRecordingDuration_ns_ > 0)
    {
        remainingTimeRecordingDuration_ns_ = 0;

        if (Project001::TimeProfiler::EndSession())
        {
            LOG_INFO("RECORDING END");
        }
    }
}

void TestSceneBase002::ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorPositionEvent)
{
    const float& xPosition = cursorPositionEvent.xPosition;
    const float& yPosition = cursorPositionEvent.yPosition;

    UpdateWorldCursor(xPosition, yPosition);
}

void TestSceneBase002::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_ESCAPE)
        {
            TestApplicationData* testApplicationDataPtr = GetSharedDataPtr<TestApplicationData>();
            SendEventToApplication(Project001::SwitchSceneEvent(testApplicationDataPtr->testScene001Id));
            if (GetActiveScene()->GetId() == testApplicationDataPtr->testScene001Id)
            {
                SendEventToScene(GetId(), Project001::DeinitializeEvent());
                SendEventToApplication(Project001::InitializeEvent());
            }
            return;
        }
        else if (keyCode == Project001::KeyCode::KEY_CODE_N)
        {
            selectedEntityIdIndex_++;
            if (selectedEntityIdIndex_ >= entityIds_.size())
            {
                selectedEntityIdIndex_ = 0;
            }
        }
        else if (keyCode == Project001::KeyCode::KEY_CODE_B)
        {
            if (selectedEntityIdIndex_ == 0)
            {
                selectedEntityIdIndex_ = (unsigned int)entityIds_.size() - 1;
            }
            else if (selectedEntityIdIndex_ >= entityIds_.size())
            {
                selectedEntityIdIndex_ = (unsigned int)entityIds_.size() - 1;
            }
            else
            {
                selectedEntityIdIndex_--;
            }
        }
        else if (keyCode == Project001::KeyCode::KEY_CODE_R)
        {
            if (selectedEntityIdIndex_ < entityIds_.size())
            {
                const unsigned int& selectedEntityId = entityIds_[selectedEntityIdIndex_];
                Project001::CollisionBody2D* collisionBodyPtr = nullptr;
                bool collisionBodyFound = GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, selectedEntityId);
                if (collisionBodyFound)
                {
                    collisionBodyPtr->SetFixedRotation(!collisionBodyPtr->GetFixedRotation());
                }
            }
        }
        else if (keyCode == Project001::KeyCode::KEY_CODE_T)
        {
            if (selectedEntityIdIndex_ < entityIds_.size())
            {
                const unsigned int& selectedEntityId = entityIds_[selectedEntityIdIndex_];
                Project001::CollisionBody2D* collisionBodyPtr = nullptr;
                bool collisionBodyFound = GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, selectedEntityId);
                if (collisionBodyFound)
                {
                    collisionBodyPtr->SetFixedTranslation(!collisionBodyPtr->GetFixedTranslation());
                }
            }
        }
        else if (keyCode == Project001::KeyCode::KEY_CODE_G)
        {
            applyGravity_ = !applyGravity_;
        }
        else if (keyCode == Project001::KeyCode::KEY_CODE_0)
        {
            std::string timeProfileOutFileName = "TestSceneBase002_";
#ifndef NDEBUG
            timeProfileOutFileName += "D_";
#else
            timeProfileOutFileName += "R_";
#endif
            if (GetSleepyRunLoop())
            {
                timeProfileOutFileName += "Sleepy_";
            }
            else
            {
                timeProfileOutFileName += "Fast_";
            }
            timeProfileOutFileName += std::to_string(Project001::TimeProfiler::GetTimeStamp());
            timeProfileOutFileName += ".json";
            if (Project001::TimeProfiler::BeginSession(timeProfileOutFileName.c_str()))
            {
                LOG_INFO("RECORDING START");
            }
            remainingTimeRecordingDuration_ns_ = 1000000000;
        }
    }
}

void TestSceneBase002::ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent)
{
    Project001::MouseButton& mouseButton = mouseButtonEvent.mouseButton;
    Project001::ButtonAction& buttonAction = mouseButtonEvent.buttonAction;

    cursorGrabbingEntity_ = false;

    if (mouseButton == Project001::MouseButton::MOUSE_BUTTON_1 &&
        buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS)
    {
        selectedEntityIdIndex_ = (unsigned int)-1;

        Project001::CollisionSystem2D::CalculateOverlapForGivenEntity(cursorEntityId_, GetComponentStoresPtr());

        Project001::CollisionBody2D* cursorCollisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(cursorCollisionBody2DPtr, cursorEntityId_));
        if(cursorCollisionBody2DPtr != nullptr)
        {
            const std::vector<Project001::CollisionData2D>& cursorCollisions = cursorCollisionBody2DPtr->GetCollisions();

            // This loop goes backwards to grab the component drawn last first.
            // This relies on the order the components and render bodies were 
            // added to select the one rendered behind.
            for (int i = (int)cursorCollisions.size() - 1; i >= 0; --i)
            // for (size_t i = 0; i < cursorCollisions.size(); ++i)
            {
                const Project001::CollisionData2D& currentCollision = cursorCollisions[i];

                for (unsigned int j = 0; j < entityIds_.size(); ++j)
                {
                    if (currentCollision.otherEntityId == entityIds_[j])
                    {
                        selectedEntityIdIndex_ = j;

                        cursorGrabbingEntity_ = true;

                        break;
                    }
                }
            }
        }
    }
}

void TestSceneBase002::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    Project001::ScopeTimer scopeTimer("TestSceneBase002::ProcessRenderEvent");

    Sync_RenderedMesh_CollisionBody_Components();

    if (generateFpsTextMesh_)
    {
        UpdateFpsTextMesh(renderEvent.timestep_ns);
    }

    Project001::RenderSystem::Render(GetComponentStoresPtr(), GetRendererPtr());
}

void TestSceneBase002::ProcessScrollEvent(Project001::ScrollEvent& scrollEvent)
{
    float& yOffset = scrollEvent.yOffset;

    float speedConstant = 0.1f;
    float cameraResize = 1.0f + speedConstant * yOffset;

    Project001::Camera* cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraEntityId_));
    if (cameraPtr != nullptr)
    {
        float newTopCutoff = cameraPtr->GetTopCutoff() * cameraResize;
        float newBottomCutoff = cameraPtr->GetBottomCutoff() * cameraResize;
        float newLeftCutoff = cameraPtr->GetLeftCutoff() * cameraResize;
        float newRightCutoff = cameraPtr->GetRightCutoff() * cameraResize;

        if (newTopCutoff > 0.1f && newTopCutoff < 10.0f)
        {
            cameraPtr->SetTopCutoff(newTopCutoff);
            cameraPtr->SetBottomCutoff(newBottomCutoff);
            cameraPtr->SetLeftCutoff(newLeftCutoff);
            cameraPtr->SetRightCutoff(newRightCutoff);
        }
    }

    float xPosition = 0.0f;
    float yPosition = 0.0f;
    GetWindowPtr()->GetCursorPosition(xPosition, yPosition);
    UpdateWorldCursor(xPosition, yPosition);
}

void TestSceneBase002::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    Project001::ScopeTimer scopeTimer("TestSceneBase002::ProcessUpdateEvent");

    unsigned long long timestep_ns = updateEvent.timestep_ns;

    if (applyGravity_)
    {
        ApplyGravity(timestep_ns);
    }

    CapPositions();
    CapVelocities();

    if (selectedEntityIdIndex_ >= entityIds_.size())
    {
        UpdateCameraPosition(timestep_ns);
    }
    else
    {
        if (velocityBasedMovement_)
        {
            UpdateSelectedEntityVelocity(timestep_ns);
        }
        else
        {
            UpdateSelectedEntityPosition(timestep_ns);
        }
    }

    if (remainingTimeRecordingDuration_ns_ > 0)
    {
        timer01_.Start("CollisionSystem2D::CalculateCollisions");
    }

    float timestep_s = (float)timestep_ns / 1e9f;

    float physicsTimestep_s = timestep_s / (float)physicsStepsPerUpdate_;
    if (useCollisionBodyQuadTree_)
    {
        for (size_t i = 0; i < physicsStepsPerUpdate_; ++i)
        {
            Project001::CollisionSystem2D::ApplyMovement(GetComponentStoresPtr(), physicsTimestep_s);
            Project001::CollisionSystem2D::CalculateCollisionsWithQuadTree(GetComponentStoresPtr());
        }
    }
    else
    {
        for (size_t i = 0; i < physicsStepsPerUpdate_; ++i)
        {
            Project001::CollisionSystem2D::ApplyMovement(GetComponentStoresPtr(), physicsTimestep_s);
            Project001::CollisionSystem2D::CalculateCollisions(GetComponentStoresPtr());
        }
    }

    if (remainingTimeRecordingDuration_ns_ > 0)
    {
        timer01_.Stop();

        if (remainingTimeRecordingDuration_ns_ < timestep_ns)
        {
            remainingTimeRecordingDuration_ns_ = 0;
        }
        else
        {
            remainingTimeRecordingDuration_ns_ -= timestep_ns;
        }

        if (remainingTimeRecordingDuration_ns_ == 0)
        {
            if (Project001::TimeProfiler::EndSession())
            {
                LOG_INFO("RECORDING END");
            }
        }
    }

    UpdateCollisionBodyColors();

    if (generateCursorLineAndDistanceTextMesh_)
    {
        UpdateCursorLineAndDistanceTextMesh();
    }

    if (generateMassTextMesh_)
    {
        UpdateMassTextMesh();
    }

    if (generateMomentOfInertiaTextMesh_)
    {
        UpdateMomentOfInertiaTextMesh();
    }

    if (generateEntityIdTextMesh_)
    {
        UpdateEntityIdTextMesh();
    }

    if (generateEnergyTextMesh_)
    {
        UpdateEnergyTextMesh();
    }

    UpdateCollisionBodyBorderMesh();

    if (generateCollisionBodyQuadTreeMesh_)
    {
        UpdateCollisionBodyQuadTreeMesh();
    }

    if (generateCollisionMarkerCollectionMesh_)
    {
        UpdateCollisionMarkerCollectionMesh();
    }

    if (updateCollisionBodyTexture_)
    {
        UpdateCollisionBodyTexture();
    }
}

void TestSceneBase002::UpdateWorldCursor(float xPosition, float yPosition)
{
    int windowWidth, windowHeight;
    GetWindowPtr()->GetWindowSize(windowWidth, windowHeight);

    unsigned int xOffset, yOffset, viewportWidth, viewportHeight;
    GetRendererPtr()->GetViewport(xOffset, yOffset, viewportWidth, viewportHeight);

    // Convert coordinates from window to viewport
    glm::vec2 viewportCursorPosition(
        xPosition - xOffset,
        windowHeight - yOffset - yPosition
    );

    if (viewportCursorPosition.x < viewportWidth || viewportCursorPosition.y < viewportHeight)
    {
        Project001::Camera* cameraPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraEntityId_));
        Project001::CollisionBody2D* cursorCollisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(cursorCollisionBody2DPtr, cursorEntityId_));
        if (cameraPtr != nullptr && cursorCollisionBody2DPtr != nullptr)
        {
            // Convert coordinates from viewport to world
            glm::vec2 worldCursorPosition = cameraPtr->ConvertPointFromViewportToOrthoWorld(
                viewportWidth,
                viewportHeight,
                viewportCursorPosition
            );

            cursorCollisionBody2DPtr->SetPosition(worldCursorPosition);

            if (cursorGrabbingEntity_ && selectedEntityIdIndex_ < entityIds_.size())
            {
                unsigned int selectedEntityId = entityIds_[selectedEntityIdIndex_];

                Project001::CollisionBody2D* selectedCollisionBody2DPtr = nullptr;
                FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(selectedCollisionBody2DPtr, selectedEntityId));
                if (selectedCollisionBody2DPtr != nullptr)
                {
                    if (!velocityBasedMovement_)
                    {
                        glm::vec2 translation = worldCursorPosition - previousWorldCursorPosition_;
                        selectedCollisionBody2DPtr->AddTranslation(translation);
                    }
                }
            }

            previousWorldCursorPosition_ = worldCursorPosition;
        }
    }
}

void TestSceneBase002::UpdateCameraPosition(unsigned long long timestep_ns)
{
    Project001::Camera* cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraEntityId_));
    if (cameraPtr != nullptr)
    {
        float timestep_s = (float)(timestep_ns / 1000000) / 1000;

        float speedConstant = cameraPtr->GetTopCutoff();
        glm::vec2 cameraUp = cameraPtr->GetUpVector();
        glm::vec2 cameraLeft = cameraPtr->GetLeftVector();

        float translationSpeed = speedConstant * 0.8f * timestep_s;
        float rotationSpeed = 1.2f * timestep_s;

        bool movingLeft = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_A);
        bool movingRight = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_D);
        bool movingUp = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_W);
        bool movingDown = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_S);
        bool rollingLeft = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_Q);
        bool rollingRight = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_E);

        if (movingLeft)
        {
            cameraPtr->MoveLeft(translationSpeed);
        }

        if (movingRight)
        {
            cameraPtr->MoveRight(translationSpeed);
        }

        if (movingUp)
        {
            cameraPtr->MoveUp(translationSpeed);
        }

        if (movingDown)
        {
            cameraPtr->MoveDown(translationSpeed);
        }

        if (rollingLeft)
        {
            cameraPtr->AddRoll(-1.0f * rotationSpeed);
        }

        if (rollingRight)
        {
            cameraPtr->AddRoll(rotationSpeed);
        }

        float xPosition = 0.0f;
        float yPosition = 0.0f;
        GetWindowPtr()->GetCursorPosition(xPosition, yPosition);
        UpdateWorldCursor(xPosition, yPosition);
    }
}

void TestSceneBase002::UpdateSelectedEntityPosition(unsigned long long timestep_ns)
{
    if (selectedEntityIdIndex_ >= entityIds_.size())
    {
        return;
    }

    Project001::Camera* cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraEntityId_));
    if (cameraPtr != nullptr)
    {
        float timestep_s = (float)(timestep_ns / 1000000) / 1000;

        float speedConstant = cameraPtr->GetTopCutoff();
        glm::vec2 cameraUp = cameraPtr->GetUpVector();
        glm::vec2 cameraLeft = cameraPtr->GetLeftVector();

        float translationSpeed = speedConstant * 0.8f * timestep_s;
        float rotationSpeed = 1.2f * timestep_s;

        bool movingLeft = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_A);
        bool movingRight = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_D);
        bool movingUp = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_W);
        bool movingDown = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_S);
        bool rollingLeft = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_Q);
        bool rollingRight = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_E);

        unsigned int selectedEntityId = entityIds_[selectedEntityIdIndex_];

        Project001::CollisionBody2D* collisionBodyPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, selectedEntityId));
        if (collisionBodyPtr != nullptr)
        {
            if (movingLeft)
            {
                // collisionBodyPtr->TranslateRight(-1.0f * translationSpeed);
                collisionBodyPtr->AddTranslation(cameraLeft * translationSpeed);
            }

            if (movingRight)
            {
                // collisionBodyPtr->TranslateRight(translationSpeed);
                collisionBodyPtr->AddTranslation(cameraLeft * -1.0f * translationSpeed);
            }

            if (movingUp)
            {
                // collisionBodyPtr->TranslateUp(translationSpeed);
                collisionBodyPtr->AddTranslation(cameraUp * translationSpeed);
            }

            if (movingDown)
            {
                // collisionBodyPtr->TranslateUp(-1.0f * translationSpeed);
                collisionBodyPtr->AddTranslation(cameraUp * -1.0f * translationSpeed);
            }

            if (rollingLeft)
            {
                collisionBodyPtr->AddRotation(rotationSpeed);
            }

            if (rollingRight)
            {
                collisionBodyPtr->AddRotation(-1.0f * rotationSpeed);
            }
        }
    }
}

void TestSceneBase002::UpdateSelectedEntityVelocity(unsigned long long timestep_ns)
{
    if (selectedEntityIdIndex_ >= entityIds_.size())
    {
        return;
    }

    Project001::Camera* cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraEntityId_));
    if (cameraPtr != nullptr)
    {
        float timestep_s = (float)(timestep_ns / 1000000) / 1000;

        float speedConstant = cameraPtr->GetTopCutoff();
        glm::vec2 cameraUp = cameraPtr->GetUpVector();
        glm::vec2 cameraLeft = cameraPtr->GetLeftVector();

        float accelerationSpeed = 1.6f * timestep_s;
        float angularAccelerationSpeed = 1.6f * timestep_s;

        float decelerationSpeed = accelerationSpeed;
        float angularDecelerationSpeed = angularAccelerationSpeed;

        bool movingLeft = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_A);
        bool movingRight = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_D);
        bool movingUp = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_W);
        bool movingDown = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_S);
        bool rollingLeft = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_Q);
        bool rollingRight = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_E);

        unsigned int selectedEntityId = entityIds_[selectedEntityIdIndex_];

        Project001::CollisionBody2D* collisionBodyPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, selectedEntityId));
        if (collisionBodyPtr != nullptr)
        {
            const glm::vec2& velocity = collisionBodyPtr->GetVelocity();
            float velocityMagnitude = glm::length(velocity);
            glm::vec2 velocityDirection = velocity / velocityMagnitude;

            if (!movingLeft && !movingRight && !movingUp && !movingDown && !cursorGrabbingEntity_)
            {
                if (velocityMagnitude != 0.0f)
                {
                    float decelerationMagnitude = glm::min(velocityMagnitude, decelerationSpeed);

                    glm::vec2 velocityDirection = velocity / velocityMagnitude;

                    collisionBodyPtr->SetVelocity(velocity - velocityDirection * decelerationMagnitude);
                }
            }
            else
            {
                if (cursorGrabbingEntity_)
                {
                    glm::vec2 closestPoint_position;
                    float distanceSquared = collisionBodyPtr->GetDistanceSquaredToPointAndClosestPointToPoint(previousWorldCursorPosition_, closestPoint_position);
                    glm::vec2 grabTranslation = previousWorldCursorPosition_ - closestPoint_position;

                    if (distanceSquared == 0.0f)
                    {
                        if (velocityMagnitude != 0.0f)
                        {
                            float decelerationMagnitude = glm::min(velocityMagnitude, decelerationSpeed);

                            glm::vec2 velocityDirection = velocity / velocityMagnitude;

                            collisionBodyPtr->SetVelocity(velocity - velocityDirection * decelerationMagnitude);
                        }
                    }
                    else
                    {
                        float accelerationMagnitude = glm::length(grabTranslation);

                        if (accelerationMagnitude != 0.0f)
                        {
                            glm::vec2 accelerationDirection = grabTranslation / accelerationMagnitude;

                            float accelerationMultiplier = 1.0f;
                            if (glm::dot(accelerationDirection, velocityDirection) < 0)
                            {
                                accelerationMultiplier = 2.0f;
                            }

                            collisionBodyPtr->SetVelocity(velocity + accelerationDirection * accelerationSpeed * accelerationMultiplier);
                        }
                    }
                }

                if (movingLeft)
                {
                    collisionBodyPtr->SetVelocity(velocity + cameraLeft * accelerationSpeed);
                }

                if (movingRight)
                {
                    collisionBodyPtr->SetVelocity(velocity + cameraLeft * -1.0f * accelerationSpeed);
                }

                if (movingUp)
                {
                    collisionBodyPtr->SetVelocity(velocity + cameraUp * accelerationSpeed);
                }

                if (movingDown)
                {
                    collisionBodyPtr->SetVelocity(velocity + cameraUp * -1.0f * accelerationSpeed);
                }
            }

            const float& angularVelocity = collisionBodyPtr->GetAngularVelocity();

            if (!rollingLeft && !rollingRight)
            {
                float angularVelocityMagnitude = std::abs(angularVelocity);

                float angularDecelerationMagnitude = glm::min(angularVelocityMagnitude, angularDecelerationSpeed);

                if (angularVelocity < 0.0f)
                {
                    collisionBodyPtr->SetAngularVelocity(angularVelocity + angularDecelerationMagnitude);
                }
                else if (angularVelocity > 0.0f)
                {
                    collisionBodyPtr->SetAngularVelocity(angularVelocity - angularDecelerationMagnitude);
                }
            }
            else
            {
                if (rollingLeft)
                {
                    collisionBodyPtr->SetAngularVelocity(angularVelocity + angularAccelerationSpeed);
                }

                if (rollingRight)
                {
                    collisionBodyPtr->SetAngularVelocity(angularVelocity - angularAccelerationSpeed);
                }
            }
        }
    }
}

void TestSceneBase002::ApplyGravity(unsigned long long timestep_ns)
{
    float timestep_s = (float)timestep_ns / 1e9f;

    unsigned int selectedEntityId = (unsigned int)-1;
    if (cursorGrabbingEntity_ && selectedEntityIdIndex_ < entityIds_.size())
    {
        selectedEntityId = entityIds_[selectedEntityIdIndex_];
    }

    Project001::CollisionBody2D* collisionBody2DArray = nullptr;
    size_t collisionBodyCount = 0;

    GetComponentStoresPtr()->GetAllComponents<Project001::CollisionBody2D>(collisionBody2DArray, collisionBodyCount);

    for (unsigned int i = 0; i < collisionBodyCount; ++i)
    {
        Project001::CollisionBody2D& collisionBody2D = collisionBody2DArray[i];

        if (!(collisionBody2D.GetCollisionGroupMask() & mainCollisionGroupMask_))
        {
            continue;
        }

        unsigned int currentEntityId = (unsigned int)-1;
        if (GetComponentStoresPtr()->GetComponentEntityId(currentEntityId, &collisionBody2D) &&
            currentEntityId == selectedEntityId)
        {
            continue;
        }

        if (collisionBody2D.GetFixedTranslation())
        {
            continue;
        }

        collisionBody2D.SetVelocity(collisionBody2D.GetVelocity() + gravity_ * timestep_s);
    }
}

void TestSceneBase002::CapPositions()
{
    Project001::CollisionBody2D* collisionBody2DArray = nullptr;
    size_t collisionBodyCount = 0;

    GetComponentStoresPtr()->GetAllComponents<Project001::CollisionBody2D>(collisionBody2DArray, collisionBodyCount);

    for (unsigned int i = 0; i < collisionBodyCount; ++i)
    {
        Project001::CollisionBody2D& collisionBody2D = collisionBody2DArray[i];

        const uint32_t& collisionGroupMask = collisionBody2D.GetCollisionGroupMask();

        if (!(collisionGroupMask & mainCollisionGroupMask_))
        {
            continue;
        }

        const glm::vec2& collisionBody2D_position = collisionBody2D.GetPosition();

        glm::vec2 positionBorderHalfSize = positionBorderSize_ * 0.5f;

        if (collisionBody2D_position.x > positionBorderHalfSize.x)
        {
            collisionBody2D.SetPositionX(-positionBorderHalfSize.x + std::fmod(collisionBody2D_position.x, positionBorderHalfSize.x));
        }
        else if (collisionBody2D_position.x < -8.0f)
        {
            collisionBody2D.SetPositionX(positionBorderHalfSize.x + std::fmod(collisionBody2D_position.x, positionBorderHalfSize.x));
        }
        // else if (std::isnan(collisionBody2D_position.x))
        // {
        //     collisionBody2D.SetPositionX(0.0f);
        // }

        if (collisionBody2D_position.y > positionBorderHalfSize.y)
        {
            collisionBody2D.SetPositionY(-positionBorderHalfSize.y + std::fmod(collisionBody2D_position.y, positionBorderHalfSize.y));
        }
        else if (collisionBody2D_position.y < -positionBorderHalfSize.y)
        {
            collisionBody2D.SetPositionY(positionBorderHalfSize.y + std::fmod(collisionBody2D_position.y, positionBorderHalfSize.y));
        }
        // else if (std::isnan(collisionBody2D_position.y))
        // {
        //     collisionBody2D.SetPositionY(0.0f);
        // }
    }
}

void TestSceneBase002::CapVelocities()
{
    Project001::CollisionBody2D* collisionBody2DArray = nullptr;
    size_t collisionBodyCount = 0;

    GetComponentStoresPtr()->GetAllComponents<Project001::CollisionBody2D>(collisionBody2DArray, collisionBodyCount);

    for (unsigned int i = 0; i < collisionBodyCount; ++i)
    {
        Project001::CollisionBody2D& collisionBody2D = collisionBody2DArray[i];

        const uint32_t& collisionGroupMask = collisionBody2D.GetCollisionGroupMask();

        if (!(collisionGroupMask & mainCollisionGroupMask_))
        {
            continue;
        }

        const glm::vec2& collisionBody2D_velocity = collisionBody2D.GetVelocity();

        float velocityMagnitude = glm::length(collisionBody2D_velocity);

        if (velocityMagnitude > maxVelocity_)
        {
            glm::vec2 newVelocity = glm::normalize(collisionBody2D_velocity) * maxVelocity_;
            collisionBody2D.SetVelocity(newVelocity);
        }

        const float& collisionBody2D_angularVelocity = collisionBody2D.GetAngularVelocity();

        if (collisionBody2D_angularVelocity > maxAngularVelocity_)
        {
            collisionBody2D.SetAngularVelocity(maxAngularVelocity_);
        }
        else if (collisionBody2D_angularVelocity < -maxAngularVelocity_)
        {
            collisionBody2D.SetAngularVelocity(-maxAngularVelocity_);
        }
    }
}

void TestSceneBase002::Sync_RenderedMesh_CollisionBody_Components()
{
    Project001::CollisionBody2D* collisionBody2DArray = nullptr;
    size_t collisionBodyCount = 0;

    GetComponentStoresPtr()->GetAllComponents<Project001::CollisionBody2D>(collisionBody2DArray, collisionBodyCount);

    for (unsigned int i = 0; i < collisionBodyCount; ++i)
    {
        Project001::CollisionBody2D& collisionBody2D = collisionBody2DArray[i];

        unsigned int entityId;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponentEntityId(entityId, &collisionBody2D));

        Project001::RenderedMesh* renderedMeshPtr;
        if (GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, entityId))
        {
            const float& positionX = collisionBody2D.GetPosition().x;
            const float& positionY = collisionBody2D.GetPosition().y;
            renderedMeshPtr->SetPosition(positionX, positionY, 0.0f);
            const float& rotation = collisionBody2D.GetRotation();
            renderedMeshPtr->ResetOrientation();
            renderedMeshPtr->AddWorldRotationZ(rotation);
        }
    }
}

void TestSceneBase002::UpdateCollisionBodyColors()
{
    Project001::CollisionBody2D* collisionBodyPtrs = nullptr;
    size_t collisionBodyCount = 0;
    GetComponentStoresPtr()->GetAllComponents<Project001::CollisionBody2D>(collisionBodyPtrs, collisionBodyCount);
    for (size_t i = 0; i < collisionBodyCount; ++i)
    {
        bool collisionBodyColliding = false;

        Project001::CollisionBody2D& currentCollisionBody = collisionBodyPtrs[i];
        const std::vector<Project001::CollisionData2D>& currentCollisions = currentCollisionBody.GetCollisions();
        for (size_t j = 0; j < currentCollisions.size(); ++j)
        {
            const Project001::CollisionData2D& currentCollision = currentCollisions[j];
            if (currentCollision.otherShapeTag == 0)
            {
                collisionBodyColliding = true;
                break;
            }
        }

        unsigned int collidingEntityId;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponentEntityId(collidingEntityId , &currentCollisionBody));

        // Don't recolor the cursor
        if (collidingEntityId == cursorEntityId_)
        {
            continue;
        }

        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        bool renderedMeshFound = GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, collidingEntityId);

        if (renderedMeshFound)
        {
            if (recolorOverlappingCollisionBodies_ && collisionBodyColliding )
            {
                if (selectedEntityIdIndex_ < entityIds_.size() && collidingEntityId == entityIds_[selectedEntityIdIndex_])
                {
                    renderedMeshPtr->SetColor(0.75f, 0.5f, 0.25f, 0.5f);
                }
                else
                {
                    renderedMeshPtr->SetColor(0.75f, 0.25f, 0.25f, 0.5f);
                }
            }
            else
            {
                if (selectedEntityIdIndex_ < entityIds_.size() && collidingEntityId == entityIds_[selectedEntityIdIndex_])
                {
                    renderedMeshPtr->SetColor(0.25f, 0.25f, 1.0f, 1.0f);
                }
                else
                {
                    const Project001::CollisionBody2D::PhysicsType& physicsType = currentCollisionBody.GetPhysicsType();

                    if (physicsType == Project001::CollisionBody2D::PhysicsType::PHYSICS_TYPE_OVERLAP_ONLY ||
                        physicsType == Project001::CollisionBody2D::PhysicsType::PHYSICS_TYPE_DETAILED_OVERLAP_ONLY)
                    {
                        renderedMeshPtr->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
                    }
                    else if (physicsType == Project001::CollisionBody2D::PhysicsType::PHYSICS_TYPE_REGULAR_PHYSICS)
                    {
                        const bool& fixedTranslation = currentCollisionBody.GetFixedTranslation();
                        const bool& fixedRotation = currentCollisionBody.GetFixedRotation();
                        if (fixedTranslation)
                        {
                            if (fixedRotation)
                            {
                                renderedMeshPtr->SetColor(0.9f, 0.6f, 0.9f, 1.0f);
                            }
                            else
                            {
                                renderedMeshPtr->SetColor(0.6f, 0.6f, 0.9f, 1.0f);
                            }
                        }
                        else if (fixedRotation)
                        {
                            renderedMeshPtr->SetColor(0.9f, 0.6f, 0.6f, 1.0f);
                        }
                        else
                        {
                            renderedMeshPtr->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
                        }
                    }
                }
            }
        }
    }
}

void TestSceneBase002::UpdateCursorLineAndDistanceTextMesh()
{
    cursorLineMeshDataPtr_->Clear();
    distanceTextMeshDataPtr_->Clear();

    if (selectedEntityIdIndex_ < entityIds_.size())
    {
        const unsigned int& selectedEntityId = entityIds_[selectedEntityIdIndex_];
        Project001::CollisionBody2D* collisionBodyPtr = nullptr;
        bool collisionBodyFound = GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, selectedEntityId);
        if (collisionBodyFound)
        {
            float distanceSquared = collisionBodyPtr->GetDistanceSquaredToPoint(previousWorldCursorPosition_);
            if (distanceSquared != std::numeric_limits<float>::infinity())
            {
                glm::vec2 closestPoint_position;
                float distanceSquared_unused = collisionBodyPtr->GetDistanceSquaredToPointAndClosestPointToPoint(previousWorldCursorPosition_, closestPoint_position);
                cursorLinePositions_.clear();
                cursorLinePositions_.emplace_back();
                cursorLinePositions_.emplace_back(closestPoint_position - previousWorldCursorPosition_);

                Project001::MeshLoader::Generate2DLine(*cursorLineMeshDataPtr_, cursorLinePositions_, 0.01f); // No Fail Check

                FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
                    *distanceTextMeshDataPtr_,
                    *font01_FontDataPtr_,
                    "distance: " + std::to_string(glm::sqrt(distanceSquared)),
                    fontPixelSize_
                ));
            }
        }
    }
}

void TestSceneBase002::UpdateMassTextMesh()
{
    massTextMeshDataPtr_->Clear();

    if (selectedEntityIdIndex_ < entityIds_.size())
    {
        const unsigned int& selectedEntityId = entityIds_[selectedEntityIdIndex_];
        Project001::CollisionBody2D* collisionBodyPtr = nullptr;
        bool collisionBodyFound = GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, selectedEntityId);
        if (collisionBodyFound)
        {
            const float& mass = collisionBodyPtr->GetMass();

            FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
                *massTextMeshDataPtr_,
                *font01_FontDataPtr_,
                "mass: " + std::to_string(mass),
                fontPixelSize_
            ));
        }
    }
}

void TestSceneBase002::UpdateMomentOfInertiaTextMesh()
{
    momentOfInertiaTextMeshDataPtr_->Clear();

    if (selectedEntityIdIndex_ < entityIds_.size())
    {
        const unsigned int& selectedEntityId = entityIds_[selectedEntityIdIndex_];
        Project001::CollisionBody2D* collisionBodyPtr = nullptr;
        bool collisionBodyFound = GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, selectedEntityId);
        if (collisionBodyFound)
        {
            const float& momentOfInertia = collisionBodyPtr->GetMomentOfInertia();

            FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
                *momentOfInertiaTextMeshDataPtr_,
                *font01_FontDataPtr_,
                "momentOfInertia: " + std::to_string(momentOfInertia),
                fontPixelSize_
            ));
        }
    }
}

void TestSceneBase002::UpdateEntityIdTextMesh()
{
    entityIdTextMeshDataPtr_->Clear();

    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursorEntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        std::string entityIdTextString = "entityId:";

        const std::vector<Project001::CollisionData2D>& currentCollisions = collisionBody2DPtr->GetCollisions();
        for (size_t j = 0; j < currentCollisions.size(); ++j)
        {
            const Project001::CollisionData2D& currentCollision = currentCollisions[j];
            if (currentCollision.otherShapeTag == 0)
            {
                entityIdTextString += ' ' + std::to_string(currentCollision.otherEntityId);
            }
        }

        FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
            *entityIdTextMeshDataPtr_,
            *font01_FontDataPtr_,
            entityIdTextString,
            fontPixelSize_
        ));
    }
}

void TestSceneBase002::UpdateFpsTextMesh(unsigned long long timestep_ns)
{
    float fps = 1000000000.0f / (float)timestep_ns;
    std::string fps_string = std::to_string(fps);
    if (fps_string.length() > 5)
    {
        fps_string.resize(5);
    }
    fps_string = "fps: " + fps_string;
    fps_MeshDataPtr_->Clear();
    FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(*fps_MeshDataPtr_, *font01_FontDataPtr_, fps_string, fontPixelSize_));
    // Project001::MeshLoader::RecenterMesh(*fps_MeshDataPtr_);
}

void TestSceneBase002::UpdateEnergyTextMesh()
{
    energy_MeshDataPtr_->Clear();

    float totalLinearKeneticEnergy = 0.0f;
    float totalRotationalKeneticEnergy = 0.0f;

    Project001::CollisionBody2D* collisionBodyPtrs = nullptr;
    size_t collisionBodyCount = 0;
    bool componentFound = GetComponentStoresPtr()->GetAllComponents<Project001::CollisionBody2D>(collisionBodyPtrs, collisionBodyCount);

    for (size_t i = 0; i < collisionBodyCount; ++i)
    {
        Project001::CollisionBody2D& currentCollisionBody = collisionBodyPtrs[i];

        if (!std::isinf(currentCollisionBody.GetMass()))
        {
            totalLinearKeneticEnergy += currentCollisionBody.GetLinearKeneticEnergy();
        }

        if (!std::isinf(currentCollisionBody.GetMomentOfInertia()))
        {
            totalRotationalKeneticEnergy += currentCollisionBody.GetRotationalKeneticEnergy();
        }
    }

    std::string linearKeneticEnergy_string = std::to_string(totalLinearKeneticEnergy);
    if (linearKeneticEnergy_string.length() > 5)
    {
        linearKeneticEnergy_string.resize(5);
    }
    linearKeneticEnergy_string = "LKE: " + linearKeneticEnergy_string;

    std::string rotationalKeneticEnergy_string = std::to_string(totalRotationalKeneticEnergy);
    if (rotationalKeneticEnergy_string.length() > 5)
    {
        rotationalKeneticEnergy_string.resize(5);
    }
    rotationalKeneticEnergy_string = "RKE: " + rotationalKeneticEnergy_string;

    std::string totalKeneticEnergy_string = std::to_string(totalLinearKeneticEnergy + totalRotationalKeneticEnergy);
    if (totalKeneticEnergy_string.length() > 5)
    {
        totalKeneticEnergy_string.resize(5);
    }
    totalKeneticEnergy_string = "TKE: " + totalKeneticEnergy_string;

    std::string energy_string = linearKeneticEnergy_string + " + " + rotationalKeneticEnergy_string + " = " + totalKeneticEnergy_string;

    FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(*energy_MeshDataPtr_, *font01_FontDataPtr_, energy_string, fontPixelSize_));
    // Project001::MeshLoader::RecenterMesh(*energy_MeshDataPtr_);
    // Project001::MeshLoader::TranslateMesh(*energy_MeshDataPtr_, -0.5f * energy_MeshDataPtr_->GetSize());
}

void TestSceneBase002::UpdateCollisionBodyBorderMesh()
{
    const float lineWidth = 0.04f;

    collisionBodyBorderMeshDataPtr_->Clear();

    Project001::MeshLoader::Generate2DRectangleFrame(*collisionBodyBorderMeshDataPtr_, -0.5f * positionBorderSize_, 0.5f * positionBorderSize_, lineWidth);
}

void TestSceneBase002::UpdateCollisionBodyQuadTreeMesh()
{
    const Project001::CollisionBodyQuadTreeNode2D* rootNodePtr = Project001::CollisionSystem2D::GetCollisionBodyQuadTree2D().GetRootNode();

    const float lineWidth = 0.04f;

    collisionBodyQuadTreeMeshDataPtr_->Clear();

    if (useCollisionBodyQuadTree_ && (!rootNodePtr->leafNode || !rootNodePtr->bodyPtrs.empty()))
    {
        Project001::MeshLoader::Generate2DRectangleFrame(*collisionBodyQuadTreeMeshDataPtr_, rootNodePtr->min, rootNodePtr->max, lineWidth);

        std::stack<const Project001::CollisionBodyQuadTreeNode2D*> nodePtrStack;
        nodePtrStack.push(rootNodePtr);

        while (!nodePtrStack.empty())
        {
            const Project001::CollisionBodyQuadTreeNode2D* nodePtr = nodePtrStack.top();
            nodePtrStack.pop();

            if (!nodePtr->leafNode)
            {
                const glm::vec2& min = nodePtr->min;
                const glm::vec2& max = nodePtr->max;
                glm::vec2 mid((nodePtr->max + nodePtr->min) * 0.5f);
                glm::vec2 top(mid.x, max.y);
                glm::vec2 bottom(mid.x, min.y);
                glm::vec2 left(min.x, mid.y);
                glm::vec2 right(max.x, mid.y);

                Project001::MeshLoader::Generate2DLine(*collisionBodyQuadTreeMeshDataPtr_, top, bottom, lineWidth);
                Project001::MeshLoader::Generate2DLine(*collisionBodyQuadTreeMeshDataPtr_, left, right, lineWidth);

                for (size_t i = 0; i < 4; ++i)
                {
                    if (nodePtr->childrenPtrs[i])
                    {
                        nodePtrStack.push(nodePtr->childrenPtrs[i]);
                    }
                }
            }
        }
    }
}

void TestSceneBase002::UpdateCollisionMarkerCollectionMesh()
{
    collisionMarkerCollectionMeshDataPtr_->Clear();

    std::vector<glm::vec2> positions;

    Project001::CollisionBody2D* collisionBodyPtrs = nullptr;
    size_t collisionBodyCount = 0;
    GetComponentStoresPtr()->GetAllComponents<Project001::CollisionBody2D>(collisionBodyPtrs, collisionBodyCount);
    for (size_t i = 0; i < collisionBodyCount; ++i)
    {
        Project001::CollisionBody2D& currentCollisionBody = collisionBodyPtrs[i];
        unsigned int currentEntityId;
        GetComponentStoresPtr()->GetComponentEntityId<Project001::CollisionBody2D>(currentEntityId, &currentCollisionBody);
        const std::vector<Project001::CollisionData2D>& currentCollisions = currentCollisionBody.GetCollisions();
        for (size_t j = 0; j < currentCollisions.size(); ++j)
        {
            const Project001::CollisionData2D& currentCollision = currentCollisions[j];

            if (currentEntityId < currentCollision.otherEntityId)
            {
                if (!std::isnan(currentCollision.point.x) && !std::isnan(currentCollision.point.y))
                {
                    positions.emplace_back(currentCollision.point + glm::vec2(-0.02f, 0.0f));
                    positions.emplace_back(currentCollision.point + glm::vec2(0.0f, -0.02f));
                    positions.emplace_back(currentCollision.point + glm::vec2(0.02f, 0.0f));
                    positions.emplace_back(currentCollision.point + glm::vec2(0.0f, 0.02f));
                    Project001::MeshLoader::Generate2DTriangleFan(*collisionMarkerCollectionMeshDataPtr_, positions);
                    positions.clear();

                    if (!std::isnan(currentCollision.normal.x) && !std::isnan(currentCollision.normal.y) && !std::isnan(currentCollision.depth))
                    {
                        Project001::MeshLoader::Generate2DLine(*collisionMarkerCollectionMeshDataPtr_, currentCollision.point, currentCollision.point + currentCollision.normal * currentCollision.depth, 0.01f);
                    }
                }
            }
        }
    }
}

void TestSceneBase002::UpdateCollisionBodyTexture()
{
    Project001::CollisionBody2D* collisionBodyPtrs = nullptr;
    size_t collisionBodyCount = 0;
    GetComponentStoresPtr()->GetAllComponents<Project001::CollisionBody2D>(collisionBodyPtrs, collisionBodyCount);
    for (size_t i = 0; i < collisionBodyCount; ++i)
    {
        Project001::CollisionBody2D& currentCollisionBody = collisionBodyPtrs[i];
        const uint32_t& collisionGroupMask = currentCollisionBody.GetCollisionGroupMask();
        if (!(collisionGroupMask & mainCollisionGroupMask_))
        {
            continue;
        }

        unsigned int currentEntityId;
        GetComponentStoresPtr()->GetComponentEntityId<Project001::CollisionBody2D>(currentEntityId, &currentCollisionBody);

        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, currentEntityId));
        if (renderedMeshPtr != nullptr)
        {
            const Project001::CollisionBody2D::PhysicsType& physicsType = currentCollisionBody.GetPhysicsType();

            if (physicsType == Project001::CollisionBody2D::PhysicsType::PHYSICS_TYPE_OVERLAP_ONLY ||
                physicsType == Project001::CollisionBody2D::PhysicsType::PHYSICS_TYPE_DETAILED_OVERLAP_ONLY)
            {
                renderedMeshPtr->SetTextureId(text_overlapOnly_TextureId_);
            }
            else if (physicsType == Project001::CollisionBody2D::PhysicsType::PHYSICS_TYPE_REGULAR_PHYSICS)
            {
                const bool& fixedTranslation = currentCollisionBody.GetFixedTranslation();
                const bool& fixedRotation = currentCollisionBody.GetFixedRotation();
                if (fixedTranslation)
                {
                    if (fixedRotation)
                    {
                        renderedMeshPtr->SetTextureId(text_static_TextureId_);
                    }
                    else
                    {
                        renderedMeshPtr->SetTextureId(text_rotationOnly_TextureId_);
                    }
                }
                else if (fixedRotation)
                {
                    renderedMeshPtr->SetTextureId(text_translationOnly_TextureId_);
                }
                else
                {
                    renderedMeshPtr->SetTextureId(text_dynamic_TextureId_);
                }
            }
        }
    }
}