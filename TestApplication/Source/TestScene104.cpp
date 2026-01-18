// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-01-17

#include "TestScene104.h"

#include "TestApplicationData.h"
#include "TestResource_AntonioRegular_png.h"
#include "TestResource_AntonioRegular_ssf.h"

#include "Components/Camera.h"
#include "Components/CollisionBody2D.h"
#include "Components/RenderedModel.h"
#include "Resources/PixelFont5x6.h"
#include "Utilities/FontUtility.h"
#include "Utilities/MathUtility.h"
#include "Utilities/MeshUtility.h"
#include "Utilities/TextureUtility.h"
#include "ComponentStores.h"
#include "Logger.h"
#include "RenderSystem.h"
#include "Window.h"



struct CursorInfo
{
    enum class State
    {
        STATE_OPEN,
        STATE_POINTER,
        STATE_GRAB
    };

    State state = State::STATE_OPEN;
};

// public ----------------------------------------------------------------------

TestScene104::TestScene104(Project001::Application* applicationPtr)
    : Scene(applicationPtr)
    , instructionScene_(applicationPtr)
{
    GetSharedDataPtr<TestApplicationData>()->testScene104Id = GetId();
}

TestScene104::~TestScene104()
{}

void TestScene104::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene104::ProcessInitializeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene104::ProcessDeinitializeEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::CursorPositionEvent>(event, std::bind(&TestScene104::ProcessCursorPositionEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene104::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::MouseButtonEvent>(event, std::bind(&TestScene104::ProcessMouseButtonEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestScene104::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene104::ProcessUpdateEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene104::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   TestScene104:            " << GetId());

    // -------------------------------------------------------------------------

    GetWindowPtr()->SetCursorVisible(false);

    LoadPixelFontResources();
    LoadCursorResources();

    CreateMainCameraEntity();
    CreateCursorEntity(cursor_EntityId_);

    InitializeInstructionScene();
}

void TestScene104::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    LOG_INFO("DEINITIALIZING: TestScene104:            " << GetId());

    // -------------------------------------------------------------------------

    GetWindowPtr()->SetCursorVisible(true);

    GetRendererPtr()->DeleteAllTextures();
    GetRendererPtr()->DeleteAllMeshes();
    GetComponentStoresPtr()->DeleteAllEntities();

    // Resources ---------------------------------------------------------------

    pixelFont_FontDataPtr_ = nullptr;
    pixelFont_TextureDataPtr_ = nullptr;
    pixelFont_TextureId_ = static_cast<unsigned int>(-1);

    delete cursorHandOpen_MeshDataPtr_;
    cursorHandOpen_MeshDataPtr_ = nullptr;
    delete cursorHandPointer_MeshDataPtr_;
    cursorHandPointer_MeshDataPtr_ = nullptr;
    delete cursorHandGrab_MeshDataPtr_;
    cursorHandGrab_MeshDataPtr_ = nullptr;
    delete cursor_TextureDataPtr_;
    cursor_TextureDataPtr_ = nullptr;
    cursor_TextureId_ = static_cast<unsigned int>(-1);
    delete cursorCollision_MeshDataPtr_;
    cursorCollision_MeshDataPtr_ = nullptr;

    // Entities ----------------------------------------------------------------

    mainCamera_EntityId_ = static_cast<unsigned int>(-1);

    cursor_EntityId_ = static_cast<unsigned int>(-1);
}

void TestScene104::ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorPositionEvent)
{
    UpdateCursorPositionUsingWindowCoordinates(cursor_EntityId_, cursorPositionEvent.xPosition, cursorPositionEvent.yPosition);

    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_EntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
        collisionPoints[s_cursorPosition_CollisionPointIndex_].enabled = true;
    }
}

void TestScene104::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
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
    }
}

void TestScene104::ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent)
{
    Project001::MouseButton& mouseButton = mouseButtonEvent.mouseButton;
    Project001::ButtonAction& buttonAction = mouseButtonEvent.buttonAction;

    if (mouseButton == Project001::MouseButton::MOUSE_BUTTON_1)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS)
        {
            Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_EntityId_));
            if (collisionBody2DPtr != nullptr)
            {
                std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
                collisionPoints[s_cursorPress_CollisionPointIndex_].position = collisionPoints[s_cursorPosition_CollisionPointIndex_].position;
                collisionPoints[s_cursorPress_CollisionPointIndex_].enabled = true;
                collisionPoints[s_cursorRelease_CollisionPointIndex_].enabled = false;
            }
        }
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
        {
            Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_EntityId_));
            if (collisionBody2DPtr != nullptr)
            {
                std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
                collisionPoints[s_cursorRelease_CollisionPointIndex_].position = collisionPoints[s_cursorPosition_CollisionPointIndex_].position;
                collisionPoints[s_cursorRelease_CollisionPointIndex_].enabled = true;
            }
        }
    }
}

