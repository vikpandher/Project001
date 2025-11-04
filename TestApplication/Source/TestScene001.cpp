// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-11-03

#include "TestScene001.h"

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
#include "SoundPlayer.h"
#include "TextureLoader.h"
#include "Window.h"



struct TestButtonData001
{
    std::string textString;

    unsigned int destinationSceneId = (unsigned int)-1;

    unsigned int aboveEntityId = (unsigned int)-1;
    unsigned int leftEntityId = (unsigned int)-1;
    unsigned int bellowEntityId = (unsigned int)-1;
    unsigned int rightEntityId = (unsigned int)-1;
};

// public ----------------------------------------------------------------------

TestScene001::TestScene001(Project001::Application* applicationPtr)
    : Scene(applicationPtr)
    , instructionScene_(applicationPtr)
    , font01_FontDataPtr_(nullptr)
    , font01_TextureDataPtr_(nullptr)
    , font01_TextureId_((unsigned int)-1)
    , rectangleMeshDataPtr_(nullptr)
    , rectangularMeshId_((unsigned int)-1)
    , selectorMeshDataPtr_(nullptr)
    , selectorMeshId_((unsigned int)-1)
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
    , selectorEntityId_((unsigned int)-1)
    , selectedEntityId_((unsigned int)-1)
{
    GetSharedDataPtr<TestApplicationData>()->testScene001Id = GetId();
}

TestScene001::~TestScene001()
{}

