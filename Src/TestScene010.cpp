#include "TestScene010.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/RenderedModel.h"
#include "Engine/Math/CoordinateSystems.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/MeshStores.h"
#include "Engine/Renderer.h"
#include "Engine/SoundPlayer.h"
#include "Engine/SoundStores.h"
#include "Engine/TextureStores.h"
#include "Engine/Window.h"



// public: ---------------------------------------------------------------------

TestScene010::TestScene010()
{
    ClearIndiciesAndEntityIds();

    // TestOpenAL();
}

TestScene010::~TestScene010()
{}

const char* TestScene010::Name()
{
    return "TestScene010";
}

void TestScene010::Initialize()
{
    TestSceneBase001::Initialize();

    // Load meshes
    // -------------------------------------------------------------------------

    _FAIL_CHECK(meshStoresPtr_->LoadMeshOBJ(shape01MeshIndex_, "../Models/saucer.obj"));
    _FAIL_CHECK(meshStoresPtr_->RecenterMesh(shape01MeshIndex_));
    _FAIL_CHECK(meshStoresPtr_->NormalizeMeshSize(shape01MeshIndex_));

    // Load sounds
    // -------------------------------------------------------------------------

    _FAIL_CHECK(soundStoresPtr_->LoadSoundWAV(song01SoundIndex_, "../Sounds/bounce.wav"));

    Project001::SoundData song01SoundData;
    _FAIL_CHECK(soundStoresPtr_->GetSound(song01SoundIndex_, song01SoundData));

    _FAIL_CHECK(soundPlayerPtr_->CreateSoundBuffer(
        song01SoundIndex_,
        song01SoundData.data,
        song01SoundData.numberOfChannels,
        song01SoundData.sampleRate,
        song01SoundData.bitsPerSample,
        song01SoundData.size
    ));

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
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape01EntityId_));

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape01EntityId_));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape01EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(1.28f, 0.0f, 0.0f);
        renderedModelPtr->SetMeshIndex(shape01MeshIndex_);

        unsigned int soundSourceIndex;
        _FAIL_CHECK(soundPlayerPtr_->CreateSoundSource(soundSourceIndex));
        _FAIL_CHECK(soundPlayerPtr_->LinkSoundBufferToSoundSource(song01SoundIndex_, soundSourceIndex));
        _FAIL_CHECK(soundPlayerPtr_->UpdateSoundSource(
            soundSourceIndex,
            renderedModelPtr->GetPosition(),
            glm::vec3(0.0f, 0.0f, 0.0f),
            1.0f,
            0.5f,
            true
        ));
        soundPlayerPtr_->PlaySoundSource(soundSourceIndex);
    }
}

void TestScene010::Deinitialize()
{
    TestSceneBase001::Deinitialize();

    ClearIndiciesAndEntityIds();
}

void TestScene010::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene010::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene010::ProcessUpdateEvent, this, std::placeholders::_1));

    TestSceneBase001::OnEvent(event);
}

// protected: ------------------------------------------------------------------

void TestScene010::ClearIndiciesAndEntityIds()
{
    shape01MeshIndex_ = (unsigned int)-1;

    song01SoundIndex_ = (unsigned int)-1;

    shape01EntityId_ = (unsigned int)-1;
}

void TestScene010::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
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

void TestScene010::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    unsigned long timestep_ns = updateEvent.timestep_ns;

    // Update Entities
    UpdateShape01EntityPosition(timestep_ns);
}

void TestScene010::UpdateShape01EntityPosition(unsigned long timestep_ns)
{
    float timestep_s = (float)(timestep_ns / 1000000) / 1000;

    Project001::RenderedModel* renderedModelPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape01EntityId_, renderedModelPtr));
    glm::vec3 currentPosition = renderedModelPtr->GetPosition();

    glm::vec2 currentPositionPolar = Project001::CartesianToPolar(currentPosition.x, currentPosition.y);
    currentPositionPolar.y += timestep_s;
    glm::vec2 newPosition = Project001::PolarToCartesian(currentPositionPolar);

    renderedModelPtr->SetPosition(newPosition.x, newPosition.y, 0.0f);

    glm::vec3 velocity((newPosition.x - currentPosition.x) / timestep_s, (newPosition.y - currentPosition.y) / timestep_s, 0.0f);

    soundPlayerPtr_->UpdateSoundSource(
        song01SoundIndex_,
        renderedModelPtr->GetPosition(),
        velocity,
        1.0f,
        0.5f,
        true
    );
}

// private: --------------------------------------------------------------------

void TestScene010::TestOpenAL() const
{
    Project001::SoundStores* soundStoresPtr = new Project001::SoundStores();

    unsigned int congratulationsSoundIndex_;
    _FAIL_CHECK(soundStoresPtr->LoadSoundOGG(congratulationsSoundIndex_, "../Sounds/f_congratulations.ogg"));

    Project001::SoundData congratulationsSoundData;
    _FAIL_CHECK(soundStoresPtr->GetSound(congratulationsSoundIndex_, congratulationsSoundData));

    Project001::SoundPlayer* soundPlayerPtr = Project001::SoundPlayer::Create();

    _FAIL_CHECK(soundPlayerPtr->CreateSoundBuffer(
        congratulationsSoundIndex_,
        congratulationsSoundData.data,
        congratulationsSoundData.numberOfChannels,
        congratulationsSoundData.sampleRate,
        congratulationsSoundData.bitsPerSample,
        congratulationsSoundData.size
    ));

    unsigned int soundSourceIndex01_ = (unsigned int)-1;
    _FAIL_CHECK(soundPlayerPtr->CreateSoundSource(soundSourceIndex01_));
    _FAIL_CHECK(soundPlayerPtr->LinkSoundBufferToSoundSource(congratulationsSoundIndex_, soundSourceIndex01_));
    _FAIL_CHECK(soundPlayerPtr->UpdateSoundSource(soundSourceIndex01_, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f, false));
    _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(soundSourceIndex01_));

    unsigned int i = 0;
    while (i < ((unsigned int)-1) / 128)
    {
        i++;
    }

    unsigned int soundSourceIndex02_ = (unsigned int)-1;
    _FAIL_CHECK(soundPlayerPtr->CreateSoundSource(soundSourceIndex02_));
    _FAIL_CHECK(soundPlayerPtr->LinkSoundBufferToSoundSource(congratulationsSoundIndex_, soundSourceIndex02_));
    _FAIL_CHECK(soundPlayerPtr->UpdateSoundSource(soundSourceIndex02_, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f, false));
    _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(soundSourceIndex02_));

    i = 0;
    while (i < ((unsigned int)-1) / 2)
    {
        i++;
    }

    soundStoresPtr->ClearSounds();

    delete soundStoresPtr;
    delete soundPlayerPtr;
}
