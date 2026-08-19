// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-08-18

#pragma once

#include "glm/gtc/constants.hpp"

#include "Event.h"



namespace Project001
{
    struct FontData;
    struct MeshData;
    struct SoundData;
    struct TextureData;

    class Window;
}

struct ControlSchemeInfo
{
    static constexpr size_t s_controlSchemeIndex_unknown = 0;
    static constexpr size_t s_controlSchemeIndex_keyboard1 = 1;
    static constexpr size_t s_controlSchemeIndex_keyboard2 = 2;
    static constexpr size_t s_controlSchemeIndex_controller1 = 3;
    static constexpr size_t s_controlSchemeIndex_controller2 = 4;
    static constexpr size_t s_controlSchemeIndex_controller3 = 5;
    static constexpr size_t s_controlSchemeIndex_controller4 = 6;

    static const char* ControlSchemeIndexToString(size_t controlSchemeIndex);
    static size_t StringToControlSchemeIndex(const std::string& str);

    float axisDeadzone = 0.2f;

    unsigned int pause_pressCount = 0;
    unsigned int left_pressCount = 0;
    unsigned int right_pressCount = 0;
    unsigned int up_pressCount = 0;
    unsigned int down_pressCount = 0;
    unsigned int grab_pressCount = 0;
    unsigned int drop_pressCount = 0;
    float leftRightAxisValue = 0.0f;
    float upDownAxisValue = 0.0f;
};

struct PlayerCreationInfo
{
    size_t playerNumber = 0;
    bool turnedOn = true;

    size_t controlSchemeIndex = 0;

    float spawnPositionX = 0.0f;
    float spawnPositionY = 0.0f;
    float spawnRotation = 0.0f;

    bool dead = false;
};

struct SharedApplicationData
{
    // Scene Ids ---------------------------------------------------------------

    unsigned int scene001Id = static_cast<unsigned int>(-1);
    unsigned int scene002Id = static_cast<unsigned int>(-1);
    unsigned int scene003Id = static_cast<unsigned int>(-1);

    // Game Info ---------------------------------------------------------------

    unsigned int score = 0;
    float gameTime_s = 0.0f;

    static constexpr size_t s_controlScheme_count = 7;
    ControlSchemeInfo controlSchemeInfos[s_controlScheme_count] = {};

    static constexpr size_t s_player_count = 4;
    PlayerCreationInfo playerCreationInfos[s_player_count] = {
        {0, true, ControlSchemeInfo::s_controlSchemeIndex_keyboard1, -64.0f, 64.0f, -0.75f * glm::pi<float>()},
        {1, true, ControlSchemeInfo::s_controlSchemeIndex_keyboard2, 64.0f, 64.0f, -1.25f * glm::pi<float>()},
        {2, true, ControlSchemeInfo::s_controlSchemeIndex_controller1, 64.0f, -64.0f, -1.75f * glm::pi<float>()},
        {3, true, ControlSchemeInfo::s_controlSchemeIndex_controller2, -64.0f, -64.0f, -2.25f * glm::pi<float>()}
    };

    void UpdateKeyboardButtonPresses(const Project001::KeyEvent& keyEvent);
    void UpdateMouseButtonPresses(const Project001::MouseButtonEvent& mouseButtonEvent);
    void UpdateButtonPressCounts(const Project001::Window* windowPtr);

    // Player Controls ---------------------------------------------------------

    Project001::KeyCode keyboard_1_pause_keyCode = Project001::KeyCode::KEY_CODE_ENTER;
    Project001::KeyCode keyboard_1_left_keyCode = Project001::KeyCode::KEY_CODE_A;
    Project001::KeyCode keyboard_1_right_keyCode = Project001::KeyCode::KEY_CODE_D;
    Project001::KeyCode keyboard_1_up_keyCode = Project001::KeyCode::KEY_CODE_W;
    Project001::KeyCode keyboard_1_down_keyCode = Project001::KeyCode::KEY_CODE_S;
    Project001::KeyCode keyboard_1_grab_keyCode = Project001::KeyCode::KEY_CODE_SPACE;
    Project001::KeyCode keyboard_1_drop_keyCode = Project001::KeyCode::KEY_CODE_RIGHT_ALT;