void TestScene001::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene001::ProcessInitializeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene001::ProcessDeinitializeEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::CursorPositionEvent>(event, std::bind(&TestScene001::ProcessCursorPositionEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene001::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::MouseButtonEvent>(event, std::bind(&TestScene001::ProcessMouseButtonEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestScene001::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene001::ProcessUpdateEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene001::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   TestScene001:            " << GetId());

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

    float buttonRectangleWidth = 1.25f;
    float buttonRectangleHeight = 0.5f;

    rectangleMeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
        *rectangleMeshDataPtr_,
        buttonRectangleWidth,
        buttonRectangleHeight,
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

    selectorMeshDataPtr_ = new Project001::MeshData();

    std::vector<glm::vec2> triangleFanPositions;
    triangleFanPositions.emplace_back(0.625f, 0.25f);
    triangleFanPositions.emplace_back(0.625f, 0.125f);
    triangleFanPositions.emplace_back(0.75f, 0.125f);
    triangleFanPositions.emplace_back(0.75f, 0.375f);
    triangleFanPositions.emplace_back(0.25f, 0.375f);
    triangleFanPositions.emplace_back(0.25f, 0.25f);
    Project001::MeshLoader::Generate2DTriangleFan(*selectorMeshDataPtr_, triangleFanPositions);

    triangleFanPositions.clear();
    triangleFanPositions.emplace_back(-0.625f, 0.25f);
    triangleFanPositions.emplace_back(-0.25f, 0.25f);
    triangleFanPositions.emplace_back(-0.25f, 0.375f);
    triangleFanPositions.emplace_back(-0.75f, 0.375f);
    triangleFanPositions.emplace_back(-0.75f, 0.125f);
    triangleFanPositions.emplace_back(-0.625f, 0.125f);
    Project001::MeshLoader::Generate2DTriangleFan(*selectorMeshDataPtr_, triangleFanPositions);

    triangleFanPositions.clear();
    triangleFanPositions.emplace_back(-0.625f, -0.25f);
    triangleFanPositions.emplace_back(-0.625f, -0.125f);
    triangleFanPositions.emplace_back(-0.75f, -0.125f);
    triangleFanPositions.emplace_back(-0.75f, -0.375f);
    triangleFanPositions.emplace_back(-0.25f, -0.375f);
    triangleFanPositions.emplace_back(-0.25f, -0.25f);
    Project001::MeshLoader::Generate2DTriangleFan(*selectorMeshDataPtr_, triangleFanPositions);

    triangleFanPositions.clear();
    triangleFanPositions.emplace_back(0.625f, -0.25f);
    triangleFanPositions.emplace_back(0.25f, -0.25f);
    triangleFanPositions.emplace_back(0.25f, -0.375f);
    triangleFanPositions.emplace_back(0.75f, -0.375f);
    triangleFanPositions.emplace_back(0.75f, -0.125f);
    triangleFanPositions.emplace_back(0.625f, -0.125f);
    Project001::MeshLoader::Generate2DTriangleFan(*selectorMeshDataPtr_, triangleFanPositions);

    GetRendererPtr()->CreateMesh(
        selectorMeshId_,
        selectorMeshDataPtr_->meshVertexArray.data(),
        (unsigned int)selectorMeshDataPtr_->meshVertexArray.size(),
        selectorMeshDataPtr_->meshIndexArray.data(),
        (unsigned int)selectorMeshDataPtr_->meshIndexArray.size()
    );

    cursorCircleMeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*cursorCircleMeshDataPtr_, 0.08f, 12));

    std::vector<std::string> buttonStrings;
    buttonStrings.emplace_back("TestScene002");
    buttonStrings.emplace_back("TestScene003");
    buttonStrings.emplace_back("TestScene004");
    buttonStrings.emplace_back("TestScene005");
    buttonStrings.emplace_back("TestScene006");
    buttonStrings.emplace_back("TestScene007");
    buttonStrings.emplace_back("TestScene009");
    buttonStrings.emplace_back("TestScene020");
    buttonStrings.emplace_back("TestScene021");
    buttonStrings.emplace_back("TestScene022");
    buttonStrings.emplace_back("TestScene023");
    buttonStrings.emplace_back("TestScene025");
    buttonStrings.emplace_back("TestScene026");
    buttonStrings.emplace_back("TestScene027");
    buttonStrings.emplace_back("TestScene030");
    buttonStrings.emplace_back("TestScene031");
    buttonStrings.emplace_back("TestScene032");
    buttonStrings.emplace_back("TestScene033");
    buttonStrings.emplace_back("TestScene034");
    buttonStrings.emplace_back("TestScene050");
    buttonStrings.emplace_back("TestScene051");
    buttonStrings.emplace_back("TestScene060");
    buttonStrings.emplace_back("TestScene070");
    buttonStrings.emplace_back("TestScene102");
    buttonStrings.emplace_back("TestScene103");

    std::vector<unsigned int> buttonDestinationSceneIds;

    TestApplicationData* testApplicationDataPtr = GetSharedDataPtr<TestApplicationData>();
    buttonDestinationSceneIds.push_back(testApplicationDataPtr->testScene002Id);
    buttonDestinationSceneIds.push_back(testApplicationDataPtr->testScene003Id);
    buttonDestinationSceneIds.push_back(testApplicationDataPtr->testScene004Id);
    buttonDestinationSceneIds.push_back(testApplicationDataPtr->testScene005Id);
    buttonDestinationSceneIds.push_back(testApplicationDataPtr->testScene006Id);
    buttonDestinationSceneIds.push_back(testApplicationDataPtr->testScene007Id);
    buttonDestinationSceneIds.push_back(testApplicationDataPtr->testScene009Id);
    buttonDestinationSceneIds.push_back(testApplicationDataPtr->testScene020Id);
    buttonDestinationSceneIds.push_back(testApplicationDataPtr->testScene021Id);
    buttonDestinationSceneIds.push_back(testApplicationDataPtr->testScene022Id);
    buttonDestinationSceneIds.push_back(testApplicationDataPtr->TestScene028Id);
    buttonDestinationSceneIds.push_back(testApplicationDataPtr->testScene025Id);
    buttonDestinationSceneIds.push_back(testApplicationDataPtr->testScene026Id);
    buttonDestinationSceneIds.push_back(testApplicationDataPtr->testScene027Id);
    buttonDestinationSceneIds.push_back(testApplicationDataPtr->testScene030Id);
    buttonDestinationSceneIds.push_back(testApplicationDataPtr->testScene031Id);
    buttonDestinationSceneIds.push_back(testApplicationDataPtr->testScene032Id);
    buttonDestinationSceneIds.push_back(testApplicationDataPtr->testScene033Id);
    buttonDestinationSceneIds.push_back(testApplicationDataPtr->testScene034Id);
    buttonDestinationSceneIds.push_back(testApplicationDataPtr->testScene050Id);
    buttonDestinationSceneIds.push_back(testApplicationDataPtr->testScene051Id);
    buttonDestinationSceneIds.push_back(testApplicationDataPtr->testScene060Id);
    buttonDestinationSceneIds.push_back(testApplicationDataPtr->testScene070Id);
    buttonDestinationSceneIds.push_back(testApplicationDataPtr->testScene102Id);
    buttonDestinationSceneIds.push_back(testApplicationDataPtr->testScene103Id);

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
                float mainCameraHalfHeight = 2.75f;
                float mainCameraHalfWidth = aspectRatio * mainCameraHalfHeight;
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

        Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo = {};
        collisionBody2DCreationInfo.physicsType = Project001::CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR;

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(cursorEntityId_, collisionBody2DCreationInfo));
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

    // Button Entities
    // -------------------------------------------------------------------------

    // Button Indices:
    // 
    // [ 000 ] [ 001 ] [ 002 ] [ 003 ] [ 004 ]
    // [ 005 ] [ 006 ] [ 007 ] [ 008 ] [ 009 ]
    // [ 010 ] [ 011 ] [ 012 ] [ 013 ] [ 014 ]
    // [ 015 ] [ 016 ] [ 017 ] [ 018 ] [ 019 ]
    // [ 020 ] [ 021 ] [ 022 ] [ 023 ] [ 024 ]
    // 
    // Test Scenes:
    // 
    // [ 002 ] [ 003 ] [ 004 ] [ 005 ] [ 006 ]
    // [ 007 ] [ 008 ] [ 010 ] [ 011 ] [ 012 ]
    // [ 013 ] [ 015 ] [ 016 ] [ 017 ] [ 030 ]
    // [ 031 ] [ 032 ] [ 033 ] [ 034 ] [ 050 ]
    // [ 051 ] [ 060 ] [ 070 ] [ 102 ] [ 103 ]

    size_t columns = 5;
    size_t rows = buttonStrings.size() / columns + 1;
    std::vector<glm::vec3> buttonPositions;
    float xPos = -3.0f;
    float yPos = 2.0f;
    for (size_t y = 0; y < rows; ++y)
    {
        for (size_t x = 0; x < columns; ++x)
        {
            buttonPositions.emplace_back(xPos, yPos, 0.0f);
            xPos += 1.5f;
        }
        xPos = -3.0;
        yPos -= 0.75f;
    }

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
            renderedModelPtr->SetPosition(buttonPositions[i]);
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

        Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo = {};
        collisionBody2DCreationInfo.physicsType = Project001::CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR;

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(buttonEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, buttonEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(buttonPositions[i]);
            std::vector<Project001::CollisionRectangle2D>& collisionRectangles = collisionBody2DPtr->GetCollisionRectangles();
            collisionRectangles.emplace_back(
                glm::vec2(-0.5f * buttonRectangleWidth, -0.5f * buttonRectangleHeight),
                glm::vec2(0.5f * buttonRectangleWidth, 0.5f * buttonRectangleHeight)
            );
        }
    }

    for (size_t i = 0; i < buttonEntityIds_.size(); ++i)
    {
        const unsigned int& buttonEntityId = buttonEntityIds_[i];

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<TestButtonData001>(buttonEntityId));
        TestButtonData001* testButtonData001Ptr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<TestButtonData001>(testButtonData001Ptr, buttonEntityId));
        if (testButtonData001Ptr != nullptr)
        {
            testButtonData001Ptr->textString = buttonStrings[i];
            testButtonData001Ptr->destinationSceneId = buttonDestinationSceneIds[i];

            // columnIndex, rowIndex
            // [ 0,0 ] [ 1,0 ] [ 2,0 ] [ 3,0 ] [ 4,0 ]
            // [ 0,1 ] [ 1,1 ] [ 2,1 ] [ 3,1 ] [ 4,1 ]
            // [ 0,2 ] [ 1,2 ] [ 2,2 ] [ 3,2 ] [ 4,2 ]
            // [ 0,3 ] [ 1,3 ] [ 2,3 ] [ 3,3 ] [ 4,3 ]
            // [ 0,4 ] [ 1,4 ] [ 2,4 ] [ 3,4 ] [ 4,4 ]

            int currentColumnIndex = i % columns;
            int currentRowIndex = i / columns;

            int aboveRowIndex;
            if (currentRowIndex == 0)
            {
                // first row goes up to the last row
                aboveRowIndex = rows - 1;
                if (aboveRowIndex * columns + currentColumnIndex >= buttonEntityIds_.size())
                {
                    // if last row spot doesn't exist, go up to the second last row
                    aboveRowIndex--;
                }
            }
            else
            {
                // rows other then the first go up one row
                aboveRowIndex = currentRowIndex - 1;
            }
            int aboveEntityIndex = aboveRowIndex * columns + currentColumnIndex;
            testButtonData001Ptr->aboveEntityId = buttonEntityIds_[aboveEntityIndex];

            int bellowRowIndex;
            if (currentRowIndex == rows - 1)
            {
                // the last row goes down to the first row
                bellowRowIndex = 0;
            }
            else
            {
                // rows other then the last go down one row
                bellowRowIndex = currentRowIndex + 1;
                if (bellowRowIndex == rows - 1 &&
                    bellowRowIndex * columns + currentColumnIndex >= buttonEntityIds_.size())
                {
                    // if the bellow row is the last row and
                    // if the bellow row spot doesn't exist, go to the first row
                    bellowRowIndex = 0;
                }
            }
            int bellowEntityIndex = bellowRowIndex * columns + currentColumnIndex;
            testButtonData001Ptr->bellowEntityId = buttonEntityIds_[bellowEntityIndex];

            int leftColumnIndex;
            if (currentColumnIndex == 0)
            {
                // first column goes left to the last column
                leftColumnIndex = columns - 1;
                if (currentRowIndex == rows - 1 &&
                    currentRowIndex * columns + leftColumnIndex >= buttonEntityIds_.size())
                {
                    // if the current row is the last row and
                    // if the last column doesn't exist, go to the farthest left column
                    leftColumnIndex = (buttonEntityIds_.size() - 1) % columns;
                }
            }
            else
            {
                // columns other then the first go left down one column
                leftColumnIndex = currentColumnIndex - 1;
            }
            int leftEntityIndex = currentRowIndex * columns + leftColumnIndex;
            testButtonData001Ptr->leftEntityId = buttonEntityIds_[leftEntityIndex];

            int rightColumnIndex;
            if (currentColumnIndex == columns - 1)
            {
                // the last column goes right to the first column
                rightColumnIndex = 0;
            }
            else
            {
                rightColumnIndex = currentColumnIndex + 1;
                if (currentRowIndex == rows - 1 &&
                    currentRowIndex * columns + rightColumnIndex >= buttonEntityIds_.size())
                {
                    // if the current row is the last row and
                    // if the column to the right doesn't exist, go to the first column
                    rightColumnIndex = 0;
                }
            }
            int rightEntityIndex = currentRowIndex * columns + rightColumnIndex;
            testButtonData001Ptr->rightEntityId = buttonEntityIds_[rightEntityIndex];
        }
    }

    // Selector Entity ---------------------------------------------------------
    {
        GetComponentStoresPtr()->CreateEntity(selectorEntityId_);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(selectorEntityId_));
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, selectorEntityId_));
        if (renderedModelPtr != nullptr)
        {
            renderedModelPtr->SetCameraMask(s_mainCameraMask_);
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

            cursorPositionRenderedMeshIndex_ = renderedMeshes.size();
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& selectorMesh = renderedMeshes.back();
            selectorMesh.SetMeshDataPtr(selectorMeshDataPtr_);
            selectorMesh.SetPositionZ(0.01f);
            selectorMesh.SetColor(0.8f, 0.8f, 0.8f, 0.4f);
            selectorMesh.SetTranslucent(true);
            selectorMesh.SetUseLighting(false);
            selectorMesh.SetVisible(false);
        }
    }

    // Member Scenes -----------------------------------------------------------

    TestInstructionScene001::InitializationInfo instructionSceneInfo;
    instructionSceneInfo.hiddenInstructionString = std::string("Press <Tab> to show instructions.");
    instructionSceneInfo.instructionString = std::string(
        "This is the Main Menu Scene.\n"
        "Use <WASD> to navigate and <Space> to select.\n"
        "Alternativly use the <Mouse> and <Left-Click>.\n"
        "Press <Tab> to hide instructions."
    );
    instructionSceneInfo.fontDataPtr = font01_FontDataPtr_;
    instructionSceneInfo.fontTextureIdPtr = &font01_TextureId_;
    instructionSceneInfo.cameraEntityIdPtr = &uiCameraEntityId_;
    instructionSceneInfo.cameraMaskPtr = &s_uiCameraMask_;
    instructionSceneInfo.keyCode_toggleInstructionsPtr = &s_keyCode_toggleInstructions_;
    instructionScene_.Initialize(instructionSceneInfo);
}

