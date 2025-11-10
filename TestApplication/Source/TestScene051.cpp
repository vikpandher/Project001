// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-11-10

#include "TestScene051.h"

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

TestScene051::TestScene051(Project001::Application* applicationPtr)
    : Scene(applicationPtr)
    , instructionScene_(applicationPtr)
    , sound01_SoundDataPtr_(nullptr)
    , sound01_SoundBufferId_((unsigned int)-1)
    , font01_FontDataPtr_(nullptr)
    , font01_TextureDataPtr_(nullptr)
    , font01_TextureId_((unsigned int)-1)
    , rectangleMeshDataPtr_(nullptr)
    , rectangularMeshId_((unsigned int)-1)
    , cursorCircleMeshDataPtr_(nullptr)
    , buttonTextMeshDataPtrs_()
    , mainCameraEntityId_((unsigned int)-1)
    , uiCameraEntityId_((unsigned int)-1)
    , cursorEntityId_((unsigned int)-1)
    , cursorPositionRenderedMeshIndex_((unsigned int)-1)
    , cursorPressRenderedMeshIndex_((unsigned int)-1)
    , cursorReleaseRenderedMeshIndex_((unsigned int)-1)
    , cursorPositionCollisionPointIndex_((unsigned int)-1)
    , cursorPressCollisionPointIndex_((unsigned int)-1)
    , cursorReleaseCollisionPointIndex_((unsigned int)-1)
    , buttonEntityIds_()
{
    GetSharedDataPtr<TestApplicationData>()->testScene051Id = GetId();
}

TestScene051::~TestScene051()
{}