    bool keyboard_1_pause_pressed = false;
    bool keyboard_1_left_pressed = false;
    bool keyboard_1_right_pressed = false;
    bool keyboard_1_up_pressed = false;
    bool keyboard_1_down_pressed = false;
    bool keyboard_1_grab_pressed = false;
    bool keyboard_1_drop_pressed = false;

    Project001::KeyCode keyboard_2_pause_keyCode = Project001::KeyCode::KEY_CODE_KP_ENTER;
    Project001::KeyCode keyboard_2_left_keyCode = Project001::KeyCode::KEY_CODE_LEFT;
    Project001::KeyCode keyboard_2_right_keyCode = Project001::KeyCode::KEY_CODE_RIGHT;
    Project001::KeyCode keyboard_2_up_keyCode = Project001::KeyCode::KEY_CODE_UP;
    Project001::KeyCode keyboard_2_down_keyCode = Project001::KeyCode::KEY_CODE_DOWN;
    Project001::KeyCode keyboard_2_grab_keyCode = Project001::KeyCode::KEY_CODE_KP_0;
    Project001::KeyCode keyboard_2_drop_keyCode = Project001::KeyCode::KEY_CODE_KP_DECIMAL;

    bool keyboard_2_pause_pressed = false;
    bool keyboard_2_left_pressed = false;
    bool keyboard_2_right_pressed = false;
    bool keyboard_2_up_pressed = false;
    bool keyboard_2_down_pressed = false;
    bool keyboard_2_grab_pressed = false;
    bool keyboard_2_drop_pressed = false;

    unsigned int controller_1_pause_buttonIndex = 7;
    unsigned int controller_1_left_buttonIndex = 13;
    unsigned int controller_1_right_buttonIndex = 11;
    unsigned int controller_1_up_buttonIndex = 10;
    unsigned int controller_1_down_buttonIndex = 12;
    unsigned int controller_1_grab_buttonIndex = 0;
    unsigned int controller_1_drop_buttonIndex = 1;
    unsigned int controller_1_moveRightLeft_axisIndex = 0;
    unsigned int controller_1_moveDownUp_axisIndex = 1;
    float controller_1_axisDeadzone = 0.2f;

    unsigned int controller_2_pause_buttonIndex = 7;
    unsigned int controller_2_left_buttonIndex = 13;
    unsigned int controller_2_right_buttonIndex = 11;
    unsigned int controller_2_up_buttonIndex = 10;
    unsigned int controller_2_down_buttonIndex = 12;
    unsigned int controller_2_grab_buttonIndex = 0;
    unsigned int controller_2_drop_buttonIndex = 1;
    unsigned int controller_2_moveRightLeft_axisIndex = 0;
    unsigned int controller_2_moveDownUp_axisIndex = 1;
    float controller_2_axisDeadzone = 0.2f;

    unsigned int controller_3_pause_buttonIndex = 7;
    unsigned int controller_3_left_buttonIndex = 13;
    unsigned int controller_3_right_buttonIndex = 11;
    unsigned int controller_3_up_buttonIndex = 10;
    unsigned int controller_3_down_buttonIndex = 12;
    unsigned int controller_3_grab_buttonIndex = 0;
    unsigned int controller_3_drop_buttonIndex = 1;
    unsigned int controller_3_moveRightLeft_axisIndex = 0;
    unsigned int controller_3_moveDownUp_axisIndex = 1;
    float controller_3_axisDeadzone = 0.2f;

    unsigned int controller_4_pause_buttonIndex = 7;
    unsigned int controller_4_left_buttonIndex = 13;
    unsigned int controller_4_right_buttonIndex = 11;
    unsigned int controller_4_up_buttonIndex = 10;
    unsigned int controller_4_down_buttonIndex = 12;
    unsigned int controller_4_grab_buttonIndex = 0;
    unsigned int controller_4_drop_buttonIndex = 1;
    unsigned int controller_4_moveRightLeft_axisIndex = 0;
    unsigned int controller_4_moveDownUp_axisIndex = 1;
    float controller_4_axisDeadzone = 0.2f;

