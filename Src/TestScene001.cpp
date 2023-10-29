#include "TestScene001.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/CollisionBody2D.h"
#include "Engine/Components/RenderedModel.h"
#include "Engine/CollisionSystem2D.h"
#include "Engine/ComponentStores.h"
#include "Engine/FreetypeTextLoader.h"
#include "Engine/Logger.h"
#include "Engine/MeshLoader.h"
#include "Engine/Renderer.h"
#include "Engine/RenderSystem.h"
#include "Engine/SoundPlayer.h"
#include "Engine/Window.h"

#include "TestButtonInfo001.h"



// public ----------------------------------------------------------------------

TestScene001::TestScene001()
    : windowPtr_(nullptr)
    , rendererPtr_(nullptr)
    , soundPlayerPtr_(nullptr)
    , componentStoresPtr_(nullptr)
    , font01_FontDataPtr_(nullptr)
    , font01_TextureDataPtr_(nullptr)
    , rectangleMeshDataPtr_(nullptr)
    , selectorMeshDataPtr_(nullptr)
    , circleMeshDataPtr_(nullptr)
{
    ClearResources();
}

TestScene001::~TestScene001()
{}

const char* TestScene001::Name()
{
    return "TestScene001";
}

// protected -------------------------------------------------------------------

bool TestScene001::OnInitialize()
{
    _LOG_MESSAGE("INITIALIZING: %s", Name());

    windowPtr_ = GetApplicationWindowPtr();

    rendererPtr_ = GetApplicationRendererPtr();

    soundPlayerPtr_ = GetApplicationSoundPlayerPtr();

    componentStoresPtr_ = GetApplicaitonComponentStoresPtr();

    // Font Data ---------------------------------------------------------------

    {
        font01_FontDataPtr_ = new Project001::FontData;
        font01_TextureDataPtr_ = new Project001::TextureData;
        std::vector<unsigned char> characterList;
        for (unsigned char c = 32; c < 127; ++c) // ASCII characters
        {
            characterList.push_back(c);
        }
        _FAIL_CHECK(Project001::FreetypeTextLoader::LoadTexture(
            *font01_TextureDataPtr_,
            *font01_FontDataPtr_,
            characterList,
            "../Fonts/Antonio-Regular.ttf",
            64
        ));
        rendererPtr_->CreateTexture(
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
    _FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
        *rectangleMeshDataPtr_,
        buttonRectangleWidth,
        buttonRectangleHeight,
        0.0f,
        1.0f,
        0.0f,
        1.0f
    ));
    rendererPtr_->CreateMesh(
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

    rendererPtr_->CreateMesh(
        selectorMeshId_,
        selectorMeshDataPtr_->meshVertexArray.data(),
        (unsigned int)selectorMeshDataPtr_->meshVertexArray.size(),
        selectorMeshDataPtr_->meshIndexArray.data(),
        (unsigned int)selectorMeshDataPtr_->meshIndexArray.size()
    );

    circleMeshDataPtr_ = new Project001::MeshData();
    _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*circleMeshDataPtr_, 0.08f, 12));

    // Text Mesh Data ----------------------------------------------------------

    std::vector<std::string> buttonStrings;
    buttonStrings.emplace_back("TestScene002");
    buttonStrings.emplace_back("TestScene003");
    buttonStrings.emplace_back("TestScene004");
    buttonStrings.emplace_back("TestScene006");
    buttonStrings.emplace_back("TestScene007");
    buttonStrings.emplace_back("TestScene010");
    buttonStrings.emplace_back("TestScene011");
    buttonStrings.emplace_back("TestScene012");
    buttonStrings.emplace_back("TestScene013");
    buttonStrings.emplace_back("TestScene030");
    buttonStrings.emplace_back("TestScene031");
    buttonStrings.emplace_back("TestScene032");
    buttonStrings.emplace_back("TestScene033");
    buttonStrings.emplace_back("TestScene034");
    buttonStrings.emplace_back("TestScene050");

    for (size_t i = 0; i < buttonStrings.size(); ++i)
    {
        Project001::MeshData* newTextMeshDataPtr = new Project001::MeshData();
        _FAIL_CHECK(Project001::FreetypeTextLoader::LoadMesh(
            *newTextMeshDataPtr,
            *font01_FontDataPtr_,
            buttonStrings[i],
            fontPixelSize_
        ));
        Project001::MeshLoader::RecenterMesh(*newTextMeshDataPtr);
        textMeshDataPtrs_.push_back(newTextMeshDataPtr);
    }

    // Entity Ids --------------------------------------------------------------

    // Main Camera Entity ------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(mainCameraEntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::Camera>(mainCameraEntityId_));

        Project001::Camera* cameraPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));
        int aspectRatioNumerator;
        int aspectRatioDenominator;
        windowPtr_->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);
        if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
        {
            float aspectRatio = (float)aspectRatioNumerator / (float)aspectRatioDenominator;
            float secondaryCameraHalfHeight = 2.75f;
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
        cameraPtr->SetProjectionToOrthographic();
        cameraPtr->TurnOn();
    }

    // Cursor Entity -----------------------------------------------------------
    {
        float cursorX_position;
        float cursorY_position;
        windowPtr_->GetCursorPosition(cursorX_position, cursorY_position);
        UpdatePreviousWorldCursorPosition(cursorX_position, cursorY_position);

        _FAIL_CHECK(componentStoresPtr_->CreateEntity(cursorEntityId_));

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(cursorEntityId_));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(cursorEntityId_, renderedModelPtr));
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        float xPosition;
        float yPosition;
        windowPtr_->GetCursorPosition(xPosition, yPosition);

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

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(cursorEntityId_));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(cursorEntityId_, collisionBody2DPtr));
        std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
        cursorPositionCollisionPointIndex_ = collisionPoints.size();
        collisionPoints.emplace_back(glm::vec2(), cursorPositionCollisionShapeId_, true);
        cursorPressCollisionPointIndex_ = collisionPoints.size();
        collisionPoints.emplace_back(glm::vec2(), cursorPressCollisionShapeId_, false);
        cursorReleaseCollisionPointIndex_ = collisionPoints.size();
        collisionPoints.emplace_back(glm::vec2(), cursorReleaseCollisionShapeId_, false);
    }

    // Button Entities
    // -------------------------------------------------------------------------

    // Button Indicies:
    // 
    // [ 000 ] [ 001 ] [ 002 ] [ 003 ]
    // [ 004 ] [ 005 ] [ 006 ] [ 007 ]
    // [ 008 ] [ 009 ] [ 010 ] [ 011 ]
    // [ 012 ] [ 013 ] [ 014 ]
    // 
    // Test Scenes:
    // 
    // [ 002 ] [ 003 ] [ 004 ] [ 006 ]
    // [ 007 ] [ 010 ] [ 011 ] [ 012 ]
    // [ 013 ] [ 030 ] [ 031 ] [ 032 ]
    // [ 033 ] [ 034 ] [ 050 ]

    size_t columns = 4;
    size_t rows = buttonStrings.size() / columns + 1;
    std::vector<glm::vec3> buttonPositions;
    float xPos = -2.615f;
    float yPos = 2.0f;
    for (size_t y = 0; y < rows; ++y)
    {
        for (size_t x = 0; x < columns; ++x)
        {
            buttonPositions.emplace_back(xPos, yPos, 0.0f);
            xPos += 1.75f;
        }
        xPos = -2.615f;
        yPos -= 1.0f;
    }

    for (size_t i = 0; i < buttonStrings.size(); ++i)
    {
        unsigned int buttonEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(buttonEntityId));

        buttonEntityIds_.push_back(buttonEntityId);

        // ---------------------------------------------------------------------

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(buttonEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(buttonEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(buttonPositions[i]);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        renderedMeshes.emplace_back();
        Project001::RenderedMesh& buttonMesh = renderedMeshes.back();
        buttonMesh.SetMeshId(rectangularMeshId_);
        buttonMesh.SetMaxBoundingRadius(rectangleMeshDataPtr_->maxBoundingRadius);
        buttonMesh.SetColor(0.2f, 0.2f, 0.8f, 1.0f);
        buttonMesh.SetLit(false);

        renderedMeshes.emplace_back();
        Project001::RenderedMesh& textMesh = renderedMeshes.back();
        textMesh.SetMeshDataPtr(textMeshDataPtrs_[i]);
        textMesh.SetTextureId(font01_TextureId_);
        textMesh.SetPositionZ(0.01f);
        textMesh.SetLit(false);

        // ---------------------------------------------------------------------

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(buttonEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(buttonEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(buttonPositions[i]);
        std::vector<Project001::CollisionRectangle2D>& collisionRectangles = collisionBody2DPtr->GetCollisionRectangles();
        collisionRectangles.emplace_back(
            glm::vec2(-0.5f * buttonRectangleWidth, -0.5f * buttonRectangleHeight),
            glm::vec2(0.5f * buttonRectangleWidth, 0.5f * buttonRectangleHeight)
        );
    }

    for (size_t i = 0; i < buttonEntityIds_.size(); ++i)
    {
        const unsigned int& buttonEntityId = buttonEntityIds_[i];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<TestButtonInfo001>(buttonEntityId));
        TestButtonInfo001* testButtonInfo001Ptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<TestButtonInfo001>(buttonEntityId, testButtonInfo001Ptr));
        testButtonInfo001Ptr->textString_ = buttonStrings[i];

        // columnIndex, rowIndex
        // [ 0,0 ] [ 1,0 ] [ 2,0 ] [ 3,0 ]
        // [ 0,1 ] [ 1,1 ] [ 2,1 ] [ 3,1 ]
        // [ 0,2 ] [ 1,2 ] [ 2,2 ] [ 3,2 ]
        // [ 0,3 ] [ 1,3 ] [ 2,3 ]

        int currentColumnIndex = i % columns;
        int currentRowIndex = i / columns;

        int aboveRowIndex;
        if (currentRowIndex == 0)
        {
            // first row goes up to the last row
            aboveRowIndex = rows - 1;
            if (aboveRowIndex * columns + currentColumnIndex == buttonEntityIds_.size())
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
        testButtonInfo001Ptr->aboveEntityId_ = buttonEntityIds_[aboveEntityIndex];

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
                bellowRowIndex * columns + currentColumnIndex == buttonEntityIds_.size())
            {
                // if the bellow row is the last row and
                // if the bellow row spot doesn't exist, go to the first row
                bellowRowIndex = 0;
            }
        }
        int bellowEntityIndex = bellowRowIndex * columns + currentColumnIndex;
        testButtonInfo001Ptr->bellowEntityId_ = buttonEntityIds_[bellowEntityIndex];

        int leftColumnIndex;
        if (currentColumnIndex == 0)
        {
            // first column goes left to the last column
            leftColumnIndex = columns - 1;
            if (currentRowIndex == rows - 1 &&
                currentRowIndex * columns + leftColumnIndex == buttonEntityIds_.size())
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
        testButtonInfo001Ptr->leftEntityId_ = buttonEntityIds_[leftEntityIndex];

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
                currentRowIndex * columns + rightColumnIndex == buttonEntityIds_.size())
            {
                // if the current row is the last row and
                // if the column to the right doesn't exist, go to the first column
                rightColumnIndex = 0;
            }
        }
        int rightEntityIndex = currentRowIndex * columns + rightColumnIndex;
        testButtonInfo001Ptr->rightEntityId_ = buttonEntityIds_[rightEntityIndex];
    }

    // Selector Entity ---------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(selectorEntityId_));

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(selectorEntityId_));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(selectorEntityId_, renderedModelPtr));
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        cursorPositionRenderedMeshIndex_ = renderedMeshes.size();
        renderedMeshes.emplace_back();
        Project001::RenderedMesh& selectorMesh = renderedMeshes.back();
        selectorMesh.SetMeshDataPtr(selectorMeshDataPtr_);
        selectorMesh.SetPositionZ(0.01f);
        selectorMesh.SetColor(0.8f, 0.8f, 0.8f, 0.4f);
        selectorMesh.SetTranslucent(true);
        selectorMesh.SetLit(false);
        selectorMesh.SetVisible(false);
    }

    return true;
}