void TestScene051::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene051::ProcessInitializeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene051::ProcessDeinitializeEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::CursorPositionEvent>(event, std::bind(&TestScene051::ProcessCursorPositionEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene051::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::MouseButtonEvent>(event, std::bind(&TestScene051::ProcessMouseButtonEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestScene051::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene051::ProcessUpdateEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene051::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   TestScene051:            " << GetId());

    // SoundData ---------------------------------------------------------------

    sound01_SoundDataPtr_ = new Project001::SoundData();
    FAIL_CHECK(Project001::SoundLoader::LoadSoundOGG(*sound01_SoundDataPtr_, "../Sounds/Congratulations.ogg"));

    FAIL_CHECK(GetSoundPlayerPtr()->CreateSoundBuffer(
        sound01_SoundBufferId_,
        sound01_SoundDataPtr_->data,
        sound01_SoundDataPtr_->sizeInBytes,
        sound01_SoundDataPtr_->numberOfChannels,
        sound01_SoundDataPtr_->sampleRate_Hz,
        sound01_SoundDataPtr_->bitsPerSample,
        sound01_SoundDataPtr_->sizeInFrames
    ));

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

    cursorCircleMeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*cursorCircleMeshDataPtr_, 0.08f, 12));

    std::vector<std::string> buttonStrings;
    for (size_t i = 32; i < 128; ++i)
    {
        buttonStrings.emplace_back(1, (char)i);
    }

    for (size_t i = 0; i < buttonStrings.size(); ++i)
    {
        Project001::MeshData* newTextMeshDataPtr = new Project001::MeshData();
        FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
            *newTextMeshDataPtr,
            *font01_FontDataPtr_,
            buttonStrings[i],
            buttonFontPixelSize_
        ));
        Project001::MeshLoader::RecenterMesh(*newTextMeshDataPtr);
        buttonTextMeshDataPtrs_.push_back(newTextMeshDataPtr);
    }

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
            cameraPtr->SetCameraMask(s_mainCameraMask_);

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
            cameraPtr->SetCameraMask(s_uiCameraMask_);
            cameraPtr->SetPriorityValue(1000000);
        }
    }

    // Cursor Entity -----------------------------------------------------------
    {
        GetComponentStoresPtr()->CreateEntity(cursorEntityId_);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(cursorEntityId_));
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, cursorEntityId_));
        if (renderedModelPtr != nullptr)
        {
            renderedModelPtr->SetCameraMask(s_mainCameraMask_);
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

            float xPosition;
            float yPosition;
            GetWindowPtr()->GetCursorPosition(xPosition, yPosition);

            cursorPositionRenderedMeshIndex_ = renderedMeshes.size();
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& circleMesh01 = renderedMeshes.back();
            circleMesh01.SetMeshDataPtr(cursorCircleMeshDataPtr_);
            circleMesh01.SetPositionZ(0.53f);
            circleMesh01.SetColor(0.8f, 0.8f, 0.8f, 0.4f);
            circleMesh01.SetTranslucent(true);
            circleMesh01.SetUseLighting(false);

            cursorPressRenderedMeshIndex_ = renderedMeshes.size();
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& circleMesh02 = renderedMeshes.back();
            circleMesh02.SetMeshDataPtr(cursorCircleMeshDataPtr_);
            circleMesh02.SetPositionZ(0.52f);
            circleMesh02.SetColor(0.8f, 0.2f, 0.2f, 0.4f);
            circleMesh02.SetTranslucent(true);
            circleMesh02.SetUseLighting(false);
            circleMesh02.SetVisible(false);

            cursorReleaseRenderedMeshIndex_ = renderedMeshes.size();
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& circleMesh03 = renderedMeshes.back();
            circleMesh03.SetMeshDataPtr(cursorCircleMeshDataPtr_);
            circleMesh03.SetPositionZ(0.51f);
            circleMesh03.SetColor(0.8f, 0.8f, 0.2f, 0.4f);
            circleMesh03.SetTranslucent(true);
            circleMesh03.SetUseLighting(false);
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

        float cursorX_position;
        float cursorY_position;
        GetWindowPtr()->GetCursorPosition(cursorX_position, cursorY_position);
        UpdateCursorPosition(cursorX_position, cursorY_position);
    }

    // Button Positions --------------------------------------------------------

    size_t numRows = 8;
    size_t numCols = 12;

    float canvasHorizontalSpacing = 0.25f;
    float canvasVerticalSpacing = 0.25f;

    float canvasWidth = mainCameraHalfWidth * 2.0f - canvasHorizontalSpacing * 2.0f;
    float canvasHeight = mainCameraHalfHeight * 2.0f - canvasVerticalSpacing * 2.0f;

    float horizontalSpacing = 0.0f;
    float verticalSpacing = 0.0f;

    float cellWidth = canvasWidth / numCols;
    float cellHeight = canvasHeight / numRows;

    float buttonWidth = cellWidth - horizontalSpacing * 2.0f;
    float buttonHeight = cellHeight - verticalSpacing * 2.0f;

    float xPos = -mainCameraHalfWidth + cellWidth * 0.5f + canvasHorizontalSpacing;
    float yPos = mainCameraHalfHeight - cellHeight * 0.5 - canvasVerticalSpacing;

    float initialXPos = xPos;

    std::vector<glm::vec2> buttonPositions;
    for (size_t y = 0; y < numRows; ++y) {
        for (size_t x = 0; x < numCols; ++x) {
            buttonPositions.emplace_back(xPos, yPos);
            xPos += cellWidth;
        }
        xPos = initialXPos;
        yPos -= cellHeight;
    }

    // Button Mesh -------------------------------------------------------------

    rectangleMeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
        *rectangleMeshDataPtr_,
        buttonWidth,
        buttonHeight,
        0.0f,
        1.0f,
        0.0f,
        1.0f
    ));
    GetRendererPtr()->CreateMesh(
        rectangularMeshId_,
        rectangleMeshDataPtr_->meshVertexArray.data(),
        (unsigned int)rectangleMeshDataPtr_->meshVertexArray.size(),
        rectangleMeshDataPtr_->meshIndexArray.data(),
        (unsigned int)rectangleMeshDataPtr_->meshIndexArray.size()
    );

    // Button Sound Source -----------------------------------------------------

    for (size_t i = 0; i < buttonStrings.size(); ++i)
    {
        buttonSoundSourceIds_.emplace_back();
        unsigned int& currentSoundSourceId = buttonSoundSourceIds_.back();

        FAIL_CHECK(GetSoundPlayerPtr()->CreateSoundSource(
            currentSoundSourceId,
            sound01_SoundBufferId_
        ));

        FAIL_CHECK(GetSoundPlayerPtr()->SetSoundSourcePitch(
            currentSoundSourceId,
            0.1f + 0.02f * (float)i
        ));
    }

    // Button Entities
    // -------------------------------------------------------------------------

    for (size_t i = 0; i < buttonStrings.size(); ++i)
    {
        unsigned int buttonEntityId;
        GetComponentStoresPtr()->CreateEntity(buttonEntityId);

        buttonEntityIds_.push_back(buttonEntityId);

        // ---------------------------------------------------------------------

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(buttonEntityId));
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, buttonEntityId));
        if (renderedModelPtr != nullptr)
        {
            renderedModelPtr->SetCameraMask(s_mainCameraMask_);
            renderedModelPtr->SetPosition(
                buttonPositions[i].x,
                buttonPositions[i].y,
                0.0f
            );
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

            renderedMeshes.emplace_back();
            Project001::RenderedMesh& buttonMesh = renderedMeshes.back();
            buttonMesh.SetMeshIdAndMaxBoundingRadius(rectangularMeshId_, rectangleMeshDataPtr_->maxBoundingRadius);
            buttonMesh.SetColor(s_buttonColor_);
            buttonMesh.SetUseLighting(false);

            renderedMeshes.emplace_back();
            Project001::RenderedMesh& textMesh = renderedMeshes.back();
            textMesh.SetMeshDataPtr(buttonTextMeshDataPtrs_[i]);
            textMesh.SetTextureId(font01_TextureId_);
            textMesh.SetPositionZ(0.01f);
            textMesh.SetUseLighting(false);
        }

        // ---------------------------------------------------------------------

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(buttonEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, buttonEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(buttonPositions[i]);
            std::vector<Project001::CollisionRectangle2D>& collisionRectangles = collisionBody2DPtr->GetCollisionRectangles();
            collisionRectangles.emplace_back(
                glm::vec2(-0.5f * buttonWidth, -0.5f * buttonHeight),
                glm::vec2(0.5f * buttonWidth, 0.5f * buttonHeight)
            );
        }
    }

    // Member Scenes -----------------------------------------------------------

    TestInstructionScene001::InitializationInfo instructionSceneInfo;
    instructionSceneInfo.hiddenInstructionString = std::string("Press <Tab> to show instructions.");
    instructionSceneInfo.instructionString = std::string(
        "This Scene tests playing multiple instances\n"
        "of the same sound. <Left-Click> on a tile to\n"
        "start playing an instance.\n"
        "Press <Esc> to return to Main Menu.\n"
        "Press <Tab> to hide instructions."
    );
    instructionSceneInfo.fontDataPtr = font01_FontDataPtr_;
    instructionSceneInfo.fontTextureId = font01_TextureId_;
    instructionSceneInfo.cameraEntityId = uiCameraEntityId_;
    instructionSceneInfo.cameraMask = s_uiCameraMask_;
    instructionSceneInfo.keyCode_toggleInstructions = s_keyCode_toggleInstructions_;
    instructionScene_.Initialize(instructionSceneInfo);
}

