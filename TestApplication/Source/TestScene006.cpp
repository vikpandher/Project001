// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#include "TestScene006.h"

#include "TestResource_AntonioRegular_png.h"
#include "TestResource_AntonioRegular_ssf.h"

#include "Components/Camera.h"
#include "Components/RenderedModel.h"
#include "Application.h"
#include "ComponentStores.h"
#include "FontLoader.h"
#include "Logger.h"
#include "MeshLoader.h"
#include "Renderer.h"
#include "TextureLoader.h"
#include "Window.h"



// public ----------------------------------------------------------------------

TestScene006::TestScene006(Project001::Application* applicationPtr)
    : TestSceneBase001(applicationPtr)
    , instructionScene_(applicationPtr)
    , zzzEntityId_((unsigned int)-1)
    , printableEntityId_((unsigned int)-1)
    , sphinxEntityId_((unsigned int)-1)
    , timestamp_ns_(0ull)
{}

TestScene006::~TestScene006()
{}

void TestScene006::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene006::ProcessDeinitializeEvent, this, std::placeholders::_1));

    TestSceneBase001::HandleEvent(event);

    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene006::ProcessInitializeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene006::ProcessUpdateEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene006::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   TestScene006:            " << GetId());

    // Main Camera Entity
    // -------------------------------------------------------------------------
    {
        Project001::Camera* cameraPtr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(cameraPtr, mainCameraEntityId_));
        if (cameraPtr != nullptr)
        {
            cameraPtr->SetDepthTestEnabled(false);
        }
    }

    // Load Font ---------------------------------------------------------------

    std::vector<unsigned char> characterList;
    for (unsigned char c = 32; c < 127; ++c) // ASCII characters
    {
        characterList.push_back(c);
    }

    float font01_pixelSize = 0.005f;
    Project001::FontData font01_FontData;
    FAIL_CHECK(Project001::FontLoader::LoadFontDataFromMemory(
        font01_FontData,
        g_AntonioRegular_ssf,
        sizeof(g_AntonioRegular_ssf)
    ));

    Project001::TextureData font01_TextureData;
    FAIL_CHECK(Project001::TextureLoader::LoadTextureFromMemory(
        font01_TextureData,
        g_AntonioRegular_png,
        sizeof(g_AntonioRegular_png)
    ));
    unsigned int font01_TextureId = (unsigned int)-1;
    rendererPtr_->CreateTexture(
        font01_TextureId,
        font01_TextureData.data,
        font01_TextureData.width,
        font01_TextureData.height,
        font01_TextureData.bytesPerPixel,
        true,
        false
    );

    float font02_pixelSize = 0.005f;
    Project001::FontData font02_FontData;
    FAIL_CHECK(Project001::FontLoader::LoadFontData(
        font02_FontData,
        "../Fonts/Antonio-Regular.ssf"
    ));

    Project001::TextureData font02_TextureData;
    FAIL_CHECK(Project001::TextureLoader::LoadTexture(
        font02_TextureData,
        "../Fonts/Antonio-Regular.png"
    ));
    unsigned int font02_TextureId = (unsigned int)-1;
    rendererPtr_->CreateTexture(
        font02_TextureId,
        font02_TextureData.data,
        font02_TextureData.width,
        font02_TextureData.height,
        font02_TextureData.bytesPerPixel,
        false,
        false
    );

    Project001::GlyphMeshData font01_Z_glyphMeshData;
    FAIL_CHECK(Project001::FontLoader::GenerateGlpyhMeshDataFromFontDataAndCharacter(
        font01_Z_glyphMeshData,
        font01_FontData,
        'Z',
        font01_pixelSize
    ));

    Project001::FontMeshData font01_fontMeshData;
    FAIL_CHECK(Project001::FontLoader::GenerateFontMeshDataFromFontData(
        font01_fontMeshData,
        font01_FontData,
        font01_pixelSize
    ));

    // Generated Shape Entity 00
    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        float width = font01_pixelSize * font01_TextureData.width;
        float height = font01_pixelSize * font01_TextureData.height;
        FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
            *newMeshDataPtr,
            width,
            height,
            0.0f,
            1.0f,
            0.0f,
            1.0f
        ));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(0.0f, 2.5f, 0.0f);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(font01_TextureId);
        }
    }

    // Generated Shape Entity 01
    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        float width = font02_pixelSize * font02_TextureData.width;
        float height = font02_pixelSize * font02_TextureData.height;
        FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
            *newMeshDataPtr,
            width,
            height,
            0.0f,
            1.0f,
            0.0f,
            1.0f
        ));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(0.0f, 2.0f, 0.0f);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(font01_TextureId);
            renderedMeshPtr->SetColorRGB(0.8f, 0.7f, 0.3f);
        }
    }

    // Generated Shape Entity 02
    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        const Project001::GlyphMetrics& currentGlyph = font01_FontData.glyphMetricsMap['A'];
        float width = 4.0f * font01_pixelSize * currentGlyph.width_px;
        float height = 4.0f * font01_pixelSize * currentGlyph.height_px;
        const float& textureBottom = currentGlyph.textureBottomLeft.y;
        const float& textureLeft = currentGlyph.textureBottomLeft.x;
        const float& textureTop = currentGlyph.textureTopRight.y;
        const float& textureRight = currentGlyph.textureTopRight.x;
        FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
            *newMeshDataPtr,
            width,
            height,
            textureLeft,
            textureRight,
            textureBottom,
            textureTop
        ));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(-2.0f, 1.0f, 0.0f);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(font02_TextureId);
            renderedMeshPtr->SetColorRGB(1.0f, 0.6f, 0.6f);
        }
    }

    // Generated Shape Entity 03
    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
            *newMeshDataPtr,
            font01_FontData,
            "LINE 001\nAND LINE 002\nAND ALSO LINE 003",
            font01_pixelSize
        ));
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(0.0f, 1.0f, 0.0f);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(font02_TextureId);
            renderedMeshPtr->SetColorRGB(0.6f, 1.0f, 0.6f);
        }
    }

    // Generated Shape Entity 04
    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
            *newMeshDataPtr,
            font01_FontData,
            "LINE 001\nAND LINE 002\nAND ALSO LINE 003",
            font01_pixelSize,
            true
        ));
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(2.0f, 1.0f, 0.0f);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(font02_TextureId);
            renderedMeshPtr->SetColorRGB(0.6f, 0.6f, 1.0f);
        }
    }

    // Generated Shape Entity 05
    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
            *newMeshDataPtr,
            font01_FontData,
            "Sphinx of black quartz, judge my vow!",
            font01_pixelSize * 2.0f
        ));
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(0.0f, 0.0f, 0.0f);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(font02_TextureId);
        }
    }

    // Generated Shape Entity 06 (zzzEntityId_)
    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        Project001::MeshLoader::CopyMesh(
            *newMeshDataPtr,
            font01_Z_glyphMeshData.meshData
        );
        // Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
        float meshScale = 4.0f;
        Project001::MeshLoader::ScaleMesh(
            *newMeshDataPtr,
            glm::vec3(meshScale)
        );

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);
        zzzEntityId_ = tempEntityId;

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(renderedModelPtr, tempEntityId));
        if (renderedModelPtr != nullptr)
        {
            renderedModelPtr->SetPosition(-3.2f, -1.6f, 0.0f);
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
            renderedMeshes.reserve(5);

            renderedMeshes.emplace_back();
            Project001::RenderedMesh& character_Z_Mesh1 = renderedMeshes.back();
            character_Z_Mesh1.SetMeshDataPtr(newMeshDataPtr);
            character_Z_Mesh1.SetTextureId(font02_TextureId);
            character_Z_Mesh1.SetColorRGB(1.0f, 0.8f, 0.8f);

            renderedMeshes.emplace_back();
            Project001::RenderedMesh& character_Z_Mesh2 = renderedMeshes.back();
            character_Z_Mesh2.SetMeshDataPtr(newMeshDataPtr);
            character_Z_Mesh2.SetTextureId(font02_TextureId);
            character_Z_Mesh2.SetColorRGB(1.0f, 0.6f, 0.6f);
            character_Z_Mesh2.SetPosition(
                character_Z_Mesh1.GetPosition().x + meshScale * font01_Z_glyphMeshData.horiAdvance,
                character_Z_Mesh1.GetPosition().y,
                character_Z_Mesh1.GetPosition().z
            );

            renderedMeshes.emplace_back();
            Project001::RenderedMesh& character_Z_Mesh3 = renderedMeshes.back();
            character_Z_Mesh3.SetMeshDataPtr(newMeshDataPtr);
            character_Z_Mesh3.SetTextureId(font02_TextureId);
            character_Z_Mesh3.SetColorRGB(1.0f, 0.4f, 0.4f);
            character_Z_Mesh3.SetPosition(
                character_Z_Mesh2.GetPosition().x + meshScale * font01_Z_glyphMeshData.horiAdvance,
                character_Z_Mesh2.GetPosition().y,
                character_Z_Mesh2.GetPosition().z
            );

            renderedMeshes.emplace_back();
            Project001::RenderedMesh& character_Z_Mesh4 = renderedMeshes.back();
            character_Z_Mesh4.SetMeshDataPtr(newMeshDataPtr);
            character_Z_Mesh4.SetTextureId(font02_TextureId);
            character_Z_Mesh4.SetColorRGB(1.0f, 0.2f, 0.2f);
            character_Z_Mesh4.SetPosition(
                character_Z_Mesh3.GetPosition().x + meshScale * font01_Z_glyphMeshData.horiAdvance,
                character_Z_Mesh3.GetPosition().y,
                character_Z_Mesh3.GetPosition().z
            );

            renderedMeshes.emplace_back();
            Project001::RenderedMesh& character_Z_Mesh5 = renderedMeshes.back();
            character_Z_Mesh5.SetMeshDataPtr(newMeshDataPtr);
            character_Z_Mesh5.SetTextureId(font02_TextureId);
            character_Z_Mesh5.SetColorRGB(1.0f, 0.0f, 0.0f);
            character_Z_Mesh5.SetPosition(
                character_Z_Mesh4.GetPosition().x + meshScale * font01_Z_glyphMeshData.horiAdvance,
                character_Z_Mesh4.GetPosition().y,
                character_Z_Mesh4.GetPosition().z
            );
        }
    }

    // Generated Shape Entity 07 (printableEntityId_)
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);
        printableEntityId_ = tempEntityId;

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(renderedModelPtr, tempEntityId));
        if (renderedModelPtr != nullptr)
        {
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
            float currentOffsetX = 0.0f;
            float colorFade = 0.0f;
            float scale = 0.8f;
            std::unordered_map<unsigned char, Project001::GlyphMeshData>::const_iterator iter =
                font01_fontMeshData.glyphMeshDataMap.begin();
            for (; iter != font01_fontMeshData.glyphMeshDataMap.end(); ++iter)
            {
                const Project001::MeshData currentMeshData = iter->second.meshData;

                Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
                meshDataPtrArray_.push_back(newMeshDataPtr);
                Project001::MeshLoader::CopyMesh(
                    *newMeshDataPtr,
                    currentMeshData
                );

                renderedMeshes.emplace_back();
                Project001::RenderedMesh& currentRenderedMesh = renderedMeshes.back();
                currentRenderedMesh.SetMeshDataPtr(newMeshDataPtr);
                currentRenderedMesh.SetTextureId(font02_TextureId);
                currentRenderedMesh.SetColorRGB(1.0f - colorFade, 1.0f, 1.0f - colorFade);
                currentRenderedMesh.SetScale(glm::vec3(scale));
                currentRenderedMesh.SetPositionX(currentOffsetX * scale);

                currentOffsetX += iter->second.horiAdvance;
                colorFade += 0.01f;
            }
            renderedModelPtr->SetPosition(-0.5f * currentOffsetX * scale, -2.4f, 0.0f);
        }
    }

    // Generated Shape Entity 08 (sphinxEntityId_)
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);
        sphinxEntityId_ = tempEntityId;

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(renderedModelPtr, tempEntityId));
        if (renderedModelPtr != nullptr)
        {
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
            renderedModelPtr->SetPosition(0.0f, -1.6f, 0.0f);
            float currentOffsetX = 0.0f;
            float colorFade = 0.0f;
            glm::vec3 scale(1.2f, 3.6f, 1.0f);

            std::string text = "Sphinx of black quartz, judge my vow!";
            for (size_t i = 0; i < text.length(); ++i)
            {
                const char& c = text[i];
                const Project001::GlyphMeshData& currentGlyphMeshData = font01_fontMeshData.glyphMeshDataMap[c];

                Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
                meshDataPtrArray_.push_back(newMeshDataPtr);
                Project001::MeshLoader::CopyMesh(
                    *newMeshDataPtr,
                    currentGlyphMeshData.meshData
                );

                renderedMeshes.emplace_back();
                Project001::RenderedMesh& currentRenderedMesh = renderedMeshes.back();
                currentRenderedMesh.SetMeshDataPtr(newMeshDataPtr);
                currentRenderedMesh.SetTextureId(font02_TextureId);
                currentRenderedMesh.SetColorRGB(1.0f - colorFade, 1.0f - colorFade, 1.0f);
                currentRenderedMesh.SetScale(scale);
                currentRenderedMesh.SetPositionX(currentOffsetX * scale.x);
                currentRenderedMesh.SetTranslucent(true);

                currentOffsetX += currentGlyphMeshData.horiAdvance;
                colorFade += 0.02f;
            }
        }
    }

    // Member Scenes -----------------------------------------------------------

    const Project001::KeyCode keyCode_toggleInstructions = Project001::KeyCode::KEY_CODE_TAB;

    TestInstructionScene001::InitializationInfo instructionSceneInfo;
    instructionSceneInfo.hiddenInstructionString = std::string("Press <Tab> to show instructions.");
    instructionSceneInfo.instructionString = std::string(
        "This Scene tests Text Generation.\n"
        "Use <WASD> to move the camera up, left, down, and right.\n"
        "Use <Q> to roll left and <E> to roll right.\n"
        "Use <Scroll> to move forward and back.\n"
        "<Left-Click> and drag the <Mouse> to move camera.\n"
        "Press <Esc> to return to Main Menu.\n"
        "Press <Tab> to hide instructions."
    );
    instructionSceneInfo.fontDataPtr = &font01_FontData;
    instructionSceneInfo.fontTextureIdPtr = &font01_TextureId;
    instructionSceneInfo.cameraEntityIdPtr = &uiCameraEntityId_;
    instructionSceneInfo.cameraMaskPtr = &s_uiCameraMask_;
    instructionSceneInfo.keyCode_toggleInstructionsPtr = &keyCode_toggleInstructions;
    instructionScene_.Initialize(instructionSceneInfo);
}

