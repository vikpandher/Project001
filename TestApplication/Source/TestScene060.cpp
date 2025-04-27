// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-04-26

#include "TestScene060.h"

#include "TestApplicationData.h"
#include "TestResource_AntonioRegular_png.h"
#include "TestResource_AntonioRegular_ssf.h"

#include "Components/Camera.h"
#include "Components/RenderedModel.h"
#include "Math/MathUtilities.h"
#include "ComponentStores.h"
#include "FontLoader.h"
#include "Logger.h"
#include "MeshLoader.h"
#include "RenderSystem.h"
#include "TextureLoader.h"
#include "Window.h"



// public ----------------------------------------------------------------------

TestScene060::TestScene060(Project001::Application* applicationPtr)
    : Scene(applicationPtr)
    , instructionScene_(applicationPtr)
    , windowPtr_(nullptr)
    , rendererPtr_(nullptr)
    , componentStoresPtr_(nullptr)
    , font01_FontDataPtr_(nullptr)
    , font01_TextureDataPtr_(nullptr)
    , font01_TextureId_((unsigned int)-1)
    , backgroundRectangleMeshDataPtr_(nullptr)
    , backgroundRectangleMeshId_((unsigned int)-1)
    , joystickAxisTextMeshDataPtr_(nullptr)
    , joystickButton_01_TextMeshDataPtr_(nullptr)
    , joystickButton_02_TextMeshDataPtr_(nullptr)
    , triangleMeshDataPtr_(nullptr)
    , triangleMeshId_((unsigned int)-1)
    , circleMeshDataPtr_(nullptr)
    , circleMeshId_((unsigned int)-1)
    , mainCameraEntityId_((unsigned int)-1)
    , uiCameraEntityId_((unsigned int)-1)
    , backgroundEntityId_((unsigned int)-1)
    , joystickAxisTextEntityId_((unsigned int)-1)
    , joystickButton_01_TextEntityId_((unsigned int)-1)
    , joystickButton_02_TextEntityId_((unsigned int)-1)
    , leftStickEntityId_((unsigned int)-1)
    , leftStickTriangleMeshIndex_((unsigned int)-1)
    , leftStickCircleMeshIndex_((unsigned int)-1)
    , rightStickEntityId_((unsigned int)-1)
    , rightStickTriangleMeshIndex_((unsigned int)-1)
    , rightStickCircleMeshIndex_((unsigned int)-1)
{
    testApplicationDataPtr_ = GetApplicationSharedDataPtr<TestApplicationData>();
    if (testApplicationDataPtr_ != nullptr)
    {
        testApplicationDataPtr_->testScene060Id = GetId();
    }
}

TestScene060::~TestScene060()
{}

