// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-05-04

#include "TestScene052.h"

#include "TestApplicationData.h"
#include "TestResource_AntonioRegular_png.h"
#include "TestResource_AntonioRegular_ssf.h"

#include "Components/Camera.h"
#include "Components/CollisionBody2D.h"
#include "Components/RenderedModel.h"
#include "CollisionSystem2D.h"
#include "ComponentStores.h"
#include "FontLoader.h"
#include "Logger.h"
#include "MeshLoader.h"
#include "RenderSystem.h"
#include "SoundLoader.h"
#include "SoundPlayer.h"
#include "TextureLoader.h"
#include "Window.h"



// public ----------------------------------------------------------------------

TestScene052::TestScene052(Project001::Application* applicationPtr)
    : Scene(applicationPtr)
    , instructionScene_(applicationPtr)
    , font01_FontDataPtr_(nullptr)
    , font01_TextureDataPtr_(nullptr)
    , font01_TextureId_((unsigned int)-1)
    , circleMeshDataPtr_(nullptr)
    , mainCameraEntityId_((unsigned int)-1)
    , uiCameraEntityId_((unsigned int)-1)
    , cursorEntityId_((unsigned int)-1)
    , cursorPositionRenderedMeshIndex_((unsigned int)-1)
    , cursorPressRenderedMeshIndex_((unsigned int)-1)
    , cursorReleaseRenderedMeshIndex_((unsigned int)-1)
    , cursorPositionCollisionPointIndex_((unsigned int)-1)
    , cursorPressCollisionPointIndex_((unsigned int)-1)
    , cursorReleaseCollisionPointIndex_((unsigned int)-1)
    , previousWorldCursorPosition_()
    , previousWorldCursorPress_()
    , previousWorldCursorRelease_()
{
    GetSharedDataPtr<TestApplicationData>()->testScene052Id = GetId();
}

TestScene052::~TestScene052()
{}