    static constexpr Project001::KeyCode debug_keyboard_toggleDebugCamera_keyCode = Project001::KeyCode::KEY_CODE_0;
    static constexpr Project001::KeyCode debug_keyboard_toggleCameraLock_keyCode = Project001::KeyCode::KEY_CODE_KP_5;
    static constexpr Project001::KeyCode debug_keyboard_setCameraPitch1_keyCode = Project001::KeyCode::KEY_CODE_KP_7;
    static constexpr Project001::KeyCode debug_keyboard_setCameraPitch2_keyCode = Project001::KeyCode::KEY_CODE_KP_9;
    static constexpr Project001::KeyCode debug_keyboard_pitchCameraUp_keyCode = Project001::KeyCode::KEY_CODE_PAGE_DOWN;
    static constexpr Project001::KeyCode debug_keyboard_pitchCameraDown_keyCode = Project001::KeyCode::KEY_CODE_PAGE_UP;
    static constexpr Project001::KeyCode debug_keyboard_moveCameraLeft_keyCode = Project001::KeyCode::KEY_CODE_KP_4;
    static constexpr Project001::KeyCode debug_keyboard_moveCameraRight_keyCode = Project001::KeyCode::KEY_CODE_KP_6;
    static constexpr Project001::KeyCode debug_keyboard_moveCameraUp_keyCode = Project001::KeyCode::KEY_CODE_KP_8;
    static constexpr Project001::KeyCode debug_keyboard_moveCameraDown_keyCode = Project001::KeyCode::KEY_CODE_KP_2;
    static constexpr Project001::KeyCode debug_keyboard_left_keyCode = Project001::KeyCode::KEY_CODE_L;
    static constexpr Project001::KeyCode debug_keyboard_right_keyCode = Project001::KeyCode::KEY_CODE_APOSTROPHE;
    static constexpr Project001::KeyCode debug_keyboard_up_keyCode = Project001::KeyCode::KEY_CODE_P;
    static constexpr Project001::KeyCode debug_keyboard_down_keyCode = Project001::KeyCode::KEY_CODE_SEMICOLON;
    static constexpr Project001::MouseButton debug_mouse_grab_mouseButton = Project001::MouseButton::MOUSE_BUTTON_LEFT;

    bool debug_keyboard_toggleDebugCamera_pressed = false;
    bool debug_keyboard_toggleCameraLock_pressed = false;
    bool debug_keyboard_setCameraPitch1_pressed = false;
    bool debug_keyboard_setCameraPitch2_pressed = false;
    bool debug_keyboard_pitchCameraUp_pressed = false;
    bool debug_keyboard_pitchCameraDown_pressed = false;
    bool debug_keyboard_moveCameraLeft_pressed = false;
    bool debug_keyboard_moveCameraRight_pressed = false;
    bool debug_keyboard_moveCameraUp_pressed = false;
    bool debug_keyboard_moveCameraDown_pressed = false;
    bool debug_keyboard_left_pressed = false;
    bool debug_keyboard_right_pressed = false;
    bool debug_keyboard_up_pressed = false;
    bool debug_keyboard_down_pressed = false;
    bool debug_mouse_grab_pressed = false;
    bool debug_mouse_grab_released = true;

    unsigned int debug_keyboard_toggleDebugCamera_pressCount = 0;
    unsigned int debug_keyboard_toggleCameraLock_pressCount = 0;
    unsigned int debug_keyboard_setCameraPitch1_pressCount = 0;
    unsigned int debug_keyboard_setCameraPitch2_pressCount = 0;
    unsigned int debug_keyboard_pitchCameraUp_pressCount = 0;
    unsigned int debug_keyboard_pitchCameraDown_pressCount = 0;
    unsigned int debug_keyboard_moveCameraLeft_pressCount = 0;
    unsigned int debug_keyboard_moveCameraRight_pressCount = 0;
    unsigned int debug_keyboard_moveCameraUp_pressCount = 0;
    unsigned int debug_keyboard_moveCameraDown_pressCount = 0;
    unsigned int debug_keyboard_left_pressCount = 0;
    unsigned int debug_keyboard_right_pressCount = 0;
    unsigned int debug_keyboard_up_pressCount = 0;
    unsigned int debug_keyboard_down_pressCount = 0;
    unsigned int debug_mouse_grab_pressCount = 0;
    unsigned int debug_mouse_grab_releaseCount = 0;