void TestScene060::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene060::ProcessInitializeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene060::ProcessDeinitializeEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene060::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestScene060::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene060::ProcessUpdateEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene060::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   TestScene060:            " << GetId());

    windowPtr_ = GetApplicationWindowPtr();
    rendererPtr_ = GetApplicationRendererPtr();
    componentStoresPtr_ = GetApplicaitonComponentStoresPtr();

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

    {
        backgroundRectangleMeshDataPtr_ = new Project001::MeshData();
        std::vector<glm::vec2> positions;
        positions.emplace_back(4.0f, -2.6f);
        positions.emplace_back(4.0f, 2.6f);
        positions.emplace_back(-4.0f, 2.6f);
        positions.emplace_back(-4.0f, -2.6f);
        FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*backgroundRectangleMeshDataPtr_, positions));
        rendererPtr_->CreateMesh(
            backgroundEntityId_,
            backgroundRectangleMeshDataPtr_->meshVertexArray.data(),
            (unsigned int)backgroundRectangleMeshDataPtr_->meshVertexArray.size(),
            backgroundRectangleMeshDataPtr_->meshIndexArray.data(),
            (unsigned int)backgroundRectangleMeshDataPtr_->meshIndexArray.size()
        );
    }

    {
        joystickAxisTextMeshDataPtr_ = new Project001::MeshData();
        FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
            *joystickAxisTextMeshDataPtr_,
            *font01_FontDataPtr_,
            "",
            joystickAxisAndButtonText_pixelSize
        ));
    }

    {
        joystickButton_01_TextMeshDataPtr_ = new Project001::MeshData();
        FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
            *joystickButton_01_TextMeshDataPtr_,
            *font01_FontDataPtr_,
            "",
            joystickAxisAndButtonText_pixelSize
        ));
    }

    {
        joystickButton_02_TextMeshDataPtr_ = new Project001::MeshData();
        FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
            *joystickButton_02_TextMeshDataPtr_,
            *font01_FontDataPtr_,
            "",
            joystickAxisAndButtonText_pixelSize
        ));
    }

    {
        triangleMeshDataPtr_ = new Project001::MeshData();
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.2f, 0.0f);
        positions.emplace_back(0.0f, 0.6f);
        positions.emplace_back(-0.2f, 0.0f);
        FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*triangleMeshDataPtr_, positions));
        rendererPtr_->CreateMesh(
            triangleMeshId_,
            triangleMeshDataPtr_->meshVertexArray.data(),
            (unsigned int)triangleMeshDataPtr_->meshVertexArray.size(),
            triangleMeshDataPtr_->meshIndexArray.data(),
            (unsigned int)triangleMeshDataPtr_->meshIndexArray.size()
        );
    }

    {
        circleMeshDataPtr_ = new Project001::MeshData();
        FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*circleMeshDataPtr_, 0.08f, 8));
        rendererPtr_->CreateMesh(
            circleMeshId_,
            circleMeshDataPtr_->meshVertexArray.data(),
            (unsigned int)circleMeshDataPtr_->meshVertexArray.size(),
            circleMeshDataPtr_->meshIndexArray.data(),
            (unsigned int)circleMeshDataPtr_->meshIndexArray.size()
        );
    }

    // Entity Ids --------------------------------------------------------------

    // Main Camera Entity

    float mainCameraHalfHeight = 0.0f;
    float mainCameraHalfWidth = 0.0f;
    {
        componentStoresPtr_->CreateEntity(mainCameraEntityId_);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::Camera>(mainCameraEntityId_));
        Project001::Camera* cameraPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(cameraPtr, mainCameraEntityId_));
        if (cameraPtr != nullptr)
        {
            int aspectRatioNumerator;
            int aspectRatioDenominator;
            windowPtr_->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);
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
            cameraPtr->TurnOn();
        }
    }

    // UI Camera Entity

    {
        componentStoresPtr_->CreateEntity(uiCameraEntityId_);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::Camera>(uiCameraEntityId_));
        Project001::Camera* cameraPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(cameraPtr, uiCameraEntityId_));
        if (cameraPtr != nullptr)
        {
            int aspectRatioNumerator;
            int aspectRatioDenominator;
            windowPtr_->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);
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

    // Background Entity

    {
        componentStoresPtr_->CreateEntity(backgroundEntityId_);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(backgroundEntityId_));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, backgroundEntityId_));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetMeshDataPtr(backgroundRectangleMeshDataPtr_);
            renderedMeshPtr->SetPositionZ(-0.2f);
            renderedMeshPtr->SetColor(0.1f, 0.1f, 0.2f, 1.0f);
            renderedMeshPtr->SetLit(false);
        }
    }

    // Joystick Axis Text Entity

    {
        componentStoresPtr_->CreateEntity(joystickAxisTextEntityId_);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(joystickAxisTextEntityId_));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, joystickAxisTextEntityId_));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetMeshDataPtr(joystickAxisTextMeshDataPtr_);
            renderedMeshPtr->SetTextureId(font01_TextureId_);
            renderedMeshPtr->SetPosition(-mainCameraHalfWidth + 0.5f, 0.0f, 0.0f);
            renderedMeshPtr->SetColor(0.8f, 0.8f, 0.8f, 1.0f);
            renderedMeshPtr->SetTranslucent(true);
            renderedMeshPtr->SetLit(false);
        }
    }

    // Joystick Button Text 01 Entity

    {
        componentStoresPtr_->CreateEntity(joystickButton_01_TextEntityId_);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(joystickButton_01_TextEntityId_));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, joystickButton_01_TextEntityId_));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetMeshDataPtr(joystickButton_01_TextMeshDataPtr_);
            renderedMeshPtr->SetTextureId(font01_TextureId_);
            renderedMeshPtr->SetPosition(-mainCameraHalfWidth + 2.5f, 0.0f, 0.0f);
            renderedMeshPtr->SetColor(0.8f, 0.8f, 0.8f, 1.0f);
            renderedMeshPtr->SetTranslucent(true);
            renderedMeshPtr->SetLit(false);
        }
    }

    // Joystick Button Text 02 Entity

    {
        componentStoresPtr_->CreateEntity(joystickButton_02_TextEntityId_);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(joystickButton_02_TextEntityId_));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, joystickButton_02_TextEntityId_));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetMeshDataPtr(joystickButton_02_TextMeshDataPtr_);
            renderedMeshPtr->SetTextureId(font01_TextureId_);
            renderedMeshPtr->SetPosition(-mainCameraHalfWidth + 4.5f, 0.0f, 0.0f);
            renderedMeshPtr->SetColor(0.8f, 0.8f, 0.8f, 1.0f);
            renderedMeshPtr->SetTranslucent(true);
            renderedMeshPtr->SetLit(false);
        }
    }

    // Left Stick Entity

    {
        componentStoresPtr_->CreateEntity(leftStickEntityId_);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(leftStickEntityId_));
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(renderedModelPtr, leftStickEntityId_));
        if (renderedModelPtr != nullptr)
        {
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

            leftStickTriangleMeshIndex_ = renderedMeshes.size();
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& polygonShadowMesh = renderedMeshes.back();
            polygonShadowMesh.SetMeshDataPtr(triangleMeshDataPtr_);
            polygonShadowMesh.SetColor(0.8f, 0.4f, 0.2f, 1.0f);
            polygonShadowMesh.SetLit(false);

            leftStickCircleMeshIndex_ = renderedMeshes.size();
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& circleMesh = renderedMeshes.back();
            circleMesh.SetMeshDataPtr(circleMeshDataPtr_);
            circleMesh.SetColor(0.6f, 0.2f, 0.2f, 1.0f);
            circleMesh.SetLit(false);
            circleMesh.SetPositionZ(0.02f);

            renderedModelPtr->SetPosition(-1.0f, 1.5f, 0.0f);
        }
    }

    // Right Stick Entity

    {
        componentStoresPtr_->CreateEntity(rightStickEntityId_);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(rightStickEntityId_));
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(renderedModelPtr, rightStickEntityId_));
        if (renderedModelPtr != nullptr)
        {
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

            rightStickTriangleMeshIndex_ = renderedMeshes.size();
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& polygonShadowMesh = renderedMeshes.back();
            polygonShadowMesh.SetMeshDataPtr(triangleMeshDataPtr_);
            polygonShadowMesh.SetColor(0.8f, 0.4f, 0.2f, 1.0f);
            polygonShadowMesh.SetLit(false);

            rightStickCircleMeshIndex_ = renderedMeshes.size();
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& circleMesh = renderedMeshes.back();
            circleMesh.SetMeshDataPtr(circleMeshDataPtr_);
            circleMesh.SetColor(0.6f, 0.2f, 0.2f, 1.0f);
            circleMesh.SetLit(false);
            circleMesh.SetPositionZ(0.02f);

            renderedModelPtr->SetPosition(1.0f, 1.5f, 0.0f);
        }
    }

    // Member Scenes -----------------------------------------------------------

    TestInstructionScene001::InitializationInfo instructionSceneInfo;
    instructionSceneInfo.hiddenInstructionString = std::string("Press <Tab> to show instructions.");
    instructionSceneInfo.instructionString = std::string(
        "This Scene tests a joystick. It'll output the\n"
        "first joystick's axis and button values while\n"
        "the joystick is plugged in.\n"
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

void TestScene060::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    LOG_INFO("DEINITIALIZING: TestScene060:            " << GetId());

    // -------------------------------------------------------------------------

    rendererPtr_->DeleteAllTextures();
    rendererPtr_->DeleteAllMeshes();
    componentStoresPtr_->DeleteAllEntities();

    windowPtr_ = nullptr;
    rendererPtr_ = nullptr;
    componentStoresPtr_ = nullptr;

    // Font Data ---------------------------------------------------------------

    delete font01_FontDataPtr_;
    font01_FontDataPtr_ = nullptr;
    delete font01_TextureDataPtr_;
    font01_TextureDataPtr_ = nullptr;
    font01_TextureId_ = (unsigned int)-1;

    // Mesh Data ---------------------------------------------------------------

    delete backgroundRectangleMeshDataPtr_;
    backgroundRectangleMeshDataPtr_ = nullptr;
    backgroundRectangleMeshId_ = (unsigned int)-1;

    delete joystickAxisTextMeshDataPtr_;
    joystickAxisTextMeshDataPtr_ = nullptr;

    delete joystickButton_01_TextMeshDataPtr_;
    joystickButton_01_TextMeshDataPtr_ = nullptr;

    delete joystickButton_02_TextMeshDataPtr_;
    joystickButton_02_TextMeshDataPtr_ = nullptr;

    delete triangleMeshDataPtr_;
    triangleMeshDataPtr_ = nullptr;
    triangleMeshId_ = (unsigned int)-1;

    delete circleMeshDataPtr_;
    circleMeshDataPtr_ = nullptr;
    circleMeshId_ = (unsigned int)-1;

    // Entity Ids --------------------------------------------------------------

    mainCameraEntityId_ = (unsigned int)-1;
    uiCameraEntityId_ = (unsigned int)-1;

    backgroundEntityId_ = (unsigned int)-1;

    joystickAxisTextEntityId_ = (unsigned int)-1;
    joystickButton_01_TextEntityId_ = (unsigned int)-1;
    joystickButton_02_TextEntityId_ = (unsigned int)-1;

    leftStickEntityId_ = (unsigned int)-1;
    leftStickTriangleMeshIndex_ = (unsigned int)-1;
    leftStickCircleMeshIndex_ = (unsigned int)-1;

    rightStickEntityId_ = (unsigned int)-1;
    rightStickTriangleMeshIndex_ = (unsigned int)-1;
    rightStickCircleMeshIndex_ = (unsigned int)-1;
}

