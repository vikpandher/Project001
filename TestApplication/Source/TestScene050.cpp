#include "TestScene050.h"

#include "TestResource_Bounce_wav.h"
#include "TestResource_Congratulations_ogg.h"

#include "Components/Camera.h"
#include "Components/RenderedMesh.h"
#include "Math/CoordinateSystems.h"
#include "Math/VectorUtilities.h"
#include "Application.h"
#include "ComponentStores.h"
#include "FreetypeTextLoader.h"
#include "Logger.h"
#include "MeshLoader.h"
#include "Renderer.h"
#include "SoundPlayer.h"
#include "SoundLoader.h"
#include "TextureLoader.h"
#include "Window.h"



// public ----------------------------------------------------------------------

TestScene050::TestScene050(Project001::Application* applicationPtr)
    : TestSceneBase001(applicationPtr)
    , instructionScene_(applicationPtr)
    , sound01_SoundDataPtr_(nullptr)
    , soundBufferId01_((unsigned int)-1)
    , soundSourceId01_((unsigned int)-1)
    , sound02_SoundDataPtr_(nullptr)
    , soundBufferId02_((unsigned int)-1)
    , soundSourceId02_((unsigned int)-1)
    , playingSound_(false)
{
    // TestSoundPlayer();
    // TestSoundPlayer2();
    // TestSoundPlayer3();
}

TestScene050::~TestScene050()
{}

void TestScene050::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene050::ProcessDeinitializeEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene050::ProcessUpdateEvent, this, std::placeholders::_1));

    TestSceneBase001::HandleEvent(event);

    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene050::ProcessInitializeEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene050::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    _LOG_MESSAGE("INITIALIZING:   TestScene050:            %u", GetId());

    // Load Meshes
    // -------------------------------------------------------------------------

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere(*newMeshDataPtr, 0.16f, 0, false));
    }

    // Load Sounds
    // -------------------------------------------------------------------------

    sound01_SoundDataPtr_ = new Project001::SoundData();
    _FAIL_CHECK(Project001::SoundLoader::LoadSoundOGG(*sound01_SoundDataPtr_, "../Sounds/Congratulations.ogg"));

    _FAIL_CHECK(soundPlayerPtr_->CreateSoundBuffer(
        soundBufferId01_,
        sound01_SoundDataPtr_->data,
        sound01_SoundDataPtr_->sizeInBytes,
        sound01_SoundDataPtr_->numberOfChannels,
        sound01_SoundDataPtr_->sampleRate_Hz,
        sound01_SoundDataPtr_->bitsPerSample,
        sound01_SoundDataPtr_->sizeInFrames
    ));

    _FAIL_CHECK(soundPlayerPtr_->CreateSoundSource(
        soundSourceId01_,
        soundBufferId01_
    ));

    sound02_SoundDataPtr_ = new Project001::SoundData();
    _FAIL_CHECK(Project001::SoundLoader::LoadSoundWAV(*sound02_SoundDataPtr_, "../Sounds/Bounce.wav"));

    _FAIL_CHECK(soundPlayerPtr_->CreateSoundBuffer(
        soundBufferId02_,
        sound02_SoundDataPtr_->data,
        sound02_SoundDataPtr_->sizeInBytes,
        sound02_SoundDataPtr_->numberOfChannels,
        sound02_SoundDataPtr_->sampleRate_Hz,
        sound02_SoundDataPtr_->bitsPerSample,
        sound02_SoundDataPtr_->sizeInFrames
    ));

    _FAIL_CHECK(soundPlayerPtr_->CreateSoundSource(
        soundSourceId02_,
        soundBufferId02_
    ));

    _FAIL_CHECK(soundPlayerPtr_->SetSoundSourceLooping(soundSourceId02_, true));

    // Generated Shape Entity 01
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        renderedMeshPtr->SetPosition(0.0f, 0.0f, 0.0f);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[0]);
    }

    // Member Scenes -----------------------------------------------------------

    Project001::FontData font01_FontData;
    Project001::TextureData font01_TextureData;
    unsigned int font01_TextureId = (unsigned int)-1;
    std::vector<unsigned char> characterList;
    for (unsigned char c = 32; c < 127; ++c) // ASCII characters
    {
        characterList.push_back(c);
    }
    _FAIL_CHECK(Project001::FreetypeTextLoader::LoadTextureDataAndFontData(
        font01_TextureData,
        font01_FontData,
        characterList,
        "../Fonts/Antonio-Regular.ttf",
        48
    ));
    rendererPtr_->CreateTexture(
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
        "This Scene tests playing a spacial sound. The sound's\n"
        "source is the Icosphere in the center.\n"
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

void TestScene050::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    _LOG_MESSAGE("DEINITIALIZING: TestScene050:            %u", GetId());

    // SoundData ---------------------------------------------------------------

    delete sound01_SoundDataPtr_;
    sound01_SoundDataPtr_ = nullptr;

    soundBufferId01_ = (unsigned int)-1;
    soundSourceId01_ = (unsigned int)-1;

    delete sound02_SoundDataPtr_;
    sound02_SoundDataPtr_ = nullptr;

    soundBufferId02_ = (unsigned int)-1;
    soundSourceId02_ = (unsigned int)-1;

    playingSound_ = false;
}