void TestScene051::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    LOG_INFO("DEINITIALIZING: TestScene051:            " << GetId());

    // -------------------------------------------------------------------------

    GetRendererPtr()->DeleteAllTextures();
    GetRendererPtr()->DeleteAllMeshes();
    GetSoundPlayerPtr()->DeleteAllSoundSources();
    GetSoundPlayerPtr()->DeleteAllSoundBuffers();
    GetComponentStoresPtr()->DeleteAllEntities();

    // Sound Data --------------------------------------------------------------

    delete sound01_SoundDataPtr_;
    sound01_SoundDataPtr_ = nullptr;
    sound01_SoundBufferId_ = (unsigned int)-1;
    buttonSoundSourceIds_.clear();

    // Font Data ---------------------------------------------------------------

    delete font01_FontDataPtr_;
    font01_FontDataPtr_ = nullptr;
    delete font01_TextureDataPtr_;
    font01_TextureDataPtr_ = nullptr;
    font01_TextureId_ = (unsigned int)-1;

    // Mesh Data ---------------------------------------------------------------

    delete rectangleMeshDataPtr_;
    rectangleMeshDataPtr_ = nullptr;
    rectangularMeshId_ = (unsigned int)-1;

    delete cursorCircleMeshDataPtr_;
    cursorCircleMeshDataPtr_ = nullptr;

    for (size_t i = 0; i < buttonTextMeshDataPtrs_.size(); ++i)
    {
        delete buttonTextMeshDataPtrs_[i];
    }
    buttonTextMeshDataPtrs_.clear();

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

    buttonEntityIds_.clear();
}

