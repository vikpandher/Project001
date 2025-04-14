// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#include "TestInstructionScene001.h"

#include "Components/Camera.h"
#include "Components/RenderedModel.h"
#include "ComponentStores.h"
#include "FontLoader.h"
#include "Logger.h"
#include "MeshLoader.h"



// public ----------------------------------------------------------------------

TestInstructionScene001::TestInstructionScene001(Project001::Application* applicationPtr)
    : Scene(applicationPtr)
    , componentStoresPtr_(nullptr)
    , hiddenInstructionTextMeshDataPtr_(nullptr)
    , instructionTextMeshDataPtr_(nullptr)
    , hiddenInstructionBackgroundMeshDataPtr_(nullptr)
    , instructionBackgroundMeshDataPtr_(nullptr)
    , instructionsEntityId_((unsigned int)-1)
    , hiddenInstructionMeshIndex_((unsigned int)-1)
    , instructionMeshIndex_((unsigned int)-1)
    , hiddenInstructionBackgroundMeshIndex_((unsigned int)-1)
    , instructionBackgroundMeshIndex_((unsigned int)-1)
    , keyCode_toggleInstructions_(Project001::KeyCode::KEY_CODE_UNKNOWN)
{}

TestInstructionScene001::~TestInstructionScene001()
{}

void TestInstructionScene001::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestInstructionScene001::ProcessKeyEvent, this, std::placeholders::_1));
}

void TestInstructionScene001::Initialize(const InitializationInfo& initializationInfo)
{
    LOG_INFO("INITIALIZING:   TestInstructionScene001: " << GetId());

    componentStoresPtr_ = GetApplicaitonComponentStoresPtr();

    keyCode_toggleInstructions_ = *initializationInfo.keyCode_toggleInstructionsPtr;

    // Create Meshes -----------------------------------------------------------

    const float instructionFontPixelSize = 0.0048f;

    hiddenInstructionTextMeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
        *hiddenInstructionTextMeshDataPtr_,
        *initializationInfo.fontDataPtr,
        initializationInfo.hiddenInstructionString,
        instructionFontPixelSize
    ));
    Project001::MeshLoader::RecenterMesh(*hiddenInstructionTextMeshDataPtr_);

    instructionTextMeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
        *instructionTextMeshDataPtr_,
        *initializationInfo.fontDataPtr,
        initializationInfo.instructionString,
        instructionFontPixelSize
    ));
    Project001::MeshLoader::RecenterMesh(*instructionTextMeshDataPtr_);

    float bezelSize = 0.08f;

    glm::vec3 hiddenInstructionMeshDataSize = hiddenInstructionTextMeshDataPtr_->GetSize();

    hiddenInstructionBackgroundMeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DBezeledRectangle(
        *hiddenInstructionBackgroundMeshDataPtr_,
        hiddenInstructionMeshDataSize.x + bezelSize,
        hiddenInstructionMeshDataSize.y + bezelSize,
        bezelSize,
        4
    ));

    glm::vec3 instructionMeshDataSize = instructionTextMeshDataPtr_->GetSize();

    instructionBackgroundMeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DBezeledRectangle(
        *instructionBackgroundMeshDataPtr_,
        instructionMeshDataSize.x + bezelSize,
        instructionMeshDataSize.y + bezelSize,
        bezelSize,
        4
    ));

    // Create Entitys ----------------------------------------------------------

    float uiCameraHalfWidth = 0;
    float uiCameraHalfHeight = 0;
    {
        Project001::Camera* cameraPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(cameraPtr, *initializationInfo.cameraEntityIdPtr));
        if(cameraPtr != nullptr)
        {
            uiCameraHalfWidth = cameraPtr->GetRightCutoff();
            uiCameraHalfHeight = cameraPtr->GetTopCutoff();
        }
    }

    {
        componentStoresPtr_->CreateEntity(instructionsEntityId_);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(instructionsEntityId_));
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(renderedModelPtr, instructionsEntityId_));
        if (renderedModelPtr != nullptr)
        {
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

            glm::vec2 hiddenInstructionMeshPosition(
                -uiCameraHalfWidth + hiddenInstructionTextMeshDataPtr_->maxVertexPosition.x + 0.2f,
                uiCameraHalfHeight - hiddenInstructionTextMeshDataPtr_->maxVertexPosition.y - 0.2f
            );

            glm::vec2 instructionMeshPosition(
                -uiCameraHalfWidth + instructionTextMeshDataPtr_->maxVertexPosition.x + 0.2f,
                uiCameraHalfHeight - instructionTextMeshDataPtr_->maxVertexPosition.y - 0.2f
            );

            hiddenInstructionMeshIndex_ = renderedMeshes.size();
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& hiddenInstructionMesh = renderedMeshes.back();
            hiddenInstructionMesh.SetCameraMask(*initializationInfo.cameraMaskPtr);
            hiddenInstructionMesh.SetMeshDataPtr(hiddenInstructionTextMeshDataPtr_);
            hiddenInstructionMesh.SetTextureId(*initializationInfo.fontTextureIdPtr);
            hiddenInstructionMesh.SetColor(0.8f, 0.7f, 0.3f, 1.0f);
            hiddenInstructionMesh.SetLit(false);
            hiddenInstructionMesh.SetPosition(hiddenInstructionMeshPosition.x, hiddenInstructionMeshPosition.y, 0.0f);

            instructionMeshIndex_ = renderedMeshes.size();
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& instructionMesh = renderedMeshes.back();
            instructionMesh.SetCameraMask(*initializationInfo.cameraMaskPtr);
            instructionMesh.SetMeshDataPtr(instructionTextMeshDataPtr_);
            instructionMesh.SetTextureId(*initializationInfo.fontTextureIdPtr);
            instructionMesh.SetColor(0.8f, 0.7f, 0.3f, 1.0f);
            instructionMesh.SetLit(false);
            instructionMesh.SetPosition(instructionMeshPosition.x, instructionMeshPosition.y, 0.0f);
            instructionMesh.SetVisible(false);

            hiddenInstructionBackgroundMeshIndex_ = renderedMeshes.size();
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& hiddenInstructionBackgroundMesh = renderedMeshes.back();
            hiddenInstructionBackgroundMesh.SetCameraMask(*initializationInfo.cameraMaskPtr);
            hiddenInstructionBackgroundMesh.SetMeshDataPtr(hiddenInstructionBackgroundMeshDataPtr_);
            hiddenInstructionBackgroundMesh.SetColor(0.1f, 0.1f, 0.1f, 0.9f);
            hiddenInstructionBackgroundMesh.SetLit(false);
            hiddenInstructionBackgroundMesh.SetPosition(hiddenInstructionMeshPosition.x, hiddenInstructionMeshPosition.y, 0.0f);
            hiddenInstructionBackgroundMesh.SetRenderPriorityOverride(-100);

            instructionBackgroundMeshIndex_ = renderedMeshes.size();
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& instructionBackgroundMesh = renderedMeshes.back();
            instructionBackgroundMesh.SetCameraMask(*initializationInfo.cameraMaskPtr);
            instructionBackgroundMesh.SetMeshDataPtr(instructionBackgroundMeshDataPtr_);
            instructionBackgroundMesh.SetColor(0.1f, 0.1f, 0.1f, 0.9f);
            instructionBackgroundMesh.SetLit(false);
            instructionBackgroundMesh.SetPosition(instructionMeshPosition.x, instructionMeshPosition.y, 0.0f);
            instructionBackgroundMesh.SetRenderPriorityOverride(-100);
            instructionBackgroundMesh.SetVisible(false);
        }
    }
}

