#include "TestScene004.h"

#include "igl/decimate.h"
#include "igl/per_vertex_normals.h"
#include "igl/read_triangle_mesh.h"

#include "Engine/Components/RenderedModel.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/MeshStores.h"
#include "Engine/Renderer.h"
#include "Engine/TextureStores.h"
#include "Engine/Window.h"



// public: ---------------------------------------------------------------------

TestScene004::TestScene004()
{
    ClearIndiciesAndEntityIds();
}

TestScene004::~TestScene004()
{}

const char* TestScene004::Name()
{
    return "TestScene004";
}

void TestScene004::Initialize()
{
    TestSceneFramework::Initialize();

    // libigl
    // -------------------------------------------------------------------------
    std::string fileName("saucer");
    std::string inFilename("../Models/" + fileName + ".obj");
    Eigen::MatrixXd originalPositions;
    Eigen::MatrixXi originalFaces;
    igl::read_triangle_mesh(inFilename, originalPositions, originalFaces);
    // Eigen::MatrixXd originalFaceNormals;
    // igl::per_face_normals(originalPositions, originalFaces, originalFaceNormals);
    Eigen::MatrixXd originalVertexNormals;
    igl::per_vertex_normals(originalPositions, originalFaces, originalVertexNormals);

    // std::cout << "originalPositions = " << std::endl << "{" << std::endl << originalPositions << std::endl << "}" << std::endl;
    // std::cout << "originalFaces = " << std::endl << "{" << std::endl << originalFaces << std::endl << "}" << std::endl;
    // std::cout << "originalNormals = " << std::endl << "{" << std::endl << originalNormals << std::endl << "}" << std::endl;

    std::vector<glm::vec3> convertedPositions00;
    for (size_t i = 0; i < (size_t)originalPositions.rows(); ++i)
    {
        convertedPositions00.emplace_back((float)originalPositions(i, 0), (float)originalPositions(i, 1), (float)originalPositions(i, 2));
    }

    std::vector<glm::ivec2> convertedFaces00;
    for (size_t i = 0; i < (size_t)originalFaces.rows(); ++i)
    {
        convertedFaces00.emplace_back(originalFaces(i, 0), originalFaces(i, 0));
        convertedFaces00.emplace_back(originalFaces(i, 1), originalFaces(i, 1));
        convertedFaces00.emplace_back(originalFaces(i, 2), originalFaces(i, 2));
    }

    std::vector<glm::vec3> convertedNormals00;
    for (size_t i = 0; i < (size_t)originalVertexNormals.rows(); ++i)
    {
        convertedNormals00.emplace_back((float)originalVertexNormals(i, 0), (float)originalVertexNormals(i, 1), (float)originalVertexNormals(i, 2));
    }

    _FAIL_CHECK(meshStoresPtr_->LoadTriangleMesh(shape01MeshIndex_, convertedPositions00, convertedNormals00, convertedFaces00));

    Eigen::MatrixXd decimatedPositions01;
    Eigen::MatrixXi decimatedFaces01;
    Eigen::VectorXi indiciesIntoOriginalFaces01;
    igl::decimate(originalPositions, originalFaces, originalPositions.rows() * 3 / 4, decimatedPositions01, decimatedFaces01, indiciesIntoOriginalFaces01);
    Eigen::MatrixXd decimatedNormals01;
    igl::per_vertex_normals(decimatedPositions01, decimatedFaces01, decimatedNormals01);

    std::vector<glm::vec3> convertedPositions01;
    for (size_t i = 0; i < (size_t)decimatedPositions01.rows(); ++i)
    {
        convertedPositions01.emplace_back((float)decimatedPositions01(i, 0), (float)decimatedPositions01(i, 1), (float)decimatedPositions01(i, 2));
    }

    std::vector<glm::ivec2> convertedFaces01;
    for (size_t i = 0; i < (size_t)decimatedFaces01.rows(); ++i)
    {
        convertedFaces01.emplace_back(decimatedFaces01(i, 0), decimatedFaces01(i, 0));
        convertedFaces01.emplace_back(decimatedFaces01(i, 1), decimatedFaces01(i, 1));
        convertedFaces01.emplace_back(decimatedFaces01(i, 2), decimatedFaces01(i, 2));
    }

    std::vector<glm::vec3> convertedNormals01;
    for (size_t i = 0; i < (size_t)decimatedNormals01.rows(); ++i)
    {
        convertedNormals01.emplace_back((float)decimatedNormals01(i, 0), (float)decimatedNormals01(i, 1), (float)decimatedNormals01(i, 2));
    }

    _FAIL_CHECK(meshStoresPtr_->LoadTriangleMesh(shape02MeshIndex_, convertedPositions01, convertedNormals01, convertedFaces01));

    // generated shape entity 01
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape01EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape01EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape01EntityId_, renderedModelPtr));
        renderedModelPtr->SetMeshIndex(shape02MeshIndex_);
    }
}

void TestScene004::Deinitialize()
{
    TestSceneFramework::Deinitialize();

    ClearIndiciesAndEntityIds();
}

void TestScene004::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene004::ProcessKeyEvent, this, std::placeholders::_1));

    TestSceneFramework::OnEvent(event);
}

// protected: ------------------------------------------------------------------

void TestScene004::ClearIndiciesAndEntityIds()
{
    shape01MeshIndex_ = (unsigned int)-1;
    shape02MeshIndex_ = (unsigned int)-1;

    shape01EntityId_ = (unsigned int)-1;
}

void TestScene004::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_X)
        {
            SendEvent(Project001::SwitchSceneEvent("TestScene001"));
            if (!IsActiveScene())
            {
                Deinitialize();
                SendEvent(Project001::InitializeSceneEvent("TestScene001"));
            }
        }
    }
}