void TestScene006::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    LOG_INFO("DEINITIALIZING: TestScene006:            " << GetId());

    zzzEntityId_ = (unsigned int)-1;
    printableEntityId_ = (unsigned int)-1;
    sphinxEntityId_ = (unsigned int)-1;

    timestamp_ns_ = 0ull;
}

void TestScene006::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    timestamp_ns_ += updateEvent.timestep_ns;

    {
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(renderedModelPtr, zzzEntityId_));
        if (renderedModelPtr != nullptr)
        {
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

            for (size_t i = 0; i < renderedMeshes.size(); ++i)
            {
                Project001::RenderedMesh& renderedMesh = renderedMeshes[i];
                float yShift = 0.2f * glm::sin(glm::pi<float>() * (0.2f * (float)(i)+8.0f * float(timestamp_ns_) * 0.0000000001f));
                renderedMesh.SetPositionY(yShift);
            }
        }
    }

    {
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(renderedModelPtr, printableEntityId_));
        if (renderedModelPtr != nullptr)
        {
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

            for (size_t i = 0; i < renderedMeshes.size(); ++i)
            {
                Project001::RenderedMesh& renderedMesh = renderedMeshes[i];
                float yShift = 0.2f * glm::sin(glm::pi<float>() * (0.2f * (float)(i)+8.0f * float(timestamp_ns_) * 0.0000000001f));
                renderedMesh.SetPositionY(yShift);
            }
        }
    }

    {
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(renderedModelPtr, sphinxEntityId_));
        if (renderedModelPtr != nullptr)
        {
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

            for (size_t i = 0; i < renderedMeshes.size(); ++i)
            {
                Project001::RenderedMesh& renderedMesh = renderedMeshes[i];
                float yShift = 0.02f * glm::sin(glm::pi<float>() * (0.2f * (float)(i)+2.0f * float(timestamp_ns_) * 0.0000000001f));
                renderedMesh.SetPositionY(yShift);
                float zRotate = -0.02f * glm::sin(glm::pi<float>() * (0.2f * (float)(i)+32.0f * float(timestamp_ns_) * 0.0000000001f));
                renderedMesh.AddRelativeRotationZ(zRotate);
            }
        }
    }
}