bool TestScene001::OnDeinitialize()
{
    ClearResources();
    rendererPtr_->DeleteAllTextures();
    rendererPtr_->DeleteAllMeshes();
    soundPlayerPtr_->DeleteAllSoundSources();
    soundPlayerPtr_->DeleteAllSoundBuffers();
    componentStoresPtr_->DeleteAllEntities();

    return true;
}

void TestScene001::OnHandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::CursorPositionEvent>(event, std::bind(&TestScene001::ProcessCursorPositionEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::FrameBufferSizeEvent>(event, std::bind(&TestScene001::ProcessFrameBufferSizeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene001::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::MouseButtonEvent>(event, std::bind(&TestScene001::ProcessMouseButtonEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestScene001::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene001::ProcessUpdateEvent, this, std::placeholders::_1));
}

void TestScene001::ClearResources()
{
    // Font Data ---------------------------------------------------------------

    delete font01_FontDataPtr_;
    delete font01_TextureDataPtr_;
    font01_TextureId_ = (unsigned int)-1;

    // Mesh Data ---------------------------------------------------------------

    delete rectangleMeshDataPtr_;
    rectangleMeshDataPtr_ = nullptr;
    rectangularMeshId_ = (unsigned int)-1;

    delete selectorMeshDataPtr_;
    selectorMeshDataPtr_ = nullptr;
    selectorMeshId_ = (unsigned int)-1;

    delete circleMeshDataPtr_;
    circleMeshDataPtr_ = nullptr;

    for (size_t i = 0; i < textMeshDataPtrs_.size(); ++i)
    {
        delete textMeshDataPtrs_[i];
    }
    textMeshDataPtrs_.clear();

    // Entity Ids --------------------------------------------------------------

    mainCameraEntityId_ = (unsigned int)-1;
    cursorEntityId_ = (unsigned int)-1;

    cursorPositionRenderedMeshIndex_ = 0;
    cursorPressRenderedMeshIndex_ = 0;
    cursorReleaseRenderedMeshIndex_ = 0;

    cursorPositionCollisionPointIndex_ = 0;
    cursorPressCollisionPointIndex_ = 0;
    cursorReleaseCollisionPointIndex_ = 0;

    buttonEntityIds_.clear();

    selectorEntityId_ = (unsigned int)-1;

    // -------------------------------------------------------------------------

    previousWorldCursorPosition_ = glm::vec2(0.0f, 0.0f);
    previousWorldCursorPress_ = glm::vec2(0.0f, 0.0f);
    previousWorldCursorRelease_ = glm::vec2(0.0f, 0.0f);

    selectedEntityId_ = (unsigned int)-1;
}

void TestScene001::ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorPositionEvent)
{
    UpdatePreviousWorldCursorPosition(cursorPositionEvent.xPosition, cursorPositionEvent.yPosition);

    Project001::RenderedModel* renderedModelPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(cursorEntityId_, renderedModelPtr));
    renderedModelPtr->SetVisible(true);

    Project001::CollisionBody2D* collisionBody2DPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(cursorEntityId_, collisionBody2DPtr));
    collisionBody2DPtr->SetTangible(true);

    cursorPositionEvent.handled = true;
}

