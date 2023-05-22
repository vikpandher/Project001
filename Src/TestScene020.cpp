#include "TestScene020.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/RenderedModel.h"
#include "Engine/Math/CoordinateSystems.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/MeshLoader.h"
#include "Engine/Renderer.h"
#include "Engine/SoundPlayer.h"
#include "Engine/SoundLoader.h"
#include "Engine/TextureLoader.h"
#include "Engine/Window.h"



// public ----------------------------------------------------------------------

TestScene020::TestScene020()
{
    ClearResources();

    // TestOpenAL();
}

TestScene020::~TestScene020()
{}

const char* TestScene020::Name()
{
    return "TestScene020";
}

// protected -------------------------------------------------------------------

bool TestScene020::OnInitialize()
{
    bool success = TestSceneBase001::OnInitialize();

    // Load meshes
    // -------------------------------------------------------------------------

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::LoadMeshOBJ(*newMeshDataPtr, "../Models/saucer.obj"));
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
        Project001::MeshLoader::NormalizeMeshSize(*newMeshDataPtr);
    }

    // Load sounds
    // -------------------------------------------------------------------------
    {
        Project001::SoundData soundData;
        _FAIL_CHECK(Project001::SoundLoader::LoadSoundWAV(soundData, "../Sounds/bounce.wav"));

        _FAIL_CHECK(soundPlayerPtr_->CreateSoundBuffer(
            song01SoundBufferId_,
            soundData.data,
            soundData.numberOfChannels,
            soundData.sampleRate,
            soundData.bitsPerSample,
            soundData.size));
    }

    // Calculating positions
    // -------------------------------------------------------------------------

    std::vector<glm::vec3> modelEntityPositions;
    for (int i = 2; i >= -2; --i)
    {
        for (int j = -3; j <= 3; ++j)
        {
            modelEntityPositions.emplace_back((float)j, (float)i, 0.0f);
        }
    }
    size_t positionPosition = 0;

    // reposition camera and listender
    {
        Project001::Camera* cameraPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));
        cameraPtr->SetPosition(0.0f, 0.0f, 2.0f);
    }

    // generated shape entity 01
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(1.28f, 0.0f, 0.0f);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[0]);

        _FAIL_CHECK(soundPlayerPtr_->CreateSoundSource(song01SoundSourceId_));
        _FAIL_CHECK(soundPlayerPtr_->LinkSoundBufferToSoundSource(song01SoundBufferId_, song01SoundSourceId_));
        _FAIL_CHECK(soundPlayerPtr_->UpdateSoundSource(
            song01SoundSourceId_,
            renderedModelPtr->GetPosition(),
            glm::vec3(0.0f, 0.0f, 0.0f),
            1.0f,
            0.5f,
            true
        ));
        soundPlayerPtr_->PlaySoundSource(song01SoundSourceId_);
    }

    return success && true;
}

bool TestScene020::OnDeinitialize()
{
    bool success = TestSceneBase001::OnDeinitialize();

    ClearResources();

    return success && true;
}

void TestScene020::OnHandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene020::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene020::ProcessUpdateEvent, this, std::placeholders::_1));

    TestSceneBase001::OnHandleEvent(event);
}

void TestScene020::ClearResources()
{
    song01SoundBufferId_ = (unsigned int)-1;

    song01SoundSourceId_ = (unsigned int)-1;
}

void TestScene020::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_X)
        {
            SendEvent(Project001::SwitchSceneEvent("TestScene002"));
            if (!IsActiveScene())
            {
                Deinitialize();
                SendEvent(Project001::InitializeSceneEvent("TestScene002"));
            }
        }
    }
}

void TestScene020::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    unsigned long timestep_ns = updateEvent.timestep_ns;

    // Update Entities
    UpdateShape01EntityPosition(timestep_ns);
}

void TestScene020::UpdateShape01EntityPosition(unsigned long timestep_ns)
{
    float timestep_s = (float)(timestep_ns / 1000000) / 1000;

    Project001::RenderedModel* renderedModelPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(entityIds_[0], renderedModelPtr));
    glm::vec3 currentPosition = renderedModelPtr->GetPosition();

    glm::vec2 currentPositionPolar = Project001::CartesianToPolar(currentPosition.x, currentPosition.y);
    currentPositionPolar.y += timestep_s;
    glm::vec2 newPosition = Project001::PolarToCartesian(currentPositionPolar);

    renderedModelPtr->SetPosition(newPosition.x, newPosition.y, 0.0f);

    glm::vec3 velocity((newPosition.x - currentPosition.x) / timestep_s, (newPosition.y - currentPosition.y) / timestep_s, 0.0f);

    soundPlayerPtr_->UpdateSoundSource(
        song01SoundSourceId_,
        renderedModelPtr->GetPosition(),
        velocity,
        1.0f,
        0.5f,
        true
    );
}

// private: --------------------------------------------------------------------

void TestScene020::TestOpenAL() const
{
    Project001::SoundData congratulationsSoundData;
    _FAIL_CHECK(Project001::SoundLoader::LoadSoundOGG(congratulationsSoundData, "../Sounds/f_congratulations.ogg"));

    Project001::SoundPlayer* soundPlayerPtr = Project001::SoundPlayer::Create();

    unsigned int congratulationsSoundId = (unsigned int)-1;
    _FAIL_CHECK(soundPlayerPtr->CreateSoundBuffer(
        congratulationsSoundId,
        congratulationsSoundData.data,
        congratulationsSoundData.numberOfChannels,
        congratulationsSoundData.sampleRate,
        congratulationsSoundData.bitsPerSample,
        congratulationsSoundData.size
    ));

    unsigned int soundSourceId01 = (unsigned int)-1;
    _FAIL_CHECK(soundPlayerPtr->CreateSoundSource(soundSourceId01));
    _FAIL_CHECK(soundPlayerPtr->LinkSoundBufferToSoundSource(congratulationsSoundId, soundSourceId01));
    _FAIL_CHECK(soundPlayerPtr->UpdateSoundSource(soundSourceId01, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f, false));
    _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(soundSourceId01));

    unsigned int i = 0;
    while (i < ((unsigned int)-1) / 128)
    {
        i++;
    }

    unsigned int soundSourceId02 = (unsigned int)-1;
    _FAIL_CHECK(soundPlayerPtr->CreateSoundSource(soundSourceId02));
    _FAIL_CHECK(soundPlayerPtr->LinkSoundBufferToSoundSource(congratulationsSoundId, soundSourceId02));
    _FAIL_CHECK(soundPlayerPtr->UpdateSoundSource(soundSourceId02, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f, false));
    _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(soundSourceId02));

    i = 0;
    while (i < ((unsigned int)-1) / 2)
    {
        i++;
    }

    delete soundPlayerPtr;
}