void TestScene050::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    unsigned long long timestep_ns = updateEvent.timestep_ns;

    UpdateCameraListenerPosition();
    // UpdateShape01EntityPosition(timestep_ns);

    if (!playingSound_)
    {
        soundPlayerPtr_->PlaySoundSource(soundSourceId02_);
        playingSound_ = true;
    }
}

void TestScene050::UpdateCameraListenerPosition()
{
    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(cameraPtr, mainCameraEntityId_));
    glm::vec3 currentPosition = cameraPtr->GetPosition();
    glm::vec3 currentForward = cameraPtr->GetForwardVector();
    glm::vec3 currentUp = cameraPtr->GetUpVector();

    soundPlayerPtr_->SetListenerPosition(currentPosition);
    soundPlayerPtr_->SetListenerForwardDirection(currentForward);
    soundPlayerPtr_->SetListenerUpDirection(currentUp);
}

void TestScene050::UpdateShape01EntityPosition(unsigned long long timestep_ns)
{
    float timestep_s = (float)(timestep_ns / 1000000) / 1000;

    Project001::RenderedMesh* renderedMeshPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, entityIds_[0]));
    glm::vec3 currentPosition = renderedMeshPtr->GetPosition();

    if (currentPosition == glm::vec3(0.0f, 0.0f, 0.0f))
    {
        currentPosition.x = 1.28f;
    }

    glm::vec2 currentPositionPolar = Project001::CartesianToPolar(currentPosition.x, currentPosition.y);
    currentPositionPolar.y += timestep_s;
    glm::vec2 newPosition = Project001::PolarToCartesian(currentPositionPolar);

    renderedMeshPtr->SetPosition(newPosition.x, newPosition.y, 0.0f);

    glm::vec3 velocity((newPosition.x - currentPosition.x) / timestep_s, (newPosition.y - currentPosition.y) / timestep_s, 0.0f);

    _FAIL_CHECK(soundPlayerPtr_->SetSoundSourcePosition(soundSourceId02_, renderedMeshPtr->GetPosition()));
    _FAIL_CHECK(soundPlayerPtr_->SetSoundSourceVelocity(soundSourceId02_, velocity));
}

// private ---------------------------------------------------------------------