void TestScene104::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    GetRenderSystemPtr()->Render();
}

void TestScene104::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    UpdateCursor(cursor_EntityId_);

    SyncCursorRenderedModels();
}

void TestScene104::InitializeInstructionScene()
{
    Project001::FontData font01_FontData;
    FAIL_CHECK(Project001::Font::LoadFontDataFromMemory(
        font01_FontData,
        g_AntonioRegular_ssf,
        sizeof(g_AntonioRegular_ssf)
    ));

    Project001::TextureData font01_TextureData;
    FAIL_CHECK(Project001::Texture::LoadTextureFromMemory(
        font01_TextureData,
        g_AntonioRegular_png,
        sizeof(g_AntonioRegular_png)
    ));
    unsigned int font01_TextureId = static_cast<unsigned int>(-1);
    GetRendererPtr()->CreateTexture(
        font01_TextureId,
        font01_TextureData.data,
        font01_TextureData.width,
        font01_TextureData.height,
        font01_TextureData.bytesPerPixel,
        true,
        false
    );

    const uint32_t uiCamera_Mask = 0b01000000000000000000000000000000;

    unsigned int uiCamera_EntityId = static_cast<unsigned int>(-1);
    GetComponentStoresPtr()->CreateEntity(uiCamera_EntityId);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::Camera>(uiCamera_EntityId));
    Project001::Camera* cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, uiCamera_EntityId));
    if (cameraPtr != nullptr)
    {
        int aspectRatioNumerator;
        int aspectRatioDenominator;
        GetWindowPtr()->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);
        if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
        {
            float aspectRatio = static_cast<float>(aspectRatioNumerator) / static_cast<float>(aspectRatioDenominator);
            float uiCameraHalfHeight = 3.5f;
            float uiCameraHalfWidth = aspectRatio * uiCameraHalfHeight;
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
        cameraPtr->SetCameraMask(uiCamera_Mask);
        cameraPtr->SetPriorityValue(1000000);
    }

    const Project001::KeyCode keyCode_toggleInstructions = Project001::KeyCode::KEY_CODE_TAB;

    TestInstructionScene001::InitializationInfo instructionSceneInfo;
    instructionSceneInfo.hiddenInstructionString = std::string("Press <Tab> to show instructions.");
    instructionSceneInfo.instructionString = std::string(
        "TODO:\n"
        "Press <Esc> to return to Main Menu.\n"
        "Press <Tab> to hide instructions."
    );
    instructionSceneInfo.fontDataPtr = &font01_FontData;
    instructionSceneInfo.fontTextureId = font01_TextureId;
    instructionSceneInfo.cameraEntityId = uiCamera_EntityId;
    instructionSceneInfo.cameraMask = uiCamera_Mask;
    instructionSceneInfo.keyCode_toggleInstructions = keyCode_toggleInstructions;
    instructionScene_.Initialize(instructionSceneInfo);
}

void TestScene104::LoadPixelFontResources()
{
    pixelFont_FontDataPtr_ = &Project001::Get_PixelFont5x6_FontData();
    pixelFont_TextureDataPtr_ = &Project001::Get_PixelFont5x6_TextureData();
    GetRendererPtr()->CreateTexture(
        pixelFont_TextureId_,
        pixelFont_TextureDataPtr_->data,
        pixelFont_TextureDataPtr_->width,
        pixelFont_TextureDataPtr_->height,
        pixelFont_TextureDataPtr_->bytesPerPixel,
        false,
        false
    );
}