    // Game Constants ----------------------------------------------------------

    bool configFileFound_ = false;

    bool cursorEnabled = false;
    bool invisiblePauseScreen = false;
    float groundApothem = 512.0f;
    float groundApothemShrinkRate_s = 8.0f;
    float sharkPathOffset = 256.0f;
    float killzoneApothem = 1024.0f;
    size_t coolGlassesPlayerIndex = 4;

    float mainCameraPitch = 0.25f * glm::pi<float>();
    float mainCameraInitialDistanceAway = 960.0f;
    float mainCameraMinimumPlayerSpread = 240.0f;
    float mainCameraPlayerToEdgeSpacing = 128.0f;
    float mainCameraMoveSpeed = 32.0f;
    float mainCameraZoomSpeed = 256.0f;

    // 0 = don't include shark
    // 1 = include shark always
    // 2 = include shark when penguin in water
    // 3 = include shark when chasing
    size_t mainCameraSharkInclusionMode = 0;

    float cursorSnowballCreationDelay_s = 0.5f;
    float cursorSnowballInitialRadius = 8.0f;
    float cursorSnowballGrowthRate_s = 6.0f;
    float cursorSnowballThrowSpeed_s = 256.0f;

    float penguinGlassesAlpha = 1.0f;
    float penguinDensity = 1.0f;
    float penguinHitHardThreshold = 128.0f;
    float penguinHitstunCooldownDivisor = 1024.0f;
    float penguinSnowballCreationDelay_s = 0.5f;
    float penguinSnowballInitialRadius = 8.0f;
    float penguinSnowballGrowthRate_s = 4.0f;
    float penguinSnowballThrowSpeed_s = 256.0f;
    float penguinRegrabDelay_s = 0.5f;
    float penguinMaxSpeed_s = 128.0f;
    float penguinAcceleration_s = 256.0f;
    float penguinFriction_s = 192.0f;
    float penguinMaxAngularSpeed_s = glm::pi<float>() * 8.0f;
    float penguinAngularAcceleration_s = glm::pi<float>() * 16.0f;
    float penguinAngularFriction_s = glm::pi<float>() * 32.0f;
    float penguinHitstunFriction_s = 16.0f;
    float penguinHitstunAngularFriction_s = glm::pi<float>();

    float sharkDensity = 0.25f;
    float sharkHitHardThreshold = 128.0f;
    float sharkHitstunCooldownDivisor = 1024.0f;
    float sharkMaxSpeed_s = 128.0f;
    float sharkAcceleration_s = 256.0f;
    float sharkFriction_s = 192.0f;
    float sharkMaxAngularSpeed_s = glm::pi<float>();
    float sharkAngularAcceleration_s = glm::pi<float>() * 2.0f;
    float sharkAngularFriction_s = glm::pi<float>() * 8.0f;
    float sharkChasingMaxSpeed_s = 256.0f;
    float sharkChasingAcceleration_s = 512.0f;
    float sharkHitstunFriction_s = 16.0f;
    float sharkHitstunAngularFriction_s = glm::pi<float>();

    float snowballDensity = 1.0f;
    float snowballFriction_s = 16.0f;
    float snowballAngularFriction_s = glm::pi<float>();

    static constexpr float s_quadtreeOffset = 128.0f;
    static constexpr unsigned int s_randomNumberSeed = 777;

    static constexpr float s_penguin_collisionRadius = 12.0f;
    static constexpr float s_penguin_grabOffset = 22.0f;
    static constexpr float s_penguin_grabRadius = 18.0f;