void TestInstructionScene001::Deinitialize()
{
    LOG_INFO("DEINITIALIZING: TestInstructionScene001: " << GetId());

    // Mesh Data ---------------------------------------------------------------

    delete hiddenInstructionTextMeshDataPtr_;
    hiddenInstructionTextMeshDataPtr_ = nullptr;
    delete instructionTextMeshDataPtr_;
    instructionTextMeshDataPtr_ = nullptr;

    delete hiddenInstructionBackgroundMeshDataPtr_;
    hiddenInstructionBackgroundMeshDataPtr_ = nullptr;
    delete instructionBackgroundMeshDataPtr_;
    instructionBackgroundMeshDataPtr_ = nullptr;

    // Entity Ids --------------------------------------------------------------

    componentStoresPtr_->DeleteEntity(instructionsEntityId_);
    instructionsEntityId_ = (unsigned int)-1;
    hiddenInstructionMeshIndex_ = (unsigned int)-1;
    instructionMeshIndex_ = (unsigned int)-1;
    hiddenInstructionBackgroundMeshIndex_ = (unsigned int)-1;
    instructionBackgroundMeshIndex_ = (unsigned int)-1;

    // -------------------------------------------------------------------------

    componentStoresPtr_ = nullptr;

    keyCode_toggleInstructions_ = Project001::KeyCode::KEY_CODE_UNKNOWN;
}

// protected -------------------------------------------------------------------

void TestInstructionScene001::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == keyCode_toggleInstructions_)
        {
            Project001::RenderedModel* renderedModelPtr = nullptr;
            FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(renderedModelPtr, instructionsEntityId_));
            if (renderedModelPtr != nullptr)
            {
                std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

                Project001::RenderedMesh& hiddenInstructionMesh = renderedMeshes[hiddenInstructionMeshIndex_];
                hiddenInstructionMesh.SetVisible(!hiddenInstructionMesh.GetVisible());

                Project001::RenderedMesh& instructionMesh = renderedMeshes[instructionMeshIndex_];
                instructionMesh.SetVisible(!instructionMesh.GetVisible());

                Project001::RenderedMesh& hiddenInstructionBackgroundMesh = renderedMeshes[hiddenInstructionBackgroundMeshIndex_];
                hiddenInstructionBackgroundMesh.SetVisible(!hiddenInstructionBackgroundMesh.GetVisible());

                Project001::RenderedMesh& instructionBackgroundMesh = renderedMeshes[instructionBackgroundMeshIndex_];
                instructionBackgroundMesh.SetVisible(!instructionBackgroundMesh.GetVisible());
            }
        }
    }
}