void TestScene001::ProcessFrameBufferSizeEvent(Project001::FrameBufferSizeEvent& frameBufferSizeEvent)
{
    const int& height = frameBufferSizeEvent.height;
    const int& width = frameBufferSizeEvent.width;

    int aspectRatioNumerator, aspectRatioDenominator;
    windowPtr_->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);

    if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
    {
        float aspectRatio = (float)aspectRatioNumerator / (float)aspectRatioDenominator;

        int adjustedHeight = (int)(width / aspectRatio);
        int adjustedWidth = (int)(height * aspectRatio);

        if (adjustedWidth > width)
        {
            adjustedWidth = width;
        }

        if (adjustedHeight > height)
        {
            adjustedHeight = height;
        }

        int lowerLeftX = (width - adjustedWidth) / 2;
        int lowerLeftY = (height - adjustedHeight) / 2;

        rendererPtr_->SetFramebufferSize(adjustedWidth, adjustedHeight);
        rendererPtr_->SetViewport(lowerLeftX, lowerLeftY, adjustedWidth, adjustedHeight);
    }
    else
    {
        rendererPtr_->SetFramebufferSize(width, height);
        rendererPtr_->SetViewport(0, 0, width, height);
    }

    frameBufferSizeEvent.handled = true;
}

void TestScene001::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == s_keyCodeMoveUp_ ||
            keyCode == s_keyCodeMoveLeft_ ||
            keyCode == s_keyCodeMoveDown_ ||
            keyCode == s_keyCodeMoveRight_ ||
            keyCode == s_keyCodeSelect_)
        {
            Project001::RenderedModel* renderedModelPtr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(cursorEntityId_, renderedModelPtr));
            renderedModelPtr->SetVisible(false);
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
            renderedMeshes[cursorPressRenderedMeshIndex_].SetVisible(false);
            renderedMeshes[cursorReleaseRenderedMeshIndex_].SetVisible(false);

            Project001::CollisionBody2D* collisionBody2DPtr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(cursorEntityId_, collisionBody2DPtr));
            collisionBody2DPtr->SetTangible(false);
            std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
            collisionPoints[cursorPressCollisionPointIndex_].tangible_ = false;
            collisionPoints[cursorReleaseCollisionPointIndex_].tangible_ = false;
        }

        if (selectedEntityId_ != (unsigned int)-1)
        {
            TestButtonInfo001* testButtonInfo001Ptr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<TestButtonInfo001>(selectedEntityId_, testButtonInfo001Ptr));

            if (keyCode == s_keyCodeMoveUp_)
            {
                selectedEntityId_ = testButtonInfo001Ptr->aboveEntityId_;

            }
            else if (keyCode == s_keyCodeMoveLeft_)
            {
                selectedEntityId_ = testButtonInfo001Ptr->leftEntityId_;
            }
            else if (keyCode == s_keyCodeMoveDown_)
            {
                selectedEntityId_ = testButtonInfo001Ptr->bellowEntityId_;
            }
            else if (keyCode == s_keyCodeMoveRight_)
            {
                selectedEntityId_ = testButtonInfo001Ptr->rightEntityId_;
            }
            else if (keyCode == s_keyCodeSelect_)
            {
                TestButtonInfo001* testButtonInfo001Ptr;
                _FAIL_CHECK(componentStoresPtr_->GetComponent<TestButtonInfo001>(selectedEntityId_, testButtonInfo001Ptr));

                // I need to make a stack copy of the string since i deinitialze
                // the scene before I'm done using it.
                std::string destinationSceneName = testButtonInfo001Ptr->textString_;

                SendEvent(Project001::SwitchSceneEvent(destinationSceneName));
                if (!IsActiveScene())
                {
                    Deinitialize();
                    SendEvent(Project001::InitializeSceneEvent(destinationSceneName));
                }
            }
        }
        else
        {
            if (keyCode == s_keyCodeMoveUp_ || keyCode == s_keyCodeMoveLeft_)
            {
                selectedEntityId_ = buttonEntityIds_.back();
            }
            else if (keyCode == s_keyCodeMoveDown_ || keyCode == s_keyCodeMoveRight_)
            {
                selectedEntityId_ = buttonEntityIds_.front();
            }
        }
    }

    keyEvent.handled = true;
}