void TestScene052::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene052::ProcessInitializeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene052::ProcessDeinitializeEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::CursorPositionEvent>(event, std::bind(&TestScene052::ProcessCursorPositionEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene052::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::MouseButtonEvent>(event, std::bind(&TestScene052::ProcessMouseButtonEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestScene052::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene052::ProcessUpdateEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene052::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   TestScene052:            " << GetId());

    // Font Data ---------------------------------------------------------------

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

    // Mesh Data ---------------------------------------------------------------

    circleMeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*circleMeshDataPtr_, 0.08f, 12));

    // Entity Ids --------------------------------------------------------------

    // Main Camera Entity ------------------------------------------------------

    float mainCameraHalfHeight = 0.0f;
    float mainCameraHalfWidth = 0.0f;
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
                mainCameraHalfHeight = 2.75f;
                mainCameraHalfWidth = aspectRatio * mainCameraHalfHeight;
                cameraPtr->SetAspectRatio(aspectRatio);
                cameraPtr->SetTopCutoff(mainCameraHalfHeight);
                cameraPtr->SetBottomCutoff(-mainCameraHalfHeight);
                cameraPtr->SetLeftCutoff(-mainCameraHalfWidth);
                cameraPtr->SetRightCutoff(mainCameraHalfWidth);
                cameraPtr->SetNearCutoff(-1.0f);
                cameraPtr->SetFarCutoff(1.0f);
            }
            cameraPtr->AddYaw(glm::pi<float>());
            cameraPtr->SetProjection(Project001::Camera::CameraProjection::CAMERA_PROJECTION_ORTHOGRAPHIC);
            cameraPtr->SetDepthTestEnabled(false);
            cameraPtr->TurnOn();

            GetSoundPlayerPtr()->SetListenerPosition(cameraPtr->GetPosition());
            GetSoundPlayerPtr()->SetListenerForwardDirection(cameraPtr->GetForwardVector());
            GetSoundPlayerPtr()->SetListenerUpDirection(cameraPtr->GetUpVector());
        }
    }

    // UI Camera Entity --------------------------------------------------------

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
            cameraPtr->TurnOn();
            cameraPtr->SetCameraMask(s_uiCameraMask_);
            cameraPtr->SetPriorityValue(1000000);
        }
    }

    // Cursor Entity -----------------------------------------------------------
    {
        float cursorX_position;
        float cursorY_position;
        GetWindowPtr()->GetCursorPosition(cursorX_position, cursorY_position);
        UpdatePreviousWorldCursorPosition(cursorX_position, cursorY_position);

        GetComponentStoresPtr()->CreateEntity(cursorEntityId_);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(cursorEntityId_));
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, cursorEntityId_));
        if (renderedModelPtr != nullptr)
        {
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

            float xPosition;
            float yPosition;
            GetWindowPtr()->GetCursorPosition(xPosition, yPosition);

            cursorPositionRenderedMeshIndex_ = renderedMeshes.size();
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& circleMesh01 = renderedMeshes.back();
            circleMesh01.SetMeshDataPtr(circleMeshDataPtr_);
            circleMesh01.SetPositionZ(0.53f);
            circleMesh01.SetColor(0.8f, 0.8f, 0.8f, 0.4f);
            circleMesh01.SetTranslucent(true);
            circleMesh01.SetLit(false);

            cursorPressRenderedMeshIndex_ = renderedMeshes.size();
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& circleMesh02 = renderedMeshes.back();
            circleMesh02.SetMeshDataPtr(circleMeshDataPtr_);
            circleMesh02.SetPositionZ(0.52f);
            circleMesh02.SetColor(0.8f, 0.2f, 0.2f, 0.4f);
            circleMesh02.SetTranslucent(true);
            circleMesh02.SetLit(false);
            circleMesh02.SetVisible(false);

            cursorReleaseRenderedMeshIndex_ = renderedMeshes.size();
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& circleMesh03 = renderedMeshes.back();
            circleMesh03.SetMeshDataPtr(circleMeshDataPtr_);
            circleMesh03.SetPositionZ(0.51f);
            circleMesh03.SetColor(0.8f, 0.8f, 0.2f, 0.4f);
            circleMesh03.SetTranslucent(true);
            circleMesh03.SetLit(false);
            circleMesh03.SetVisible(false);
        }

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(cursorEntityId_));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursorEntityId_));
        if (collisionBody2DPtr != nullptr)
        {
            std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
            cursorPositionCollisionPointIndex_ = collisionPoints.size();
            collisionPoints.emplace_back(glm::vec2(), s_cursorPositionCollisionShapeId_, true);
            cursorPressCollisionPointIndex_ = collisionPoints.size();
            collisionPoints.emplace_back(glm::vec2(), s_cursorPressCollisionShapeId_, false);
            cursorReleaseCollisionPointIndex_ = collisionPoints.size();
            collisionPoints.emplace_back(glm::vec2(), s_cursorReleaseCollisionShapeId_, false);
        }
    }

    // Member Scenes -----------------------------------------------------------

    TestInstructionScene001::InitializationInfo instructionSceneInfo;
    instructionSceneInfo.hiddenInstructionString = std::string("Press <Tab> to show instructions.");
    instructionSceneInfo.instructionString = std::string(
        "(TODO)\n"
        "Press <Esc> to return to Main Menu.\n"
        "Press <Tab> to hide instructions."
    );
    instructionSceneInfo.fontDataPtr = font01_FontDataPtr_;
    instructionSceneInfo.fontTextureIdPtr = &font01_TextureId_;
    instructionSceneInfo.cameraEntityIdPtr = &uiCameraEntityId_;
    instructionSceneInfo.cameraMaskPtr = &s_uiCameraMask_;
    instructionSceneInfo.keyCode_toggleInstructionsPtr = &s_keyCode_toggleInstructions_;
    instructionScene_.Initialize(instructionSceneInfo);
}

void TestScene052::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    LOG_INFO("DEINITIALIZING: TestScene052:            " << GetId());

    // -------------------------------------------------------------------------

    GetRendererPtr()->DeleteAllTextures();
    GetRendererPtr()->DeleteAllMeshes();
    GetSoundPlayerPtr()->DeleteAllSoundSources();
    GetSoundPlayerPtr()->DeleteAllSoundBuffers();
    GetComponentStoresPtr()->DeleteAllEntities();

    // Font Data ---------------------------------------------------------------

    delete font01_FontDataPtr_;
    font01_FontDataPtr_ = nullptr;
    delete font01_TextureDataPtr_;
    font01_TextureDataPtr_ = nullptr;
    font01_TextureId_ = (unsigned int)-1;

    delete circleMeshDataPtr_;
    circleMeshDataPtr_ = nullptr;

    // Entity Ids --------------------------------------------------------------

    mainCameraEntityId_ = (unsigned int)-1;
    uiCameraEntityId_ = (unsigned int)-1;

    cursorEntityId_ = (unsigned int)-1;
    cursorPositionRenderedMeshIndex_ = (unsigned int)-1;
    cursorPressRenderedMeshIndex_ = (unsigned int)-1;
    cursorReleaseRenderedMeshIndex_ = (unsigned int)-1;
    cursorPositionCollisionPointIndex_ = (unsigned int)-1;
    cursorPressCollisionPointIndex_ = (unsigned int)-1;
    cursorReleaseCollisionPointIndex_ = (unsigned int)-1;

    // -------------------------------------------------------------------------

    previousWorldCursorPosition_ = glm::vec2(0.0f, 0.0f);
    previousWorldCursorPress_ = glm::vec2(0.0f, 0.0f);
    previousWorldCursorRelease_ = glm::vec2(0.0f, 0.0f);
}