void TestScene051::ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorPositionEvent)
{
    UpdateCursorPosition(cursorPositionEvent.xPosition, cursorPositionEvent.yPosition);

    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursorEntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
        collisionPoints[cursorPositionCollisionPointIndex_].enabled = true;
    }
}

void TestScene051::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
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

void TestScene051::ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent)
{
    Project001::MouseButton& mouseButton = mouseButtonEvent.mouseButton;
    Project001::ButtonAction& buttonAction = mouseButtonEvent.buttonAction;

    if (mouseButton == Project001::MouseButton::MOUSE_BUTTON_1)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS)
        {
            Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursorEntityId_));
            if (collisionBody2DPtr != nullptr)
            {
                std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
                collisionPoints[cursorPressCollisionPointIndex_].position = collisionPoints[cursorPositionCollisionPointIndex_].position;
                collisionPoints[cursorPressCollisionPointIndex_].enabled = true;
                collisionPoints[cursorReleaseCollisionPointIndex_].enabled = false;
            }
        }
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
        {
            Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursorEntityId_));
            if (collisionBody2DPtr != nullptr)
            {
                std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
                collisionPoints[cursorReleaseCollisionPointIndex_].position = collisionPoints[cursorPositionCollisionPointIndex_].position;
                collisionPoints[cursorReleaseCollisionPointIndex_].enabled = true;
            }
        }
    }
}

void TestScene051::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    Project001::RenderSystem::Render(GetComponentStoresPtr(), GetRendererPtr());
}