void TestScene001::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    LOG_INFO("DEINITIALIZING: TestScene001:            " << GetId());

    GetRendererPtr()->DeleteAllTextures();
    GetRendererPtr()->DeleteAllMeshes();
    GetSoundPlayerPtr()->DeleteAllSoundSources();
    GetSoundPlayerPtr()->DeleteAllSoundBuffers();
    GetSoundPlayerPtr()->ResetListener();
    GetComponentStoresPtr()->DeleteAllEntities();

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

    delete selectorMeshDataPtr_;
    selectorMeshDataPtr_ = nullptr;
    selectorMeshId_ = (unsigned int)-1;

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

    selectorEntityId_ = (unsigned int)-1;

    // -------------------------------------------------------------------------

    selectedEntityId_ = (unsigned int)-1;
}

void TestScene001::ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorPositionEvent)
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

void TestScene001::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == s_keyCode_toggleInstructions_ ||
            keyCode == s_keyCode_moveUp_ ||
            keyCode == s_keyCode_moveLeft_ ||
            keyCode == s_keyCode_moveDown_ ||
            keyCode == s_keyCode_moveRight_ ||
            keyCode == s_keyCode_select_)
        {
            Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursorEntityId_));
            if (collisionBody2DPtr != nullptr)
            {
                std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
                collisionPoints[cursorPositionCollisionPointIndex_].enabled = false;
                collisionPoints[cursorPressCollisionPointIndex_].enabled = false;
                collisionPoints[cursorReleaseCollisionPointIndex_].enabled = false;
            }
        }

        if (selectedEntityId_ != (unsigned int)-1)
        {
            TestButtonData001* testButtonData001Ptr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<TestButtonData001>(testButtonData001Ptr, selectedEntityId_));
            if (testButtonData001Ptr != nullptr)
            {
                if (keyCode == s_keyCode_moveUp_)
                {
                    selectedEntityId_ = testButtonData001Ptr->aboveEntityId;

                }
                else if (keyCode == s_keyCode_moveLeft_)
                {
                    selectedEntityId_ = testButtonData001Ptr->leftEntityId;
                }
                else if (keyCode == s_keyCode_moveDown_)
                {
                    selectedEntityId_ = testButtonData001Ptr->bellowEntityId;
                }
                else if (keyCode == s_keyCode_moveRight_)
                {
                    selectedEntityId_ = testButtonData001Ptr->rightEntityId;
                }
                else if (keyCode == s_keyCode_select_)
                {
                    // I need to make a stack copy of the unsigned int since i deinitialze
                    // the scene before I'm done using it.
                    unsigned int destinationSceneId = testButtonData001Ptr->destinationSceneId;

                    SendEventToApplication(Project001::SwitchSceneEvent(destinationSceneId));
                    if (GetActiveScene()->GetId() == destinationSceneId)
                    {
                        SendEventToScene(GetId(), Project001::DeinitializeEvent());
                        SendEventToApplication(Project001::InitializeEvent());
                    }
                    return;
                }
            }
        }
        else
        {
            if (keyCode == s_keyCode_moveUp_ || keyCode == s_keyCode_moveLeft_)
            {
                selectedEntityId_ = buttonEntityIds_.back();
            }
            else if (keyCode == s_keyCode_moveDown_ || keyCode == s_keyCode_moveRight_)
            {
                selectedEntityId_ = buttonEntityIds_.front();
            }
        }
    }
}