void TestScene001::ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent)
{
    Project001::MouseButton& mouseButton = mouseButtonEvent.mouseButton;
    Project001::ButtonAction& buttonAction = mouseButtonEvent.buttonAction;

    if (mouseButton == Project001::MouseButton::MOUSE_BUTTON_1)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS)
        {
            Project001::RenderedModel* renderedModelPtr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(cursorEntityId_, renderedModelPtr));
            renderedModelPtr->SetVisible(true);
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
            renderedMeshes[cursorPressRenderedMeshIndex_].SetVisible(true);
            renderedMeshes[cursorReleaseRenderedMeshIndex_].SetVisible(false);

            Project001::CollisionBody2D* collisionBody2DPtr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(cursorEntityId_, collisionBody2DPtr));
            collisionBody2DPtr->SetTangible(true);
            std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
            collisionPoints[cursorPressCollisionPointIndex_].tangible_ = true;
            collisionPoints[cursorReleaseCollisionPointIndex_].tangible_ = false;

            previousWorldCursorPress_ = previousWorldCursorPosition_;
        }
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
        {
            Project001::RenderedModel* renderedModelPtr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(cursorEntityId_, renderedModelPtr));
            renderedModelPtr->SetVisible(true);
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
            renderedMeshes[cursorReleaseRenderedMeshIndex_].SetVisible(true);

            Project001::CollisionBody2D* collisionBody2DPtr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(cursorEntityId_, collisionBody2DPtr));
            collisionBody2DPtr->SetTangible(true);
            std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
            collisionPoints[cursorReleaseCollisionPointIndex_].tangible_ = true;

            previousWorldCursorRelease_ = previousWorldCursorPosition_;
        }
    }

    mouseButtonEvent.handled = true;
}