void TestScene060::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_ESCAPE && testApplicationDataPtr_ != nullptr)
        {
            SendEventToApplication(Project001::SwitchSceneEvent(testApplicationDataPtr_->testScene001Id));
            if (GetActiveScene()->GetId() == testApplicationDataPtr_->testScene001Id)
            {
                SendEventToScene(GetId(), Project001::DeinitializeEvent());
                SendEventToApplication(Project001::InitializeEvent());
            }
            return;
        }
    }
}

void TestScene060::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    Project001::RenderSystem::Render(componentStoresPtr_, rendererPtr_);
}

void TestScene060::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    if (windowPtr_->GetJoystickPresent(0))
    {
        std::vector<float> axes;
        windowPtr_->GetJoystickAxis(0, axes);

        std::string joystickAxisText;
        for (size_t i = 0; i < axes.size(); ++i)
        {
            joystickAxisText.append("axes " + std::to_string(i) + ": " + std::to_string(axes[i]) + "\n");
        }

        joystickAxisTextMeshDataPtr_->Clear();
        FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
            *joystickAxisTextMeshDataPtr_,
            *font01_FontDataPtr_,
            joystickAxisText.c_str(),
            joystickAxisAndButtonText_pixelSize
        ));

        const float deadZone = 0.2f;
        const bool usingDeadZone = true;

        glm::vec2 leftStickVector(0.0f, 0.0f);
        if (axes.size() > 2)
        {
            leftStickVector.x = axes[0];
            leftStickVector.y = -axes[1];

            if (usingDeadZone && (glm::length(leftStickVector) < deadZone))
            {
                leftStickVector.x = 0.0f;
                leftStickVector.y = 0.0f;
            }
        }

        if (!(leftStickVector.x == 0.0f && leftStickVector.y == 0.0f))
        {
            float leftStickAngle = Project001::Get2DVectorAngle(glm::vec2(0.0f, 1.0f), leftStickVector);

            Project001::RenderedModel* renderedModelPtr;
            if (componentStoresPtr_->GetComponent<Project001::RenderedModel>(renderedModelPtr, leftStickEntityId_))
            {
                renderedModelPtr->ResetOrientation();
                renderedModelPtr->AddRelativeRotationZ(leftStickAngle);
            }
        }

        glm::vec2 rightStickVector(0.0f, 0.0f);
        if (axes.size() > 4)
        {
            rightStickVector.x = axes[2];
            rightStickVector.y = -axes[3];

            if (usingDeadZone && (glm::length(rightStickVector) < deadZone))
            {
                rightStickVector.x = 0.0f;
                rightStickVector.y = 0.0f;
            }
        }

        if (!(rightStickVector.x == 0.0f && rightStickVector.y == 0.0f))
        {
            float rightStickAngle = Project001::Get2DVectorAngle(glm::vec2(0.0f, 1.0f), rightStickVector);

            Project001::RenderedModel* renderedModelPtr;
            if (componentStoresPtr_->GetComponent<Project001::RenderedModel>(renderedModelPtr, rightStickEntityId_))
            {
                renderedModelPtr->ResetOrientation();
                renderedModelPtr->AddRelativeRotationZ(rightStickAngle);
            }
        }

        // ---------------------------------------------------------------------

        std::vector<bool> buttons;
        windowPtr_->GetJoystickButtons(0, buttons);

        std::string joystickButtonText01;
        for (size_t i = 0; i < 7 && i < buttons.size(); ++i)
        {
            joystickButtonText01.append("buttons " + std::to_string(i) + ": " + std::to_string(buttons[i]) + "\n");
        }

        joystickButton_01_TextMeshDataPtr_->Clear();
        FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
            *joystickButton_01_TextMeshDataPtr_,
            *font01_FontDataPtr_,
            joystickButtonText01.c_str(),
            joystickAxisAndButtonText_pixelSize
        ));

        std::string joystickButtonText02;
        for (size_t i = 7; i < buttons.size(); ++i)
        {
            joystickButtonText02.append("buttons " + std::to_string(i) + ": " + std::to_string(buttons[i]) + "\n");
        }

        joystickButton_02_TextMeshDataPtr_->Clear();
        FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
            *joystickButton_02_TextMeshDataPtr_,
            *font01_FontDataPtr_,
            joystickButtonText02.c_str(),
            joystickAxisAndButtonText_pixelSize
        ));
    }
    else
    {
        const char* noJoyStickTestPtr = "NO JOYSTICK DETECTED";

        joystickAxisTextMeshDataPtr_->Clear();
        FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
            *joystickAxisTextMeshDataPtr_,
            *font01_FontDataPtr_,
            noJoyStickTestPtr,
            joystickAxisAndButtonText_pixelSize
        ));

        joystickButton_01_TextMeshDataPtr_->Clear();

        joystickButton_02_TextMeshDataPtr_->Clear();
    }
}