void TestScene051::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    // Sync cursor rendered model and collision body
    // -------------------------------------------------------------------------
    {
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursorEntityId_));
        if (collisionBody2DPtr != nullptr)
        {
            std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
            Project001::CollisionPoint2D collisionPoint01 = collisionPoints[cursorPositionCollisionPointIndex_];
            Project001::CollisionPoint2D collisionPoint02 = collisionPoints[cursorPressCollisionPointIndex_];
            Project001::CollisionPoint2D collisionPoint03 = collisionPoints[cursorReleaseCollisionPointIndex_];

            Project001::RenderedModel* renderedModelPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, cursorEntityId_));
            if (renderedModelPtr != nullptr)
            {
                std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

                Project001::RenderedMesh& circleMesh01 = renderedMeshes[cursorPositionRenderedMeshIndex_];
                Project001::RenderedMesh& circleMesh02 = renderedMeshes[cursorPressRenderedMeshIndex_];
                Project001::RenderedMesh& circleMesh03 = renderedMeshes[cursorReleaseRenderedMeshIndex_];

                circleMesh01.SetPositionX(collisionPoint01.position.x);
                circleMesh01.SetPositionY(collisionPoint01.position.y);
                circleMesh01.SetVisible(collisionPoint01.enabled);


                circleMesh02.SetPositionX(collisionPoint02.position.x);
                circleMesh02.SetPositionY(collisionPoint02.position.y);
                circleMesh02.SetVisible(collisionPoint02.enabled);

                circleMesh03.SetPositionX(collisionPoint03.position.x);
                circleMesh03.SetPositionY(collisionPoint03.position.y);
                circleMesh03.SetVisible(collisionPoint03.enabled);
            }
        }
    }

    // Calculate collisions
    // -------------------------------------------------------------------------
    Project001::CollisionSystem2D::CalculateCollisions(GetComponentStoresPtr());

    // Recolor all buttons and play sounds
    // -------------------------------------------------------------------------
    {
        bool leftMouseButtonDown = GetWindowPtr()->GetMouseButtonPressed(Project001::MouseButton::MOUSE_BUTTON_1);

        for (size_t i = 0; i < buttonEntityIds_.size(); ++i)
        {
            const unsigned int& buttonEntityId = buttonEntityIds_[i];

            bool collidingWithCursorPosition = false;
            bool collidingWithCursorPress = false;
            bool collidingWithCursorRelease = false;

            Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, buttonEntityId));
            if (collisionBody2DPtr != nullptr)
            {
                const std::vector<Project001::CollisionData2D> collisions = collisionBody2DPtr->GetCollisions();
                for (size_t i = 0; i < collisions.size(); ++i)
                {
                    const Project001::CollisionData2D& collisionData = collisions[i];

                    if (collisionData.otherEntityId == cursorEntityId_)
                    {
                        if (collisionData.otherShapeTag == s_cursorPositionCollisionShapeId_)
                        {
                            collidingWithCursorPosition = true;
                        }
                        if (collisionData.otherShapeTag == s_cursorPressCollisionShapeId_)
                        {
                            collidingWithCursorPress = true;
                        }
                        if (collisionData.otherShapeTag == s_cursorReleaseCollisionShapeId_)
                        {
                            collidingWithCursorRelease = true;
                        }
                    }
                }
            }

            Project001::RenderedModel* renderedModelPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, buttonEntityId));
            if (renderedModelPtr != nullptr)
            {
                std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
                Project001::RenderedMesh& buttonMesh = renderedMeshes[0];

                if (leftMouseButtonDown && collidingWithCursorPosition)
                {
                    GetSoundPlayerPtr()->PlaySoundSource(buttonSoundSourceIds_[i]);
                }

                if (GetSoundPlayerPtr()->GetIsPlayingSoundSource(buttonSoundSourceIds_[i]))
                {
                    buttonMesh.SetColor(s_buttonColor2_);
                }
                else
                {
                    buttonMesh.SetColor(s_buttonColor_);
                }

                if (collidingWithCursorPress && collidingWithCursorRelease)
                {
                    // Do Nothing
                }
            }
        }
    }
}

void TestScene051::UpdateCursorPosition(float xPosition, float yPosition)
{
    int windowWidth, windowHeight;
    GetWindowPtr()->GetWindowSize(windowWidth, windowHeight);

    glm::vec2 viewportNormalizedCursorPosition = GetRendererPtr()->ConvertPointFromWindowToViewportNormalized(glm::vec2(xPosition, yPosition), (float)windowHeight);

    Project001::Camera* cameraPtr;
    if (GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraEntityId_))
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
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursorEntityId_));
            if (collisionBody2DPtr != nullptr)
            {
                std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();

                collisionPoints[cursorPositionCollisionPointIndex_].position = worldCursorPosition;
            }
        }
    }
}

const glm::vec4 TestScene051::s_buttonColor_ = glm::vec4(0.8f, 0.3f, 0.2f, 1.0f);
const glm::vec4 TestScene051::s_buttonColor2_ = glm::vec4(0.8f, 0.7f, 0.2f, 1.0f);