void TestScene050::TestSoundPlayer()
{
    Project001::SoundPlayer* soundPlayerPtr = Project001::SoundPlayer::Create();

    Project001::SoundData soundData01;
    _FAIL_CHECK(Project001::SoundLoader::LoadSoundOGG(soundData01, "../Sounds/Congratulations.ogg"));

    unsigned int soundBufferId01;
    _FAIL_CHECK(soundPlayerPtr->CreateSoundBuffer(
        soundBufferId01,
        soundData01.data,
        soundData01.sizeInBytes,
        soundData01.numberOfChannels,
        soundData01.sampleRate_Hz,
        soundData01.bitsPerSample,
        soundData01.sizeInFrames
    ));

    unsigned int soundSourceId01;
    _FAIL_CHECK(soundPlayerPtr->CreateSoundSource(
        soundSourceId01,
        soundBufferId01
    ));

    Project001::SoundData soundData02;
    _FAIL_CHECK(Project001::SoundLoader::LoadSoundWAV(soundData02, "../Sounds/Bounce.wav"));

    unsigned int soundBufferId02;
    _FAIL_CHECK(soundPlayerPtr->CreateSoundBuffer(
        soundBufferId02,
        soundData02.data,
        soundData02.sizeInBytes,
        soundData02.numberOfChannels,
        soundData02.sampleRate_Hz,
        soundData02.bitsPerSample,
        soundData02.sizeInFrames
    ));

    unsigned int soundSourceId02;
    _FAIL_CHECK(soundPlayerPtr->CreateSoundSource(
        soundSourceId02,
        soundBufferId02
    ));

    // =========================================================================

    // Playing sound 01 in the center

    _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(soundSourceId01));

    std::this_thread::sleep_for(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<float>(soundData01.duration_s)
        )
    );

    // Playing sound 01 for half time, pausing, then playing

    _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(soundSourceId01));

    std::this_thread::sleep_for(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<float>(soundData01.duration_s * 0.5f)
        )
    );

    _FAIL_CHECK(soundPlayerPtr->PauseSoundSource(soundSourceId01));

    std::this_thread::sleep_for(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<float>(soundData01.duration_s * 0.5f)
        )
    );

    _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(soundSourceId01));

    std::this_thread::sleep_for(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<float>(soundData01.duration_s * 0.5f)
        )
    );

    // Playing the sound01 for half time, then stopping and playing it again

    _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(soundSourceId01));

    std::this_thread::sleep_for(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<float>(soundData01.duration_s * 0.5f)
        )
    );

    _FAIL_CHECK(soundPlayerPtr->StopSoundSource(soundSourceId01));

    std::this_thread::sleep_for(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<float>(soundData01.duration_s * 0.5f)
        )
    );

    _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(soundSourceId01));

    std::this_thread::sleep_for(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<float>(soundData01.duration_s)
        )
    );

    // Playing sound 02 in the center

    _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(soundSourceId02));

    std::this_thread::sleep_for(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<float>(soundData02.duration_s * 2.0f)
        )
    );

    // Play sound 01 going clockwise starting at 12

    glm::vec2 positionVector(0.0f, 10.0f);

    for (size_t i = 0; i < 9; ++i)
    {
        soundPlayerPtr->SetSoundSourcePosition(soundSourceId01, glm::vec3(positionVector, 0.0f));

        _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(soundSourceId01));

        std::this_thread::sleep_for(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::duration<float>(soundData01.duration_s)
            )
        );

        positionVector = Project001::Rotate2DVector(positionVector, -glm::quarter_pi<float>());
    }

    // Playing sound 02 in the center

    _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(soundSourceId02));

    std::this_thread::sleep_for(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<float>(soundData02.duration_s * 2.0f)
        )
    );

    soundPlayerPtr->SetSoundSourcePosition(soundSourceId01, glm::vec3(0.0f, 0.0f, 0.0f));

    // Play sound 01 with the listener going clockwise starting at 6

    positionVector = glm::vec2(0.0f, -10.0f);

    for (size_t i = 0; i < 9; ++i)
    {
        soundPlayerPtr->SetListenerPosition(glm::vec3(positionVector, 0.0f));

        _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(soundSourceId01));

        std::this_thread::sleep_for(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::duration<float>(soundData01.duration_s)
            )
        );

        positionVector = Project001::Rotate2DVector(positionVector, -glm::quarter_pi<float>());
    }

    soundPlayerPtr->SetListenerPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    // Playing sound 02 in the center

    _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(soundSourceId02));

    std::this_thread::sleep_for(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<float>(soundData02.duration_s * 2.0f)
        )
    );

    // std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    delete soundPlayerPtr;
}

