// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-19

#include "TestScene005.h"

#include "TestApplicationData.h"
#include "TestResource_AntonioRegular_png.h"
#include "TestResource_AntonioRegular_ssf.h"

#include "Components/RenderedMesh.h"
#include "Utilities/FontUtility.h"
#include "Utilities/MathUtility.h"
#include "Utilities/MeshUtility.h"
#include "Utilities/TextureUtility.h"
#include "Application.h"
#include "ComponentStores.h"
#include "Logger.h"
#include "Renderer.h"
#include "Window.h"



// public ----------------------------------------------------------------------

TestScene005::TestScene005(Project001::Application* applicationPtr)
    : TestSceneBase001(applicationPtr)
    , instructionScene_(applicationPtr)
{
    GetSharedDataPtr<TestApplicationData>()->testScene005Id = GetId();

    TestOrderPointsCCW();
    TestRemoveDuplicates();
}

TestScene005::~TestScene005()
{}

void TestScene005::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene005::ProcessDeinitializeEvent, this, std::placeholders::_1));

    TestSceneBase001::HandleEvent(event);

    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene005::ProcessInitializeEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene005::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   TestScene005:            " << GetId());

    // Texture Data ------------------------------------------------------------

    unsigned int numbers12x6_TextureId = static_cast<unsigned int>(-1);

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::Texture::LoadTexture(textureData, "../Textures/12_6_numbers.png"));
        GetRendererPtr()->CreateTexture(numbers12x6_TextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    // Creating Entities
    // -------------------------------------------------------------------------

    // Calculating positions ---------------------------------------------------

    std::vector<glm::vec3> meshEntityPositions;
    for (int j = 2; j >= -2; --j)
    {
        for (int i = -4; i <= 4; ++i)
        {
            meshEntityPositions.emplace_back(static_cast<float>(i) + 0.5f, static_cast<float>(j), 0.0f);
        }
    }

    size_t positionIndex = 0;

    // row 1 -------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        const glm::vec3 min(-0.30f, -0.20f, -0.15f);
        const glm::vec3 max(0.30f, 0.20f, 0.15f);
        FAIL_CHECK(Project001::Mesh::GenerateBox(*newMeshDataPtr, min, max));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);

        Project001::MeshData tempMeshData01;
        Project001::MeshData tempMeshData02;
        Project001::MeshData tempMeshData03;
        std::vector<glm::vec3> capCorners;
        const glm::vec3 min(-0.30f, -0.20f, -0.15f);
        const glm::vec3 max(0.30f, 0.20f, 0.15f);
        FAIL_CHECK(Project001::Mesh::GenerateBox(tempMeshData01, min, max));
        Project001::Mesh::SliceMeshWithAPlane(tempMeshData02, tempMeshData03, capCorners, tempMeshData01, glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
        Project001::Mesh::TranslateMesh(tempMeshData02, glm::vec3(0.0f, 0.04f, 0.0f));
        Project001::Mesh::TranslateMesh(tempMeshData03, glm::vec3(0.0f, -0.04f, 0.0f));
        Project001::Mesh::CopyMesh(*newMeshDataPtr, tempMeshData02);
        Project001::Mesh::CopyMesh(*newMeshDataPtr, tempMeshData03);

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    {
        positionIndex++;
    }

    {
        positionIndex++;
    }

    {
        positionIndex++;
    }

    {
        positionIndex++;
    }

    {
        positionIndex++;
    }

    {
        positionIndex++;
    }

    {
        positionIndex++;
    }

    // row 2 -------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::GenerateSphere(*newMeshDataPtr, 0.32f, 8, 8));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(numbers12x6_TextureId);
        }
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);

        Project001::MeshData tempMeshData01;
        Project001::MeshData tempMeshData02;
        Project001::MeshData tempMeshData03;
        std::vector<glm::vec3> capCorners;
        FAIL_CHECK(Project001::Mesh::GenerateSphere(tempMeshData01, 0.32f, 8, 8));
        Project001::Mesh::SliceMeshWithAPlane(tempMeshData02, tempMeshData03, capCorners, tempMeshData01, glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
        Project001::Mesh::TranslateMesh(tempMeshData02, glm::vec3(0.0f, 0.04f, 0.0f));
        Project001::Mesh::TranslateMesh(tempMeshData03, glm::vec3(0.0f, -0.04f, 0.0f));
        Project001::Mesh::CopyMesh(*newMeshDataPtr, tempMeshData02);
        Project001::Mesh::CopyMesh(*newMeshDataPtr, tempMeshData03);

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(numbers12x6_TextureId);
        }
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);

        Project001::MeshData tempMeshData01;
        Project001::MeshData tempMeshData02;
        Project001::MeshData tempMeshData03;
        std::vector<glm::vec3> capCorners;
        FAIL_CHECK(Project001::Mesh::GenerateSphere(tempMeshData01, 0.32f, 8, 8));
        Project001::Mesh::SliceMeshWithAPlane(tempMeshData02, tempMeshData03, capCorners, tempMeshData01, glm::vec3(0.0f, 1.0f, 0.0f), -0.08f);
        Project001::Mesh::TranslateMesh(tempMeshData02, glm::vec3(0.0f, 0.04f, 0.0f));
        Project001::Mesh::TranslateMesh(tempMeshData03, glm::vec3(0.0f, -0.04f, 0.0f));
        Project001::Mesh::CopyMesh(*newMeshDataPtr, tempMeshData02);
        Project001::Mesh::CopyMesh(*newMeshDataPtr, tempMeshData03);

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(numbers12x6_TextureId);
        }
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);

        Project001::MeshData tempMeshData01;
        Project001::MeshData tempMeshData02;
        Project001::MeshData tempMeshData03;
        std::vector<glm::vec3> capCorners;
        FAIL_CHECK(Project001::Mesh::GenerateSphere(tempMeshData01, 0.32f, 8, 8));
        Project001::Mesh::SliceMeshWithAPlane(tempMeshData02, tempMeshData03, capCorners, tempMeshData01, glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
        Project001::Mesh::TranslateMesh(tempMeshData02, glm::vec3(0.0f, 0.04f, 0.0f));
        Project001::Mesh::TranslateMesh(tempMeshData03, glm::vec3(0.0f, -0.04f, 0.0f));
        Project001::Mesh::CopyMesh(*newMeshDataPtr, tempMeshData02);
        Project001::Mesh::CopyMesh(*newMeshDataPtr, tempMeshData03);

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(numbers12x6_TextureId);
        }
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);

        Project001::MeshData tempMeshData01;
        Project001::MeshData tempMeshData02;
        Project001::MeshData tempMeshData03;
        std::vector<glm::vec3> capCorners;
        FAIL_CHECK(Project001::Mesh::GenerateSphere(tempMeshData01, 0.32f, 8, 8));
        Project001::Mesh::SliceMeshWithAPlane(
            tempMeshData02, tempMeshData03, capCorners, tempMeshData01, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)), 0.16f);
        Project001::Mesh::TranslateMesh(tempMeshData02, glm::vec3(0.0f, 0.04f, 0.0f));
        Project001::Mesh::TranslateMesh(tempMeshData03, glm::vec3(0.0f, -0.04f, 0.0f));
        Project001::Mesh::CopyMesh(*newMeshDataPtr, tempMeshData02);
        Project001::Mesh::CopyMesh(*newMeshDataPtr, tempMeshData03);

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(numbers12x6_TextureId);
        }
    }

    {
        positionIndex++;
    }

    {
        positionIndex++;
    }

    {
        positionIndex++;
    }

    {
        positionIndex++;
    }

    // row 3 -------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::GenerateIceCreamCup(*newMeshDataPtr, 0.48f, 0.32f, 0.08f, 8, 8, false));
        Project001::Mesh::RotateMeshZ(*newMeshDataPtr, glm::half_pi<float>());

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(numbers12x6_TextureId);
        }
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);

        Project001::MeshData tempMeshData01;
        Project001::MeshData tempMeshData02;
        Project001::MeshData tempMeshData03;
        std::vector<glm::vec3> capCorners;
        FAIL_CHECK(Project001::Mesh::GenerateIceCreamCup(tempMeshData01, 0.48f, 0.32f, 0.08f, 8, 8, false));
        Project001::Mesh::RotateMeshZ(tempMeshData01, glm::half_pi<float>());
        Project001::Mesh::SliceMeshWithAPlane(tempMeshData02, tempMeshData03, capCorners, tempMeshData01, glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
        Project001::Mesh::TranslateMesh(tempMeshData02, glm::vec3(0.0f, 0.04f, 0.0f));
        Project001::Mesh::TranslateMesh(tempMeshData03, glm::vec3(0.0f, -0.04f, 0.0f));
        Project001::Mesh::CopyMesh(*newMeshDataPtr, tempMeshData02);
        Project001::Mesh::CopyMesh(*newMeshDataPtr, tempMeshData03);

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(numbers12x6_TextureId);
        }
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);

        Project001::MeshData tempMeshData01;
        Project001::MeshData tempMeshData02;
        Project001::MeshData tempMeshData03;
        std::vector<glm::vec3> capCorners;
        FAIL_CHECK(Project001::Mesh::GenerateIceCreamCup(tempMeshData01, 0.48f, 0.32f, 0.08f, 8, 8, false));
        Project001::Mesh::RotateMeshZ(tempMeshData01, glm::half_pi<float>());
        Project001::Mesh::SliceMeshWithAPlane(tempMeshData02, tempMeshData03, capCorners, tempMeshData01, glm::vec3(0.0f, 1.0f, 0.0f), -0.12f);
        Project001::Mesh::TranslateMesh(tempMeshData02, glm::vec3(0.0f, 0.04f, 0.0f));
        Project001::Mesh::TranslateMesh(tempMeshData03, glm::vec3(0.0f, -0.04f, 0.0f));
        Project001::Mesh::CopyMesh(*newMeshDataPtr, tempMeshData02);
        Project001::Mesh::CopyMesh(*newMeshDataPtr, tempMeshData03);

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(numbers12x6_TextureId);
        }
    }

    // Member Scenes -----------------------------------------------------------

    Project001::FontData font01_FontData;
    FAIL_CHECK(Project001::Font::LoadFontDataFromMemory(
        font01_FontData,
        g_AntonioRegular_ssf,
        sizeof(g_AntonioRegular_ssf)
    ));

    Project001::TextureData font01_TextureData;
    FAIL_CHECK(Project001::Texture::LoadTextureFromMemory(
        font01_TextureData,
        g_AntonioRegular_png,
        sizeof(g_AntonioRegular_png)
    ));
    unsigned int font01_TextureId = static_cast<unsigned int>(-1);
    GetRendererPtr()->CreateTexture(
        font01_TextureId,
        font01_TextureData.data,
        font01_TextureData.width,
        font01_TextureData.height,
        font01_TextureData.bytesPerPixel,
        true,
        false
    );

    const Project001::KeyCode keyCode_toggleInstructions = Project001::KeyCode::KEY_CODE_TAB;

    TestInstructionScene001::InitializationInfo instructionSceneInfo;
    instructionSceneInfo.hiddenInstructionString = std::string("Press <Tab> to show instructions.");
    instructionSceneInfo.instructionString = std::string(
        "This Scene tests Mesh Generation.\n"
        "Use <WASD> to move the camera up, left, down, and right.\n"
        "Use <Q> to roll left and <E> to roll right.\n"
        "Use <Scroll> to move forward and back.\n"
        "<Left-Click> and drag the <Mouse> to move camera.\n"
        "Press <Esc> to return to Main Menu.\n"
        "Press <Tab> to hide instructions."
    );
    instructionSceneInfo.fontDataPtr = &font01_FontData;
    instructionSceneInfo.fontTextureId = font01_TextureId;
    instructionSceneInfo.cameraEntityId = uiCameraEntityId_;
    instructionSceneInfo.cameraMask = s_uiCameraMask_;
    instructionSceneInfo.keyCode_toggleInstructions = keyCode_toggleInstructions;
    instructionScene_.Initialize(instructionSceneInfo);
}