    static constexpr float s_waterHeight = -8.0f;

    static constexpr float s_debugControlEnabled = false;

    // Resources ---------------------------------------------------------------

    // Pixel Font Resources
    const Project001::FontData* pixelFont_fontDataPtr = nullptr;
    const Project001::TextureData* pixelFont_textureDataPtr = nullptr;
    unsigned int pixelFont_textureId = static_cast<unsigned int>(-1);

    // General Resources
    Project001::MeshData* loadingText_meshDataPtr = nullptr;

    Project001::MeshData* circle_meshDataPtr = nullptr;
    Project001::MeshData* orientationArrow_meshDataPtr = nullptr;
    Project001::MeshData* hallowCircle_meshDataPtr = nullptr;

    Project001::MeshData* player_grabZone_meshDataPtr = nullptr;

    static constexpr float s_maxAimLineLength = 4096.0f;
    static constexpr float s_aimLineWidth = 2.0f;
    Project001::MeshData* player1_aimRay1_meshDataPtr = nullptr;
    Project001::MeshData* player1_aimRay2_meshDataPtr = nullptr;
    Project001::MeshData* player2_aimRay1_meshDataPtr = nullptr;
    Project001::MeshData* player2_aimRay2_meshDataPtr = nullptr;
    Project001::MeshData* player3_aimRay1_meshDataPtr = nullptr;
    Project001::MeshData* player3_aimRay2_meshDataPtr = nullptr;
    Project001::MeshData* player4_aimRay1_meshDataPtr = nullptr;
    Project001::MeshData* player4_aimRay2_meshDataPtr = nullptr;

    Project001::TextureData* dotted_1_1_textureDataPtr = nullptr;
    unsigned int dotted_1_1_textureId = static_cast<unsigned int>(-1);
    Project001::TextureData* dotted_1_3_textureDataPtr = nullptr;
    unsigned int dotted_1_3_textureId = static_cast<unsigned int>(-1);

    // Main Menu Resources
    Project001::MeshData* uiMenuBackground_meshDataPtr = nullptr;
    Project001::MeshData* uiMenuAuthorText_meshDataPtr = nullptr;
    Project001::MeshData* uiMenuConfigFileFoundText_meshDataPtr = nullptr;
    Project001::MeshData* uiMenuLeftText_meshDataPtr = nullptr;
    Project001::MeshData* uiMenuRightText_meshDataPtr = nullptr;
    Project001::MeshData* uiMenuPlayerText1_meshDataPtr = nullptr;
    Project001::MeshData* uiMenuPlayerText2_meshDataPtr = nullptr;
    Project001::MeshData* uiMenuPlayerText3_meshDataPtr = nullptr;
    Project001::MeshData* uiMenuPlayerText4_meshDataPtr = nullptr;
    Project001::MeshData* uiMenuStartText_meshDataPtr = nullptr;
    Project001::MeshData* uiMenuTitleText_meshDataPtr = nullptr;
    Project001::MeshData* uiMenuVersionText_meshDataPtr = nullptr;

    // Cursor Resources
    Project001::MeshData* cursorHandOpen_meshDataPtr = nullptr;
    Project001::MeshData* cursorHandPointer_meshDataPtr = nullptr;
    Project001::MeshData* cursorHandGrab_meshDataPtr = nullptr;
    Project001::TextureData* cursor_textureDataPtr = nullptr;
    unsigned int cursor_textureId = static_cast<unsigned int>(-1);
    Project001::MeshData* cursor_marker_meshDataPtr = nullptr;
    Project001::MeshData* cursor_aimRay1_meshDataPtr = nullptr;
    Project001::MeshData* cursor_aimRay2_meshDataPtr = nullptr;
    Project001::MeshData* cursor_aimRay3_meshDataPtr = nullptr;

    // Impact Resources
    Project001::MeshData* impactFrame_meshDataPtrs[4][4] = {};

    // Stage Resources
    Project001::MeshData* ground_meshDataPtr = nullptr;
    Project001::MeshData* water_meshDataPtr = nullptr;

