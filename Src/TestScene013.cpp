#include "TestScene013.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/CollisionBody.h"
#include "Engine/Components/RenderedModel.h"
#include "Engine/Math/Overlap3D_UnitTests.h"
#include "Engine/Math/CoordinateSystems.h"
#include "Engine/Math/VectorUtilities.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/MeshLoader.h"
#include "Engine/Renderer.h"
#include "Engine/Window.h"



// public: ---------------------------------------------------------------------

TestScene013::TestScene013()
    : cursorGrabbingEntity_(false)
    , previousWorldCursorDownPosition_(0.0f, 0.0f)
{
    componentStoresPtr_ = new Project001::ComponentStores();
    ClearResources();

    Run_UnitTests();
}

TestScene013::~TestScene013()
{
    delete componentStoresPtr_;
}

const char* TestScene013::Name()
{
    return "TestScene013";
}

void TestScene013::Initialize()
{
    _LOG_MESSAGE("INITIALIZING: %s", Name());

    windowPtr_ = GetApplicationWindowPtr();

    rendererPtr_ = GetApplicationRendererPtr();
    rendererPtr_->SetDepthTesting(true);

    // main camera entity
    // -------------------------------------------------------------------------
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
            cameraPtr->SetAspectRatio(aspectRatio);
        }
        cameraPtr->SetPosition(0.0f, 0.0f, 5.0f);
        cameraPtr->AddYaw(glm::pi<float>());
        cameraPtr->TurnOn();
    }

    // Calculating positions
    // -------------------------------------------------------------------------

    std::vector<glm::vec3> modelEntityPositions;
    for (int i = 1; i >= -1; --i)
    {
        for (int j = -2; j <= 2; ++j)
        {
            modelEntityPositions.emplace_back((float)j, (float)i, 0.0f);
        }
    }
    size_t positionPosition = 0;

    // -------------------------------------------------------------------------

    // rectangle 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateBox(*newMeshDataPtr, 0.48f, 0.32f, 0.24f, false));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);
    }
}

void TestScene013::Deinitialize()
{
    componentStoresPtr_->DeleteAllEntities();
    rendererPtr_->DeleteAllTextures();

    ClearResources();
}

void TestScene013::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::CursorPositionEvent>(event, std::bind(&TestScene013::ProcessCursorPositionEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::FrameBufferSizeEvent>(event, std::bind(&TestScene013::ProcessFrameBufferSizeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene013::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::MouseButtonEvent>(event, std::bind(&TestScene013::ProcessMouseButtonEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestScene013::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::ScrollEvent>(event, std::bind(&TestScene013::ProcessScrollEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene013::ProcessUpdateEvent, this, std::placeholders::_1));
}

// protected: ------------------------------------------------------------------

void TestScene013::ClearResources()
{
    selectedEntityIdIndex_ = (unsigned int)-1;

    for (size_t i = 0; i < meshDataPtrArray_.size(); ++i)
    {
        delete meshDataPtrArray_[i];
    }
    meshDataPtrArray_.clear();

    mainCameraEntityId_ = (unsigned int)-1;
    entityIds_.clear();
}

void TestScene013::ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorButtonEvent)
{
    bool mouseButton1Pressed = windowPtr_->GetMouseButtonPressed(Project001::MouseButton::MOUSE_BUTTON_1);

    if (mouseButton1Pressed)
    {
        glm::vec2 currentPosition(cursorButtonEvent.xPosition, cursorButtonEvent.yPosition);

        Project001::Camera* cameraPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));
        // int windowWidth, windowHeight;
        // windowPtr_->GetWindowSize(windowWidth, windowHeight);
        // currentPosition = cameraPtr->ConvertPointFromWindowToOrtho(windowWidth, windowHeight, currentPosition);

        float speedConstant = 0.005f;

        float xOffset = currentPosition.x - previousWorldCursorDownPosition_.x;
        float yOffset = currentPosition.y - previousWorldCursorDownPosition_.y;

        // moving cursor right = positive xOffset
        // moving cursor up = positive yOffset

        float cameraYaw = -1.0f * xOffset * speedConstant;
        float cameraPitch = yOffset * speedConstant;

        cameraPtr->AddYaw(cameraYaw);
        // cameraPtr->AddWorldRotationY(cameraYaw); // for fps camera
        cameraPtr->AddPitch(cameraPitch);

        previousWorldCursorDownPosition_.x = currentPosition.x;
        previousWorldCursorDownPosition_.y = currentPosition.y;
    }

    cursorButtonEvent.handled = true;
}

void TestScene013::ProcessFrameBufferSizeEvent(Project001::FrameBufferSizeEvent& frameBufferSizeEvent)
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

void TestScene013::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_X)
        {
            SendEvent(Project001::SwitchSceneEvent("TestScene020"));
            if (!IsActiveScene())
            {
                Deinitialize();
                SendEvent(Project001::InitializeSceneEvent("TestScene020"));
            }
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
    }
}