void TestScene104::LoadCursorResources()
{
    cursorHandOpen_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::Generate2DSprite(
        *cursorHandOpen_MeshDataPtr_, 24.0f, 32.0f, 0.0f, 1.0f / 3.0f, 0.0f, 1.0f
    ));

    cursorHandPointer_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::Generate2DSprite(
        *cursorHandPointer_MeshDataPtr_, 24.0f, 32.0f, 1.0f / 3.0f, 2.0f / 3.0f, 0.0f, 1.0f
    ));

    cursorHandGrab_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::Generate2DSprite(
        *cursorHandGrab_MeshDataPtr_, 24.0f, 32.0f, 2.0f / 3.0f, 1.0f, 0.0f, 1.0f
    ));

    cursor_TextureDataPtr_ = new Project001::TextureData();
    FAIL_CHECK(Project001::Texture::LoadTexture(
        *cursor_TextureDataPtr_, "../Textures/handCursor_01.png"
    ));

    GetRendererPtr()->CreateTexture(
        cursor_TextureId_,
        cursor_TextureDataPtr_->data,
        cursor_TextureDataPtr_->width,
        cursor_TextureDataPtr_->height,
        cursor_TextureDataPtr_->bytesPerPixel,
        false, false
    );

    cursorCollision_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::Generate2DArc(
        *cursorCollision_MeshDataPtr_, 2.0f, 4.0f, 8, 0.0f, 0.0f
    ));
}

void TestScene104::CreateMainCameraEntity()
{
    GetComponentStoresPtr()->CreateEntity(mainCamera_EntityId_);
    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::Camera>(mainCamera_EntityId_));

    Project001::Camera* cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCamera_EntityId_));
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
        cameraPtr->SetCameraMask(s_mainCamera_Mask_);
    }
}

void TestScene104::CreateCursorEntity(unsigned int& entityId)
{
    GetComponentStoresPtr()->CreateEntity(entityId);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(entityId));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, entityId));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCamera_Mask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
        renderedMeshes.resize(s_cursor_RenderedMeshIndices_);

        {
            Project001::RenderedMesh& mesh = renderedMeshes[s_cursorPosition_RenderedMeshIndex_];
            mesh.SetCameraMask(s_mainCamera_Mask_);
            mesh.SetMeshDataPtr(cursorCollision_MeshDataPtr_);
            mesh.SetPositionZ(0.53f);
            mesh.SetColor(0.2f, 0.8f, 0.8f, 0.4f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[s_cursorPress_RenderedMeshIndex_];
            mesh.SetCameraMask(s_mainCamera_Mask_);
            mesh.SetMeshDataPtr(cursorCollision_MeshDataPtr_);
            mesh.SetPositionZ(0.52f);
            mesh.SetColor(0.8f, 0.2f, 0.8f, 0.4f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetVisible(false);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[s_cursorRelease_RenderedMeshIndex_];
            mesh.SetCameraMask(s_mainCamera_Mask_);
            mesh.SetMeshDataPtr(cursorCollision_MeshDataPtr_);
            mesh.SetPositionZ(0.51f);
            mesh.SetColor(0.8f, 0.8f, 0.2f, 0.4f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetVisible(false);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[s_cursorHandOpen_RenderedMeshIndex_];
            mesh.SetCameraMask(s_mainCamera_Mask_);
            mesh.SetMeshDataPtr(cursorHandOpen_MeshDataPtr_);
            mesh.SetTextureId(cursor_TextureId_);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetVisible(false);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[s_cursorHandGrab_RenderedMeshIndex_];
            mesh.SetCameraMask(s_mainCamera_Mask_);
            mesh.SetMeshDataPtr(cursorHandGrab_MeshDataPtr_);
            mesh.SetTextureId(cursor_TextureId_);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetVisible(false);
        }
    }

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(entityId));
    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, entityId));
    if (collisionBody2DPtr != nullptr)
    {
        collisionBody2DPtr->SetPhysicsType(Project001::CollisionShape2D::PhysicsType::PHYSICS_TYPE_SENSOR);

        std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
        collisionPoints.resize(s_cursor_CollisionPointIndices_);

        collisionPoints[s_cursorPosition_CollisionPointIndex_].tag = s_cursorPosition_CollisionShapeId_;

        collisionPoints[s_cursorPress_CollisionPointIndex_].tag = s_cursorPress_CollisionShapeId_;
        collisionPoints[s_cursorPress_CollisionPointIndex_].enabled = false;

        collisionPoints[s_cursorRelease_CollisionPointIndex_].tag = s_cursorRelease_CollisionShapeId_;
        collisionPoints[s_cursorRelease_CollisionPointIndex_].enabled = false;
    }

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<CursorInfo>(entityId));
}