void TestScene052::ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorPositionEvent)
{
    UpdatePreviousWorldCursorPosition(cursorPositionEvent.xPosition, cursorPositionEvent.yPosition);

    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, cursorEntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetVisible(true);
    }

    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursorEntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        collisionBody2DPtr->SetTangible(true);
    }
}

void TestScene052::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
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

void TestScene052::ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent)
{
    Project001::MouseButton& mouseButton = mouseButtonEvent.mouseButton;
    Project001::ButtonAction& buttonAction = mouseButtonEvent.buttonAction;

    if (mouseButton == Project001::MouseButton::MOUSE_BUTTON_1)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS)
        {
            Project001::RenderedModel* renderedModelPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, cursorEntityId_));
            if (renderedModelPtr != nullptr)
            {
                renderedModelPtr->SetVisible(true);
                std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
                renderedMeshes[cursorPressRenderedMeshIndex_].SetVisible(true);
                renderedMeshes[cursorReleaseRenderedMeshIndex_].SetVisible(false);
            }

            Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursorEntityId_));
            if (collisionBody2DPtr != nullptr)
            {
                collisionBody2DPtr->SetTangible(true);
                std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
                collisionPoints[cursorPressCollisionPointIndex_].tangible = true;
                collisionPoints[cursorReleaseCollisionPointIndex_].tangible = false;
            }

            previousWorldCursorPress_ = previousWorldCursorPosition_;
        }
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
        {
            Project001::RenderedModel* renderedModelPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, cursorEntityId_));
            if (renderedModelPtr != nullptr)
            {
                renderedModelPtr->SetVisible(true);
                std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
                renderedMeshes[cursorReleaseRenderedMeshIndex_].SetVisible(true);
            }

            Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursorEntityId_));
            if (collisionBody2DPtr != nullptr)
            {
                collisionBody2DPtr->SetTangible(true);
                std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
                collisionPoints[cursorReleaseCollisionPointIndex_].tangible = true;
            }

            previousWorldCursorRelease_ = previousWorldCursorPosition_;
        }
    }
}

void TestScene052::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    Project001::RenderSystem::Render(GetComponentStoresPtr(), GetRendererPtr());
}

void TestScene052::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    // Sync cursor rendered model and collision body
    // -------------------------------------------------------------------------
    {
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, cursorEntityId_));
        if (renderedModelPtr != nullptr)
        {
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

            Project001::RenderedMesh& circleMesh01 = renderedMeshes[cursorPositionRenderedMeshIndex_];
            circleMesh01.SetPositionX(previousWorldCursorPosition_.x);
            circleMesh01.SetPositionY(previousWorldCursorPosition_.y);

            Project001::RenderedMesh& circleMesh02 = renderedMeshes[cursorPressRenderedMeshIndex_];
            circleMesh02.SetPositionX(previousWorldCursorPress_.x);
            circleMesh02.SetPositionY(previousWorldCursorPress_.y);

            Project001::RenderedMesh& circleMesh03 = renderedMeshes[cursorReleaseRenderedMeshIndex_];
            circleMesh03.SetPositionX(previousWorldCursorRelease_.x);
            circleMesh03.SetPositionY(previousWorldCursorRelease_.y);
        }

        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursorEntityId_));
        if (collisionBody2DPtr != nullptr)
        {
            std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();

            collisionPoints[cursorPositionCollisionPointIndex_].position.x = previousWorldCursorPosition_.x;
            collisionPoints[cursorPositionCollisionPointIndex_].position.y = previousWorldCursorPosition_.y;

            collisionPoints[cursorPressCollisionPointIndex_].position.x = previousWorldCursorPress_.x;
            collisionPoints[cursorPressCollisionPointIndex_].position.y = previousWorldCursorPress_.y;

            collisionPoints[cursorReleaseCollisionPointIndex_].position.x = previousWorldCursorRelease_.x;
            collisionPoints[cursorReleaseCollisionPointIndex_].position.y = previousWorldCursorRelease_.y;
        }
    }

    // Calculate collisions
    // -------------------------------------------------------------------------
    // Project001::CollisionSystem2D::CalculateCollisions(GetComponentStoresPtr());
}

void TestScene052::UpdatePreviousWorldCursorPosition(float xPosition, float yPosition)
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
        Project001::Camera* cameraPtr;
        if (GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraEntityId_))
        {
            // Convert coordinates from viewport to world
            glm::vec2 worldCursorPosition = cameraPtr->ConvertPointFromViewportToOrthoWorld(
                viewportWidth,
                viewportHeight,
                viewportCursorPosition
            );

            previousWorldCursorPosition_ = worldCursorPosition;
        }
    }
}