void TestScene013::ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent)
{
    Project001::MouseButton& mouseButton = mouseButtonEvent.mouseButton;
    Project001::ButtonAction& buttonAction = mouseButtonEvent.buttonAction;

    cursorGrabbingEntity_ = false;

    if (mouseButton == Project001::MouseButton::MOUSE_BUTTON_1 &&
        buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS)
    {
        selectedEntityIdIndex_ = (unsigned int)-1;

        windowPtr_->GetCursorPosition(previousWorldCursorDownPosition_.x, previousWorldCursorDownPosition_.y);

        // Project001::Camera* cameraPtr;
        // _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));
        // int windowWidth, windowHeight;
        // windowPtr_->GetWindowSize(windowWidth, windowHeight);
        //previousCursorPosition_ = cameraPtr->ConvertPointFromWindowToOrtho(windowWidth, windowHeight, previousCursorPosition_);
    }
}

void TestScene013::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    rendererPtr_->ClearDirectionalLight();
    rendererPtr_->ClearPointLights();
    rendererPtr_->ClearSpotLights();

    rendererPtr_->Clear();

    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));

    if (cameraPtr->IsTurnedOn())
    {
        rendererPtr_->SetViewMatrix(cameraPtr->GetViewMatrix());
        rendererPtr_->SetViewPosition(cameraPtr->GetPosition());
        rendererPtr_->SetProjectionMatrix(cameraPtr->GetProjectionMatrix());
        rendererPtr_->PrepareCapabilities();

        Project001::RenderedModel* renderedModelArray = nullptr;
        size_t renderedModelCount = 0;

        componentStoresPtr_->GetAllComponents<Project001::RenderedModel>(renderedModelArray, renderedModelCount);

        for (unsigned int i = 0; i < renderedModelCount; ++i)
        {
            Project001::RenderedModel& currentRenderedModel = renderedModelArray[i];

            if (currentRenderedModel.IsVisible())
            {
                const Project001::MeshData* currentMeshDataPtr = currentRenderedModel.GetMeshDataPtr();
                if (currentMeshDataPtr != nullptr)
                {
                    if (!rendererPtr_->AddMesh(
                        currentMeshDataPtr->meshVertexArray.data(),
                        (unsigned int)currentMeshDataPtr->meshVertexArray.size(),
                        currentMeshDataPtr->meshIndexArray.data(),
                        (unsigned int)currentMeshDataPtr->meshIndexArray.size(),
                        currentRenderedModel.GetTextureId(),
                        currentRenderedModel.GetSpecularId(),
                        currentRenderedModel.GetPosition(),
                        currentRenderedModel.GetOrientation(),
                        currentRenderedModel.GetScale(),
                        currentRenderedModel.GetColor(),
                        currentRenderedModel.GetShininess(),
                        currentRenderedModel.GetTranslucent(),
                        currentRenderedModel.GetLit()))
                    {
                        rendererPtr_->Render();
                        _FAIL_CHECK(rendererPtr_->AddMesh(
                            currentMeshDataPtr->meshVertexArray.data(),
                            (unsigned int)currentMeshDataPtr->meshVertexArray.size(),
                            currentMeshDataPtr->meshIndexArray.data(),
                            (unsigned int)currentMeshDataPtr->meshIndexArray.size(),
                            currentRenderedModel.GetTextureId(),
                            currentRenderedModel.GetSpecularId(),
                            currentRenderedModel.GetPosition(),
                            currentRenderedModel.GetOrientation(),
                            currentRenderedModel.GetScale(),
                            currentRenderedModel.GetColor(),
                            currentRenderedModel.GetShininess(),
                            currentRenderedModel.GetTranslucent(),
                            currentRenderedModel.GetLit()));
                    }
                }
            }
        }

        Project001::Camera* cameraPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));

        rendererPtr_->Render();
    }

    windowPtr_->SwapBuffers();

    renderEvent.handled = true;
}

void TestScene013::ProcessScrollEvent(Project001::ScrollEvent& scrollEvent)
{
    float& yOffset = scrollEvent.yOffset;

    float speedConstant = 0.1f;
    float cameraTranslation = speedConstant * yOffset;

    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));
    cameraPtr->MoveForward(cameraTranslation);

    scrollEvent.handled = true;
}

