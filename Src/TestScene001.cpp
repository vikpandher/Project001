#include "TestScene001.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/CollisionBody2D.h"
#include "Engine/Components/RenderedModel.h"
#include "Engine/CollisionSystem2D.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/FreetypeTextLoader.h"
#include "Engine/Logger.h"
#include "Engine/MeshLoader.h"
#include "Engine/Renderer.h"
#include "Engine/RenderSystem.h"
#include "Engine/SoundPlayer.h"
#include "Engine/Window.h"



// public ----------------------------------------------------------------------

TestScene001::TestScene001()
    : windowPtr_(nullptr)
    , rendererPtr_(nullptr)
    , soundPlayerPtr_(nullptr)
    , componentStoresPtr_(nullptr)
    , font01_FontDataPtr_(nullptr)
    , font01_TextureDataPtr_(nullptr)
    , rectangleMeshDataPtr_(nullptr)
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

    circleMeshDataPtr_ = new Project001::MeshData();
    _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*circleMeshDataPtr_, 0.08f, 12));

    // Entity Ids --------------------------------------------------------------

    { // main camera
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

    { // coursor
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(cursorEntityId_));

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(cursorEntityId_));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(cursorEntityId_, renderedModelPtr));
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        cursorPositionRenderedMeshIndex_ = renderedMeshes.size();
        renderedMeshes.emplace_back();
        Project001::RenderedMesh& circleMesh01 = renderedMeshes.back();
        circleMesh01.SetMeshDataPtr(circleMeshDataPtr_);
        circleMesh01.SetPositionZ(0.53f);
        circleMesh01.SetColor(0.8f, 0.8f, 0.8f, 0.4f);
        circleMesh01.SetTranslucent(true);
        circleMesh01.SetLit(false);
        circleMesh01.SetVisible(false);

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
        collisionPoints.emplace_back(glm::vec3(), cursorPositionCollisionShapeId_, false);
        cursorPressCollisionPointIndex_ = collisionPoints.size();
        collisionPoints.emplace_back(glm::vec3(), cursorPressCollisionShapeId_, false);
        cursorReleaseCollisionPointIndex_ = collisionPoints.size();
        collisionPoints.emplace_back(glm::vec3(), cursorReleaseCollisionShapeId_, false);
    }

    // buttons
    // -------------------------------------------------------------------------

    std::vector<glm::vec3> buttonPositions;
    float xPos = -2.615f;
    float yPos = 2.0f;
    for (size_t i = 0; i < 4; ++i)
    {
        for (size_t j = 0; j < 5; ++j)
        {
            buttonPositions.emplace_back(xPos, yPos, 0.0f);
            yPos -= 1.0f;
        }
        yPos = 2.0f;
        xPos += 1.75f;
    }

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
        unsigned int newButtonEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(newButtonEntityId));

        Button001& newButton = buttons_[newButtonEntityId];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(newButtonEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(newButtonEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(buttonPositions[i]);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        renderedMeshes.emplace_back();
        Project001::RenderedMesh& buttonMesh = renderedMeshes.back();
        buttonMesh.SetMeshId(rectangularMeshId_);
        buttonMesh.SetMaxRadius(rectangleMeshDataPtr_->maxRadius);
        buttonMesh.SetColor(0.2f, 0.2f, 0.8f, 1.0f);
        buttonMesh.SetLit(false);

        newButton.textString_ = buttonStrings[i].c_str();
        newButton.textMeshDataPtr_ = new Project001::MeshData();
        _FAIL_CHECK(Project001::FreetypeTextLoader::LoadMesh(
            *newButton.textMeshDataPtr_,
            *font01_FontDataPtr_,
            newButton.textString_,
            fontPixelSize_
        ));
        Project001::MeshLoader::RecenterMesh(*newButton.textMeshDataPtr_);

        renderedMeshes.emplace_back();
        Project001::RenderedMesh& textMesh = renderedMeshes.back();
        textMesh.SetMeshDataPtr(newButton.textMeshDataPtr_);
        textMesh.SetTextureId(font01_TextureId_);
        textMesh.SetPositionZ(0.01f);
        textMesh.SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(newButtonEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(newButtonEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(buttonPositions[i]);
        std::vector<Project001::CollisionRectangle2D>& collisionRectangles = collisionBody2DPtr->GetCollisionRectangles();
        collisionRectangles.emplace_back(
            glm::vec2(-0.5f * buttonRectangleWidth, -0.5f * buttonRectangleHeight),
            glm::vec2(0.5f * buttonRectangleWidth, 0.5f * buttonRectangleHeight),
            buttonCollisionShapeId_,
            true
        );
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

    delete circleMeshDataPtr_;
    circleMeshDataPtr_ = nullptr;

    // Entity Ids --------------------------------------------------------------

    mainCameraEntityId_ = (unsigned int)-1;
    cursorEntityId_ = (unsigned int)-1;

    cursorPositionRenderedMeshIndex_ = 0;
    cursorPressRenderedMeshIndex_ = 0;
    cursorReleaseRenderedMeshIndex_ = 0;

    cursorPositionCollisionPointIndex_ = 0;
    cursorPressCollisionPointIndex_ = 0;
    cursorReleaseCollisionPointIndex_ = 0;

    // -------------------------------------------------------------------------

    previousWorldCursorPosition_ = glm::vec2(0.0f, 0.0f);
    previousWorldCursorPress_ = glm::vec2(0.0f, 0.0f);
    previousWorldCursorRelease_ = glm::vec2(0.0f, 0.0f);

    for (size_t i = 0; i < buttons_.size(); ++i)
    {
        delete buttons_[i].textMeshDataPtr_;
    }
    buttons_.clear();
}

void TestScene001::ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorPositionEvent)
{
    int windowWidth, windowHeight;
    windowPtr_->GetWindowSize(windowWidth, windowHeight);

    unsigned int xOffset, yOffset, viewportWidth, viewportHeight;
    rendererPtr_->GetViewport(xOffset, yOffset, viewportWidth, viewportHeight);

    // Convert coordinates from window to viewport
    glm::vec2 viewportCursorPosition(
        cursorPositionEvent.xPosition - xOffset,
        windowHeight - yOffset - cursorPositionEvent.yPosition
    );

    if (viewportCursorPosition.x < viewportWidth || viewportCursorPosition.y < viewportHeight)
    {
        Project001::Camera* cameraPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));

        // Convert coordinates from viewport to world
        glm::vec2 worldCursorPosition = cameraPtr->ConvertPointFromViewportToOrthoWorld(
            viewportWidth,
            viewportHeight,
            viewportCursorPosition
        );

        previousWorldCursorPosition_ = worldCursorPosition;

        // ---------------------------------------------------------------------

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(cursorEntityId_, renderedModelPtr));
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
        renderedMeshes[cursorPositionRenderedMeshIndex_].SetVisible(true);

        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(cursorEntityId_, collisionBody2DPtr));
        std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
        collisionPoints[cursorPositionCollisionPointIndex_].tangible_ = true;
    }

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
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
            renderedMeshes[cursorPressRenderedMeshIndex_].SetVisible(true);

            Project001::CollisionBody2D* collisionBody2DPtr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(cursorEntityId_, collisionBody2DPtr));
            std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
            collisionPoints[cursorPressCollisionPointIndex_].tangible_ = true;

            previousWorldCursorPress_ = previousWorldCursorPosition_;
        }
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
        {
            Project001::RenderedModel* renderedModelPtr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(cursorEntityId_, renderedModelPtr));
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
            renderedMeshes[cursorReleaseRenderedMeshIndex_].SetVisible(true);

            Project001::CollisionBody2D* collisionBody2DPtr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(cursorEntityId_, collisionBody2DPtr));
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
    { // sync cursor rendered model and collision body
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

    Project001::CollisionSystem2D::CalculateCollisions(componentStoresPtr_);

    { // recolor all buttons

        std::map<unsigned int, Button001>::iterator iter;
        for (iter = buttons_.begin(); iter != buttons_.end(); ++iter)
        {
            const unsigned int& buttonEntityId = iter->first;

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
            }
            else
            {
                buttonMesh.SetColor(0.2f, 0.2f, 0.8f, 1.0f);
            }

            if (collidingWithCursorPress && collidingWithCursorRelease)
            {
                const Button001& buttonStruct = iter->second;
                std::string destinationSceneName = buttonStruct.textString_;

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

    updateEvent.handled = true;
}