void TestScene050::TestSoundPlayer2()
{

    Project001::SoundPlayer* soundPlayerPtr = Project001::SoundPlayer::Create();

    Project001::SoundData sound01_SoundData;
    _FAIL_CHECK(Project001::SoundLoader::LoadSoundOGG(sound01_SoundData, "../Sounds/Congratulations.ogg"));

    unsigned int sound01_SoundBufferId;
    _FAIL_CHECK(soundPlayerPtr->CreateSoundBuffer(
        sound01_SoundBufferId,
        sound01_SoundData.data,
        sound01_SoundData.sizeInBytes,
        sound01_SoundData.numberOfChannels,
        sound01_SoundData.sampleRate_Hz,
        sound01_SoundData.bitsPerSample,
        sound01_SoundData.sizeInFrames
    ));

    unsigned int sound01_SoundSourceId01;
    _FAIL_CHECK(soundPlayerPtr->CreateSoundSource(
        sound01_SoundSourceId01,
        sound01_SoundBufferId
    ));

    unsigned int sound01_SoundSourceId02;
    _FAIL_CHECK(soundPlayerPtr->CreateSoundSource(
        sound01_SoundSourceId02,
        sound01_SoundBufferId
    ));

    // Project001::SoundData sound02_SoundData;
    // _FAIL_CHECK(Project001::SoundLoader::LoadSoundOGG(sound02_SoundData, "../Sounds/perfect.ogg"));
    // 
    // unsigned int sound02_SoundBufferId;
    // _FAIL_CHECK(soundPlayerPtr->CreateSoundBuffer(
    //     sound02_SoundBufferId,
    //     sound02_SoundData.data,
    //     sound02_SoundData.sizeInBytes,
    //     sound02_SoundData.numberOfChannels,
    //     sound02_SoundData.sampleRate_Hz,
    //     sound02_SoundData.bitsPerSample,
    //     sound02_SoundData.sizeInFrames
    // ));
    // 
    // unsigned int sound02_SoundSourceId;
    // _FAIL_CHECK(soundPlayerPtr->CreateSoundSource(
    //     sound02_SoundSourceId,
    //     sound02_SoundBufferId
    // ));

    // =========================================================================

    //

    _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(sound01_SoundSourceId01));

    std::this_thread::sleep_for(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<float>(sound01_SoundData.duration_s * 0.2f)
        )
    );

    //

    _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(sound01_SoundSourceId02));

    std::this_thread::sleep_for(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<float>(sound01_SoundData.duration_s)
        )
    );

    //

    // _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(sound02_SoundSourceId));
    // 
    // std::this_thread::sleep_for(
    //     std::chrono::duration_cast<std::chrono::milliseconds>(
    //         std::chrono::duration<float>(sound02_SoundData.duration_s)
    //     )
    // );

    delete soundPlayerPtr;
}

void TestScene050::TestSoundPlayer3()
{
    Project001::SoundPlayer* soundPlayerPtr = Project001::SoundPlayer::Create();

    Project001::SoundData sound01_SoundData;
    _FAIL_CHECK(Project001::SoundLoader::LoadSoundWAVFromMemory(sound01_SoundData, g_Bounce_wav, sizeof(g_Bounce_wav)/sizeof(unsigned char)));

    unsigned int sound01_SoundBufferId;
    _FAIL_CHECK(soundPlayerPtr->CreateSoundBuffer(
        sound01_SoundBufferId,
        sound01_SoundData.data,
        sound01_SoundData.sizeInBytes,
        sound01_SoundData.numberOfChannels,
        sound01_SoundData.sampleRate_Hz,
        sound01_SoundData.bitsPerSample,
        sound01_SoundData.sizeInFrames
    ));

    unsigned int sound01_SoundSourceId01;
    _FAIL_CHECK(soundPlayerPtr->CreateSoundSource(
        sound01_SoundSourceId01,
        sound01_SoundBufferId
    ));

    Project001::SoundData sound02_SoundData;
    _FAIL_CHECK(Project001::SoundLoader::LoadSoundOGGFromMemory(sound02_SoundData, g_Congratulations_ogg, sizeof(g_Congratulations_ogg)/sizeof(unsigned char)));

    unsigned int sound02_SoundBufferId;
    _FAIL_CHECK(soundPlayerPtr->CreateSoundBuffer(
        sound02_SoundBufferId,
        sound02_SoundData.data,
        sound02_SoundData.sizeInBytes,
        sound02_SoundData.numberOfChannels,
        sound02_SoundData.sampleRate_Hz,
        sound02_SoundData.bitsPerSample,
        sound02_SoundData.sizeInFrames
    ));

    unsigned int sound02_SoundSourceId01;
    _FAIL_CHECK(soundPlayerPtr->CreateSoundSource(
        sound02_SoundSourceId01,
        sound02_SoundBufferId
    ));

    // Playing sound01

    _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(sound01_SoundSourceId01));

    std::this_thread::sleep_for(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<float>(sound01_SoundData.duration_s)
        )
    );

    // Playing sound02

    _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(sound02_SoundSourceId01));

    std::this_thread::sleep_for(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<float>(sound02_SoundData.duration_s)
        )
    );

    delete soundPlayerPtr;
}