void TestScene013::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    unsigned long timestep_ns = updateEvent.timestep_ns;

    UpdatedSelectedEntityPosition(timestep_ns);
    Sync_RenderedModel_CollisionBody_Components();

    DetectCollisions();
}

void TestScene013::UpdatedSelectedEntityPosition(unsigned long timestep_ns)
{
    float timestep_s = (float)(timestep_ns / 1000000) / 1000;
    float speedConstant = 5.0f;
    float translationSpeed = speedConstant * timestep_s;
    float rotationSpeed = speedConstant * timestep_s;

    bool movingLeft = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_A);
    bool movingRight = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_D);
    bool movingUp = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_W);
    bool movingDown = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_S);
    bool rollingLeft = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_Q);
    bool rollingRight = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_E);

    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));

    if (cameraPtr)
    {
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
    }
}

void TestScene013::DetectCollisions()
{

}

void TestScene013::Sync_RenderedModel_CollisionBody_Components()
{

}

// private: ------------------------------------------------------------------

void TestScene013::Run_UnitTests() const
{
    int result;

    result = Project001::UnitTest_Check3D_Point_Point_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Point_Point_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Point_Line_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Point_Line_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Point_Ray_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Point_Ray_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Point_LineSegment_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Point_LineSegment_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Point_Plane_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Point_Plane_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Point_Triangle_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Point_Triangle_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Point_AABB_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Point_AABB_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Point_OBB_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Point_OBB_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Point_Sphere_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Point_Sphere_Overlap FAILED (%d)", result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Check3D_Line_Line_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Line_Line_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Line_Ray_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Line_Ray_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Line_LineSegment_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Line_LineSegment_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Line_Plane_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Line_Plane_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Line_Triangle_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Line_Triangle_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Line_Sphere_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Line_Sphere_Overlap FAILED (%d)", result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Check3D_Ray_Ray_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Ray_Ray_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Ray_LineSegment_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Ray_LineSegment_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Ray_Plane_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Ray_Plane_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Ray_Triangle_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Ray_Triangle_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Ray_Sphere_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Ray_Sphere_Overlap FAILED (%d)", result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Check3D_LineSegment_LineSegment_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_LineSegment_LineSegment_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_LineSegment_Plane_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_LineSegment_Plane_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_LineSegment_Triangle_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_LineSegment_Triangle_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_LineSegment_Sphere_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_LineSegment_Sphere_Overlap FAILED (%d)", result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Check3D_Plane_Plane_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Plane_Plane_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Plane_Sphere_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Plane_Sphere_Overlap FAILED (%d)", result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Check3D_Triangle_Sphere_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Triangle_Sphere_Overlap FAILED (%d)", result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Check3D_AABB_AABB_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_AABB_AABB_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_AABB_Sphere_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_AABB_Sphere_Overlap FAILED (%d)", result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Check3D_OBB_Sphere_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_OBB_Sphere_Overlap FAILED (%d)", result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Check3D_Sphere_Sphere_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Sphere_Sphere_Overlap FAILED (%d)", result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Get3D_Point_Line_ClosestPoint();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Get3D_Point_Line_ClosestPoint FAILED (%d)", result);
    }

    result = Project001::UnitTest_Get3D_Point_Ray_ClosestPoint();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Get3D_Point_Ray_ClosestPoint FAILED (%d)", result);
    }

    result = Project001::UnitTest_Get3D_Point_LineSegment_ClosestPoint();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Get3D_Point_LineSegment_ClosestPoint FAILED (%d)", result);
    }

    result = Project001::UnitTest_Get3D_Point_Plane_ClosestPoint();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Get3D_Point_Plane_ClosestPoint FAILED (%d)", result);
    }

    result = Project001::UnitTest_Get3D_Point_Triangle_ClosestPoint();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Get3D_Point_Triangle_ClosestPoint FAILED (%d)", result);
    }

    result = Project001::UnitTest_Get3D_Point_AABB_ClosestPoint();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Get3D_Point_AABB_ClosestPoint FAILED (%d)", result);
    }

    result = Project001::UnitTest_Get3D_Point_OBB_ClosestPoint();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Get3D_Point_OBB_ClosestPoint FAILED (%d)", result);
    }

    result = Project001::UnitTest_Get3D_Point_Sphere_ClosestPoint();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Get3D_Point_Sphere_ClosestPoint FAILED (%d)", result);
    }
}