    Project001::MeshData* deadZone_meshDataPtr = nullptr;
    Project001::TextureData* hazard_textureDataPtr = nullptr;
    unsigned int hazard_textureId = static_cast<unsigned int>(-1);

    // Stage Grid Resources
    Project001::MeshData* stageGrid_meshDataPtr = nullptr;
    unsigned int stageGrid_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* stageGridLabels_meshDataPtr = nullptr;
    unsigned int stageGridLabels_meshId = static_cast<unsigned int>(-1);

    Project001::MeshData* stageCollisionQuadTree_meshDataPtr = nullptr;

    // Actor Resources
    Project001::MeshData* penguin_body_meshDataPtr = nullptr;
    unsigned int penguin_body_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin_flipper_right_meshDataPtr = nullptr;
    unsigned int penguin_flipper_right_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin_flipper_left_meshDataPtr = nullptr;
    unsigned int penguin_flipper_left_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin_foot_right_meshDataPtr = nullptr;
    unsigned int penguin_foot_right_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin_foot_left_meshDataPtr = nullptr;
    unsigned int penguin_foot_left_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin_head_meshDataPtr = nullptr;
    unsigned int penguin_head_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin_eyes_meshDataPtr = nullptr;
    unsigned int penguin_eyes_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin_eyes_v2_meshDataPtr = nullptr;
    unsigned int penguin_eyes_v2_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin_beak_meshDataPtr = nullptr;
    unsigned int penguin_beak_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin_glasses_meshDataPtr = nullptr;
    unsigned int penguin_glasses_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin_glasses_v2_meshDataPtr = nullptr;
    unsigned int penguin_glasses_v2_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin_glasses_v3_meshDataPtr = nullptr;
    unsigned int penguin_glasses_v3_meshId = static_cast<unsigned int>(-1);
    Project001::TextureData* penguin_textureDataPtr = nullptr;
    unsigned int penguin_textureId = static_cast<unsigned int>(-1);
    Project001::TextureData* penguin_v2_textureDataPtr = nullptr;
    unsigned int penguin_v2_textureId = static_cast<unsigned int>(-1);
    Project001::TextureData* penguin_v3_textureDataPtr = nullptr;
    unsigned int penguin_v3_textureId = static_cast<unsigned int>(-1);
    Project001::TextureData* penguin_v4_textureDataPtr = nullptr;
    unsigned int penguin_v4_textureId = static_cast<unsigned int>(-1);

    Project001::MeshData* snowball_meshDataPtr = nullptr;
    unsigned int snowball_meshId = static_cast<unsigned int>(-1);

    Project001::MeshData* shark_back_meshDataPtr = nullptr;
    unsigned int shark_back_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* shark_front_meshDataPtr = nullptr;
    unsigned int shark_front_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* shark_jaw_meshDataPtr = nullptr;
    unsigned int shark_jaw_meshId = static_cast<unsigned int>(-1);
    Project001::TextureData* shark_textureDataPtr = nullptr;
    unsigned int shark_textureId = static_cast<unsigned int>(-1);

    Project001::MeshData* shark_back_collision_meshDataPtr = nullptr;
    Project001::MeshData* shark_front_collision_meshDataPtr = nullptr;
    Project001::MeshData* shark_jaw_collision_meshDataPtr = nullptr;
    Project001::MeshData* shark_attackRay1_meshDataPtr = nullptr;
    Project001::MeshData* shark_attackRay2_meshDataPtr = nullptr;
    Project001::MeshData* shark_attackRay3_meshDataPtr = nullptr;
    Project001::MeshData* shark_attackRay4_meshDataPtr = nullptr;

    // Ui Resources
    Project001::MeshData* uiPauseBackground_meshDataPtr = nullptr;
    Project001::MeshData* uiPauseTitleText_meshDataPtr = nullptr;
    Project001::MeshData* uiPauseText01_meshDataPtr = nullptr;
    Project001::MeshData* uiPauseText02_meshDataPtr = nullptr;
    Project001::MeshData* uiGameOverTitleText_meshDataPtr = nullptr;
};