void TestScene005::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    LOG_INFO("DEINITIALIZING: TestScene005:            " << GetId());
}

// private ---------------------------------------------------------------------

void TestScene005::TestOrderPointsCCW()
{
    std::vector<glm::vec3> test0_points =
    {
        {1, 0, 0},   // +X
        {0, -1, 0},  // -Y
        {-1, 0, 0},  // -X
        {0, 1, 0}    // +Y
    };

    glm::vec3 test0_axis(0, 0, 1); // Counterclockwise when viewed from above (+Z)
    glm::vec3 test0_startDir(1, 0, 0); // Start from +X direction

    // Expected output:
    // (1, 0, 0)
    // (0, 1, 0)
    // (-1, 0, 0)
    // (0, -1, 0)
    Project001::Math::OrderPointsCCW(test0_points, test0_axis, test0_startDir);

    std::vector<glm::vec3> test1_points =
    {
        {1, 0, 0},
        {0, 1, 0},
        {-1, 0, 0},
        {0, -1, 0}
    };

    glm::vec3 test1_axis(0, 0, 1);
    glm::vec3 test1_startDir(0, 1, 0); // Start from +Y instead of +X

    // Expected output:
    // (0, 1, 0)
    // (-1, 0, 0)
    // (0, -1, 0)
    // (1, 0, 0)
    Project001::Math::OrderPointsCCW(test1_points, test1_axis, test1_startDir);

    std::vector<glm::vec3> test2_points =
    {
        {0, 1, 0},   // +Y
        {1, 0, 0},   // +X
        {-1, 0, 0}   // -X
    };

    glm::vec3 test2_axis(0, 0, 1);
    glm::vec3 test2_startDir(1, 0, 0);

    // Expected output:
    // (0, 1, 0)
    // (-1, 0, 0)
    // (1, 0, 0)
    // Note that the first point is not (1, 0, 0) becasue the startDir
    // is the direction from the center of all the points not the origin,
    // so the (1, 0, 0) point ends up "below" the startDir.
    Project001::Math::OrderPointsCCW(test2_points, test2_axis, test2_startDir);

    std::vector<glm::vec2> test3_points =
    {
        {1, 0},
        {0, 1},
        {-1, 0},
        {0, -1}
    };

    // Expected output:
    // (1, 0)
    // (0, 1)
    // (-1, 0)
    // (0, -1)
    Project001::Math::OrderPointsCCW(test3_points, 0.0f);

    // Expected output:
    // (0, 1)
    // (-1, 0)
    // (0, -1)
    // (1, 0)
    Project001::Math::OrderPointsCCW(test3_points, glm::half_pi<float>());

    std::vector<glm::vec2> test4_points =
    {
        {1, 0}, {0, 1}, {-1, -1}
    };

    // Expected output:
    // (-1, -1) -> (1, 0) -> (0, 1)
    Project001::Math::OrderPointsCCW(test4_points, glm::pi<float>());

    int z = 0;
}