void TestScene104::UpdateCursorPositionUsingWindowCoordinates(unsigned int entityId, float xPosition, float yPosition)
{
    int windowWidth, windowHeight;
    GetWindowPtr()->GetWindowSize(windowWidth, windowHeight);

    glm::vec2 viewportNormalizedCursorPosition =
        GetRendererPtr()->ConvertPointFromWindowToViewportNormalized(glm::vec2(xPosition, yPosition), static_cast<float>(windowHeight));

    Project001::Camera* cameraPtr;
    if (GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCamera_EntityId_))
    {
        glm::vec3 worldCursorPosition;
        glm::vec3 worldCursorNormal;
        bool cursorRayIntersected = cameraPtr->RaycastPointFromNormalizedViewportToPane(
            viewportNormalizedCursorPosition,
            glm::vec3(0.0f, 0.0f, 1.0f),
            0.0f,
            worldCursorPosition,
            worldCursorNormal);

        if (cursorRayIntersected)
        {
            Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, entityId));
            if (collisionBody2DPtr != nullptr)
            {
                std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();

                collisionPoints[s_cursorPosition_CollisionPointIndex_].position = worldCursorPosition;
            }
        }
    }
}

void TestScene104::UpdateCursor(unsigned int entityId)
{
    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<CursorInfo>(entityId));
    CursorInfo* cursorInfoPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<CursorInfo>(cursorInfoPtr, entityId));
    if (cursorInfoPtr != nullptr)
    {
        const bool mouseLeftPressed = GetWindowPtr()->GetMouseButtonPressed(Project001::MouseButton::MOUSE_BUTTON_LEFT);

        if (mouseLeftPressed)
        {
            cursorInfoPtr->state = CursorInfo::State::STATE_GRAB;
        }
        else
        {
            cursorInfoPtr->state = CursorInfo::State::STATE_OPEN;
        }
    }
}

void TestScene104::SyncCursorRenderedModels()
{
    CursorInfo* cursorInfoPtrs = nullptr;
    size_t cursorInfoCount = 0;
    GetComponentStoresPtr()->GetAllComponents<CursorInfo>(cursorInfoPtrs, cursorInfoCount);
    for (size_t i = 0; i < cursorInfoCount; ++i)
    {
        CursorInfo& cursorInfo = cursorInfoPtrs[i];

        unsigned int entityId = static_cast<unsigned int>(-1);
        GetComponentStoresPtr()->GetComponentEntityId<CursorInfo>(entityId, &cursorInfo);

        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, entityId));
        if (collisionBody2DPtr != nullptr)
        {
            std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
            Project001::CollisionPoint2D collisionPoint01 = collisionPoints[s_cursorPosition_CollisionPointIndex_];
            Project001::CollisionPoint2D collisionPoint02 = collisionPoints[s_cursorPress_CollisionPointIndex_];
            Project001::CollisionPoint2D collisionPoint03 = collisionPoints[s_cursorRelease_CollisionPointIndex_];

            Project001::RenderedModel* renderedModelPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, entityId));
            if (renderedModelPtr != nullptr)
            {
                std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

                Project001::RenderedMesh& circleMesh01 = renderedMeshes[s_cursorPosition_RenderedMeshIndex_];
                Project001::RenderedMesh& circleMesh02 = renderedMeshes[s_cursorPress_RenderedMeshIndex_];
                Project001::RenderedMesh& circleMesh03 = renderedMeshes[s_cursorRelease_RenderedMeshIndex_];
                Project001::RenderedMesh& handOpenMesh = renderedMeshes[s_cursorHandOpen_RenderedMeshIndex_];
                Project001::RenderedMesh& handGrabMesh = renderedMeshes[s_cursorHandGrab_RenderedMeshIndex_];

                circleMesh01.SetPositionX(collisionPoint01.position.x);
                circleMesh01.SetPositionY(collisionPoint01.position.y);
                circleMesh01.SetVisible(collisionPoint01.enabled);

                circleMesh02.SetPositionX(collisionPoint02.position.x);
                circleMesh02.SetPositionY(collisionPoint02.position.y);
                circleMesh02.SetVisible(collisionPoint02.enabled);

                circleMesh03.SetPositionX(collisionPoint03.position.x);
                circleMesh03.SetPositionY(collisionPoint03.position.y);
                circleMesh03.SetVisible(collisionPoint03.enabled);

                handOpenMesh.SetPositionX(collisionPoint01.position.x);
                handOpenMesh.SetPositionY(collisionPoint01.position.y);

                handGrabMesh.SetPositionX(collisionPoint01.position.x);
                handGrabMesh.SetPositionY(collisionPoint01.position.y);

                if (cursorInfo.state == CursorInfo::State::STATE_OPEN)
                {
                    handOpenMesh.SetVisible(true);
                    handGrabMesh.SetVisible(false);
                }
                else if (cursorInfo.state == CursorInfo::State::STATE_GRAB)
                {
                    handOpenMesh.SetVisible(false);
                    handGrabMesh.SetVisible(true);
                }
            }
        }
    }
}