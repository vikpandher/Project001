#include "TestScene006.h"

#include "Engine/Components/RenderedModel.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/FreetypeTextLoader.h"
#include "Engine/Logger.h"
#include "Engine/MeshLoader.h"
#include "Engine/Renderer.h"
#include "Engine/TextureLoader.h"
#include "Engine/Window.h"



// public: ---------------------------------------------------------------------

TestScene006::TestScene006()
{
    ClearIndiciesAndEntityIds();
}

TestScene006::~TestScene006()
{}

const char* TestScene006::Name()
{
    return "TestScene006";
}

void TestScene006::Initialize()
{
    TestSceneBase001::Initialize();

    // Load textures
    // -------------------------------------------------------------------------

    {
        std::vector<unsigned char> characterList;
        for (unsigned char c = 32; c < 127; ++c) // ASCII characters
        {
            characterList.push_back(c);
        }
        _FAIL_CHECK(Project001::FreetypeTextLoader::LoadTexture(fontTextureData_, fontData_, characterList, "../Fonts/Antonio-Regular.ttf", 48));
        _FAIL_CHECK(rendererPtr_->CreateTexture(fontTextureId_, 1, fontTextureData_.data, fontTextureData_.width, fontTextureData_.height, fontTextureData_.bytesPerPixel));
    }

    // generated shape entity 01
    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        float width = 0.005f * fontTextureData_.width;
        float height = 0.005f * fontTextureData_.height;
        positions.emplace_back(0.5f * width, 0.5f * height);
        positions.emplace_back(-0.5f * width, 0.5f * height);
        positions.emplace_back(-0.5f * width, -0.5f * height);
        positions.emplace_back(0.5f * width, -0.5f * height);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, positions));
        std::vector<glm::vec2> textureCoordinates;
        textureCoordinates.emplace_back(1.0f, 1.0f);
        textureCoordinates.emplace_back(0.0f, 1.0f);
        textureCoordinates.emplace_back(0.0f, 0.0f);
        textureCoordinates.emplace_back(1.0f, 0.0f);
        Project001::MeshLoader::ApplyTextureCoordinates(*newMeshDataPtr, textureCoordinates);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(0.0f, 2.0f, 0.0f);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetTextureId(fontTextureId_);
    }

    // generated shape entity 02
    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        const Project001::GlyphMetrics& currentGlyph = fontData_.glyphMetricsMap['A'];
        float width = 0.005f * currentGlyph.width_px;
        float height = 0.005f * currentGlyph.height_px;
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.5f * width, 0.5f * height);
        positions.emplace_back(-0.5f * width, 0.5f * height);
        positions.emplace_back(-0.5f * width, -0.5f * height);
        positions.emplace_back(0.5f * width, -0.5f * height);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, positions));
        std::vector<glm::vec2> textureCoordinates;
        const float& textureBottom = currentGlyph.textureBottomLeft.y;
        const float& textureLeft = currentGlyph.textureBottomLeft.x;
        const float& textureTop = currentGlyph.textureTopRight.y;
        const float& textureRight = currentGlyph.textureTopRight.x;
        textureCoordinates.emplace_back(textureRight, textureTop);
        textureCoordinates.emplace_back(textureLeft, textureTop);
        textureCoordinates.emplace_back(textureLeft, textureBottom);
        textureCoordinates.emplace_back(textureRight, textureBottom);
        Project001::MeshLoader::ApplyTextureCoordinates(*newMeshDataPtr, textureCoordinates);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(-2.0f, 1.0f, 0.0f);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetTextureId(fontTextureId_);
    }

    // generated shape entity 03
    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::FreetypeTextLoader::LoadMesh(*newMeshDataPtr, fontData_, "Sphinx of black quartz, judge my vow!"));
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(0.0f, 0.0f, 0.0f);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetTextureId(fontTextureId_);
    }

    // generated shape entity 04
    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::FreetypeTextLoader::LoadMesh(*newMeshDataPtr, fontData_, "LINE 001\nAND LINE 002\nAND ALSO LINE 003", 0.005f));
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(0.0f, 1.0f, 0.0f);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetTextureId(fontTextureId_);
        renderedModelPtr->SetColorRGB(1.0f, 0.6f, 0.6f);
    }

    // generated shape entity 05
    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::FreetypeTextLoader::LoadMesh(*newMeshDataPtr, fontData_, "LINE 001\nAND LINE 002\nAND ALSO LINE 003", 0.005f, true));
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(2.0f, 1.0f, 0.0f);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetTextureId(fontTextureId_);
        renderedModelPtr->SetColorRGB(0.6f, 0.6f, 1.0f);
    }
}

void TestScene006::Deinitialize()
{
    TestSceneBase001::Deinitialize();

    ClearIndiciesAndEntityIds();
}

void TestScene006::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene006::ProcessKeyEvent, this, std::placeholders::_1));

    TestSceneBase001::OnEvent(event);
}

// protected: ------------------------------------------------------------------

void TestScene006::ClearIndiciesAndEntityIds()
{
    for (size_t i = 0; i < meshDataPtrArray_.size(); ++i)
    {
        delete meshDataPtrArray_[i];
    }
    meshDataPtrArray_.clear();

    fontData_.Clear();

    fontTextureData_.Clear();

    fontTextureId_ = (unsigned int)-1;

    entityIds_.clear();
}

void TestScene006::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_X)
        {
            SendEvent(Project001::SwitchSceneEvent("TestScene010"));
            if (!IsActiveScene())
            {
                Deinitialize();
                SendEvent(Project001::InitializeSceneEvent("TestScene010"));
            }
        }
    }
}