void TestScene005::TestRemoveDuplicates()
{
    std::vector<glm::vec3> test0_input;
    std::vector<glm::vec3> test0_output;
    Project001::Math::RemoveDuplicates(test0_output, test0_input);

    test0_input.emplace_back(0.0f, 0.0f, 0.0f);
    test0_input.emplace_back(1.0f, 0.0f, 0.0f);
    test0_input.emplace_back(1.0f, 1.0f, 0.0f);
    test0_input.emplace_back(1.0f, 1.0f, 1.0f);
    test0_input.emplace_back(2.0f, 1.0f, 1.0f);
    test0_input.emplace_back(2.0f, 2.0f, 1.0f);
    test0_input.emplace_back(2.0f, 2.0f, 2.0f);
    Project001::Math::RemoveDuplicates(test0_output, test0_input);

    std::vector<glm::vec3> test1_input;
    std::vector<glm::vec3> test1_output;
    test1_input.emplace_back(0.0f, 0.0f, 0.0f);
    test1_input.emplace_back(0.0f, 0.0f, 0.0f);
    test1_input.emplace_back(1.0f, 0.0f, 0.0f);
    test1_input.emplace_back(1.0f, 0.0f, 0.0f);
    test1_input.emplace_back(1.0f, 1.0f, 0.0f);
    test1_input.emplace_back(1.0f, 1.0f, 0.0f);
    test1_input.emplace_back(1.0f, 1.0f, 1.0f);
    test1_input.emplace_back(1.0f, 1.0f, 1.0f);
    test1_input.emplace_back(2.0f, 1.0f, 1.0f);
    test1_input.emplace_back(2.0f, 1.0f, 1.0f);
    test1_input.emplace_back(2.0f, 2.0f, 1.0f);
    test1_input.emplace_back(2.0f, 2.0f, 1.0f);
    test1_input.emplace_back(2.0f, 2.0f, 2.0f);
    test1_input.emplace_back(2.0f, 2.0f, 2.0f);
    Project001::Math::RemoveDuplicates(test1_output, test1_input);

    std::vector<glm::vec2> test2_input;
    std::vector<glm::vec2> test2_output;
    test2_input.emplace_back(0.0f, 0.0f);
    test2_input.emplace_back(0.0f, 0.0f);
    test2_input.emplace_back(1.0f, 0.0f);
    test2_input.emplace_back(1.0f, 0.0f);
    test2_input.emplace_back(1.0f, 1.0f);
    test2_input.emplace_back(1.0f, 1.0f);
    test2_input.emplace_back(1.0f, 1.0f);
    test2_input.emplace_back(1.0f, 1.0f);
    test2_input.emplace_back(2.0f, 1.0f);
    test2_input.emplace_back(2.0f, 1.0f);
    test2_input.emplace_back(2.0f, 2.0f);
    test2_input.emplace_back(2.0f, 2.0f);
    test2_input.emplace_back(2.0f, 2.0f);
    test2_input.emplace_back(2.0f, 2.0f);
    Project001::Math::RemoveDuplicates(test2_output, test2_input);

    int z = 0;
}