void TestScene001::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    Project001::RenderSystem::Render(componentStoresPtr_, rendererPtr_);

    renderEvent.handled = true;
}

void TestScene001::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    // sync cursor rendered model and collision body
    // -------------------------------------------------------------------------
    {
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(cursorEntityId_, renderedModelPtr));
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

        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(cursorEntityId_, collisionBody2DPtr));
        std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();

        collisionPoints[cursorPositionCollisionPointIndex_].position.x = previousWorldCursorPosition_.x;
        collisionPoints[cursorPositionCollisionPointIndex_].position.y = previousWorldCursorPosition_.y;

        collisionPoints[cursorPressCollisionPointIndex_].position.x = previousWorldCursorPress_.x;
        collisionPoints[cursorPressCollisionPointIndex_].position.y = previousWorldCursorPress_.y;

        collisionPoints[cursorReleaseCollisionPointIndex_].position.x = previousWorldCursorRelease_.x;
        collisionPoints[cursorReleaseCollisionPointIndex_].position.y = previousWorldCursorRelease_.y;
    }

    // calculate collisions
    // -------------------------------------------------------------------------
    Project001::CollisionSystem2D::CalculateCollisions(componentStoresPtr_);

    // recolor all buttons
    // -------------------------------------------------------------------------
    {
        for (size_t i = 0; i < buttonEntityIds_.size(); ++i)
        {
            const unsigned int& buttonEntityId = buttonEntityIds_[i];

            bool collidingWithCursorPosition = false;
            bool collidingWithCursorPress = false;
            bool collidingWithCursorRelease = false;

            Project001::CollisionBody2D* collisionBody2DPtr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(buttonEntityId, collisionBody2DPtr));
            const std::vector<Project001::CollisionData> collisions = collisionBody2DPtr->GetCollisions();
            for (size_t i = 0; i < collisions.size(); ++i)
            {
                const Project001::CollisionData& collisionData = collisions[i];

                if (collisionData.otherEntityId == cursorEntityId_)
                {
                    if (collisionData.otherShapeId == cursorPositionCollisionShapeId_)
                    {
                        collidingWithCursorPosition = true;
                    }
                    if (collisionData.otherShapeId == cursorPressCollisionShapeId_)
                    {
                        collidingWithCursorPress = true;
                    }
                    if (collisionData.otherShapeId == cursorReleaseCollisionShapeId_)
                    {
                        collidingWithCursorRelease = true;
                    }
                }
            }

            Project001::RenderedModel* renderedModelPtr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(buttonEntityId, renderedModelPtr));
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
            Project001::RenderedMesh& buttonMesh = renderedMeshes[0];

            if (collidingWithCursorPosition)
            {
                buttonMesh.SetColor(0.2f, 0.6f, 0.8f, 1.0f);

                selectedEntityId_ = buttonEntityId;
            }
            else
            {
                buttonMesh.SetColor(0.2f, 0.2f, 0.8f, 1.0f);
            }

            if (collidingWithCursorPress && collidingWithCursorRelease)
            {
                TestButtonInfo001* testButtonInfo001Ptr;
                _FAIL_CHECK(componentStoresPtr_->GetComponent<TestButtonInfo001>(buttonEntityId, testButtonInfo001Ptr));

                // I need to make a stack copy of the string since i deinitialze
                // the scene before I'm done using it.
                std::string destinationSceneName = testButtonInfo001Ptr->textString_;

                SendEvent(Project001::SwitchSceneEvent(destinationSceneName));
                if (!IsActiveScene())
                {
                    Deinitialize();
                    SendEvent(Project001::InitializeSceneEvent(destinationSceneName));
                }
                break;
            }
        }
    }

    // place selector
    // -------------------------------------------------------------------------
    {
        Project001::RenderedModel* renderedModelPtr01;
        if (componentStoresPtr_->GetComponent<Project001::RenderedModel>(selectedEntityId_, renderedModelPtr01))
        {
            Project001::RenderedModel* renderedModelPtr02;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(selectorEntityId_, renderedModelPtr02));
            renderedModelPtr02->SetPosition(renderedModelPtr01->GetPosition());
            std::vector<Project001::RenderedMesh>& renderedMeshes02 = renderedModelPtr02->GetRenderedMeshes();
            renderedMeshes02[0].SetVisible(true);
        }
    }

    updateEvent.handled = true;
}

void TestScene001::UpdatePreviousWorldCursorPosition(float xPosition, float yPosition)
{
    int windowWidth, windowHeight;
    windowPtr_->GetWindowSize(windowWidth, windowHeight);

    unsigned int xOffset, yOffset, viewportWidth, viewportHeight;
    rendererPtr_->GetViewport(xOffset, yOffset, viewportWidth, viewportHeight);

    // Convert coordinates from window to viewport
    glm::vec2 viewportCursorPosition(
        xPosition - xOffset,
        windowHeight - yOffset - yPosition
    );

    if (viewportCursorPosition.x < viewportWidth || viewportCursorPosition.y < viewportHeight)
    {
        Project001::Camera* cameraPtr;
        if (componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr))
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