void TestScene001::ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent)
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

void TestScene001::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    Project001::RenderSystem::Render(GetComponentStoresPtr(), GetRendererPtr());
}

void TestScene001::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    // sync cursor rendered model and collision body
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

    // calculate collisions
    // -------------------------------------------------------------------------
    Project001::CollisionSystem2D::CalculateCollisions(GetComponentStoresPtr());

    // recolor all buttons
    // -------------------------------------------------------------------------
    {
        for (size_t i = 0; i < buttonEntityIds_.size(); ++i)
        {
            const unsigned int& buttonEntityId = buttonEntityIds_[i];

            bool collidingWithCursorPosition = false;
            bool collidingWithCursorPress = false;
            bool collidingWithCursorRelease = false;

            Project001::CollisionBody2D* buttonCollisionBody2DPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(buttonCollisionBody2DPtr, buttonEntityId));
            if (buttonCollisionBody2DPtr != nullptr)
            {
                const std::vector<Project001::CollisionData2D> collisions = buttonCollisionBody2DPtr->GetCollisions();
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

            Project001::RenderedModel* buttonRenderedModelPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(buttonRenderedModelPtr, buttonEntityId));
            if (buttonRenderedModelPtr != nullptr)
            {
                std::vector<Project001::RenderedMesh>& renderedMeshes = buttonRenderedModelPtr->GetRenderedMeshes();
                Project001::RenderedMesh& buttonMesh = renderedMeshes[0];

                if (collidingWithCursorPosition)
                {
                    buttonMesh.SetColor(s_buttonColor2_);

                    selectedEntityId_ = buttonEntityId;
                }
                else
                {
                    buttonMesh.SetColor(s_buttonColor_);
                }
            }

            if (collidingWithCursorPress && collidingWithCursorRelease)
            {
                TestButtonData001* testButtonData001Ptr = nullptr;
                FAIL_CHECK(GetComponentStoresPtr()->GetComponent<TestButtonData001>(testButtonData001Ptr, buttonEntityId));
                if (testButtonData001Ptr != nullptr)
                {
                    // I need to make a stack copy of the unsigned int since i deinitialze
                    // the scene before I'm done using it.
                    unsigned int destinationSceneId = testButtonData001Ptr->destinationSceneId;

                    SendEventToApplication(Project001::SwitchSceneEvent(destinationSceneId));
                    if (GetActiveScene()->GetId() == destinationSceneId)
                    {
                        SendEventToScene(GetId(), Project001::DeinitializeEvent());
                        SendEventToApplication(Project001::InitializeEvent());
                    }
                    return;
                }
            }
        }
    }

    // place selector
    // -------------------------------------------------------------------------
    {
        Project001::RenderedModel* renderedModelPtr01;
        if (GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr01, selectedEntityId_))
        {
            Project001::RenderedModel* renderedModelPtr02;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr02, selectorEntityId_));
            renderedModelPtr02->SetPosition(renderedModelPtr01->GetPosition());
            std::vector<Project001::RenderedMesh>& renderedMeshes02 = renderedModelPtr02->GetRenderedMeshes();
            renderedMeshes02[0].SetVisible(true);
        }
    }
}

void TestScene001::UpdateCursorPosition(float xPosition, float yPosition)
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

const glm::vec4 TestScene001::s_buttonColor_ = glm::vec4(0.2f, 0.2f, 0.8f, 1.0f);
const glm::vec4 TestScene001::s_buttonColor2_ = glm::vec4(0.2f, 0.6f, 0.8f, 1.0f);