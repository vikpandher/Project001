// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-02-18

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

struct PlayerInfo
{
    size_t playerNumber = 0;
    bool turnedOn = true;

    enum class ControlScheme
    {
        CONTROL_SCHEME_UNKNOWN,
        CONTROL_SCHEME_KEYBOARD_1,
        CONTROL_SCHEME_KEYBOARD_2,
        CONTROL_SCHEME_CONTROLLER_1,
        CONTROL_SCHEME_CONTROLLER_2,
        CONTROL_SCHEME_CONTROLLER_3,
        CONTROL_SCHEME_CONTROLLER_4
    };

    static PlayerInfo::ControlScheme PlayerInfo::StringToControlScheme(const std::string& str);

    ControlScheme controlScheme = ControlScheme::CONTROL_SCHEME_KEYBOARD_1;

    float axisDeadzone = 0.2f;

    unsigned int start_pressCount = 0;
    unsigned int pause_pressCount = 0;
    unsigned int quit_pressCount = 0;
    unsigned int left_pressCount = 0;
    unsigned int right_pressCount = 0;
    unsigned int up_pressCount = 0;
    unsigned int down_pressCount = 0;
    unsigned int snowball_pressCount = 0;
    float leftRightAxisValue = 0.0f;
    float upDownAxisValue = 0.0f;
};

struct SharedApplicationData
{
    // Scene Ids ---------------------------------------------------------------

    unsigned int scene001Id = static_cast<unsigned int>(-1);
    unsigned int scene002Id = static_cast<unsigned int>(-1);

    // Game Info ---------------------------------------------------------------

    unsigned int score = 0;
    float gameTime_s = 0.0f;

    static const size_t s_player_count = 4;
    PlayerInfo playerInfos[s_player_count] = {
        {0, true, PlayerInfo::ControlScheme::CONTROL_SCHEME_KEYBOARD_1},
        {1, true, PlayerInfo::ControlScheme::CONTROL_SCHEME_KEYBOARD_2},
        {2, true, PlayerInfo::ControlScheme::CONTROL_SCHEME_CONTROLLER_1},
        {3, true, PlayerInfo::ControlScheme::CONTROL_SCHEME_CONTROLLER_2}
    };

    void UpdateKeyboardButtonPresses(const Project001::KeyEvent& keyEvent);
    void UpdateButtonPressCounts(const Project001::Window* windowPtr);

    // Player Controls ---------------------------------------------------------

    Project001::KeyCode keyboard_1_start_keyCode = Project001::KeyCode::KEY_CODE_SPACE;
    Project001::KeyCode keyboard_1_pause_keyCode = Project001::KeyCode::KEY_CODE_ENTER;
    Project001::KeyCode keyboard_1_quit_keyCode = Project001::KeyCode::KEY_CODE_ESCAPE;
    Project001::KeyCode keyboard_1_left_keyCode = Project001::KeyCode::KEY_CODE_A;
    Project001::KeyCode keyboard_1_right_keyCode = Project001::KeyCode::KEY_CODE_D;
    Project001::KeyCode keyboard_1_up_keyCode = Project001::KeyCode::KEY_CODE_W;
    Project001::KeyCode keyboard_1_down_keyCode = Project001::KeyCode::KEY_CODE_S;
    Project001::KeyCode keyboard_1_snowball_keyCode = Project001::KeyCode::KEY_CODE_SPACE;

    bool keyboard_1_start_pressed = false;
    bool keyboard_1_pause_pressed = false;
    bool keyboard_1_quit_pressed = false;
    bool keyboard_1_left_pressed = false;
    bool keyboard_1_right_pressed = false;
    bool keyboard_1_up_pressed = false;
    bool keyboard_1_down_pressed = false;
    bool keyboard_1_snowball_pressed = false;

    Project001::KeyCode keyboard_2_start_keyCode = Project001::KeyCode::KEY_CODE_KP_0;
    Project001::KeyCode keyboard_2_pause_keyCode = Project001::KeyCode::KEY_CODE_KP_ENTER;
    Project001::KeyCode keyboard_2_quit_keyCode = Project001::KeyCode::KEY_CODE_KP_SUBTRACT;
    Project001::KeyCode keyboard_2_left_keyCode = Project001::KeyCode::KEY_CODE_LEFT;
    Project001::KeyCode keyboard_2_right_keyCode = Project001::KeyCode::KEY_CODE_RIGHT;
    Project001::KeyCode keyboard_2_up_keyCode = Project001::KeyCode::KEY_CODE_UP;
    Project001::KeyCode keyboard_2_down_keyCode = Project001::KeyCode::KEY_CODE_DOWN;
    Project001::KeyCode keyboard_2_snowball_keyCode = Project001::KeyCode::KEY_CODE_KP_0;

    bool keyboard_2_start_pressed = false;
    bool keyboard_2_pause_pressed = false;
    bool keyboard_2_quit_pressed = false;
    bool keyboard_2_left_pressed = false;
    bool keyboard_2_right_pressed = false;
    bool keyboard_2_up_pressed = false;
    bool keyboard_2_down_pressed = false;
    bool keyboard_2_snowball_pressed = false;

    unsigned int controller_1_start_buttonIndex = 0;
    unsigned int controller_1_pause_buttonIndex = 7;
    unsigned int controller_1_quit_buttonIndex = 1;
    unsigned int controller_1_left_buttonIndex = 13;
    unsigned int controller_1_right_buttonIndex = 11;
    unsigned int controller_1_up_buttonIndex = 10;
    unsigned int controller_1_down_buttonIndex = 12;
    unsigned int controller_1_snowball_buttonIndex = 0;
    unsigned int controller_1_moveRightLeft_axisIndex = 0;
    unsigned int controller_1_moveDownUp_axisIndex = 1;
    float controller_1_axisDeadzone = 0.2f;

    unsigned int controller_2_start_buttonIndex = 0;
    unsigned int controller_2_pause_buttonIndex = 7;
    unsigned int controller_2_quit_buttonIndex = 1;
    unsigned int controller_2_left_buttonIndex = 13;
    unsigned int controller_2_right_buttonIndex = 11;
    unsigned int controller_2_up_buttonIndex = 10;
    unsigned int controller_2_down_buttonIndex = 12;
    unsigned int controller_2_snowball_buttonIndex = 0;
    unsigned int controller_2_moveRightLeft_axisIndex = 0;
    unsigned int controller_2_moveDownUp_axisIndex = 1;
    float controller_2_axisDeadzone = 0.2f;

    unsigned int controller_3_start_buttonIndex = 0;
    unsigned int controller_3_pause_buttonIndex = 7;
    unsigned int controller_3_quit_buttonIndex = 1;
    unsigned int controller_3_left_buttonIndex = 13;
    unsigned int controller_3_right_buttonIndex = 11;
    unsigned int controller_3_up_buttonIndex = 10;
    unsigned int controller_3_down_buttonIndex = 12;
    unsigned int controller_3_snowball_buttonIndex = 0;
    unsigned int controller_3_moveRightLeft_axisIndex = 0;
    unsigned int controller_3_moveDownUp_axisIndex = 1;
    float controller_3_axisDeadzone = 0.2f;

    unsigned int controller_4_start_buttonIndex = 0;
    unsigned int controller_4_pause_buttonIndex = 7;
    unsigned int controller_4_quit_buttonIndex = 1;
    unsigned int controller_4_left_buttonIndex = 13;
    unsigned int controller_4_right_buttonIndex = 11;
    unsigned int controller_4_up_buttonIndex = 10;
    unsigned int controller_4_down_buttonIndex = 12;
    unsigned int controller_4_snowball_buttonIndex = 0;
    unsigned int controller_4_moveRightLeft_axisIndex = 0;
    unsigned int controller_4_moveDownUp_axisIndex = 1;
    float controller_4_axisDeadzone = 0.2f;

    static const Project001::KeyCode debug_keyboard_toggleDebugCamera_keyCode = Project001::KeyCode::KEY_CODE_0;
    static const Project001::KeyCode debug_keyboard_toggleCameraLock_keyCode = Project001::KeyCode::KEY_CODE_KP_5;
    static const Project001::KeyCode debug_keyboard_setCameraPitch1_keyCode = Project001::KeyCode::KEY_CODE_KP_7;
    static const Project001::KeyCode debug_keyboard_setCameraPitch2_keyCode = Project001::KeyCode::KEY_CODE_KP_9;
    static const Project001::KeyCode debug_keyboard_pitchCameraUp_keyCode = Project001::KeyCode::KEY_CODE_PAGE_DOWN;
    static const Project001::KeyCode debug_keyboard_pitchCameraDown_keyCode = Project001::KeyCode::KEY_CODE_PAGE_UP;
    static const Project001::KeyCode debug_keyboard_moveCameraLeft_keyCode = Project001::KeyCode::KEY_CODE_KP_4;
    static const Project001::KeyCode debug_keyboard_moveCameraRight_keyCode = Project001::KeyCode::KEY_CODE_KP_6;
    static const Project001::KeyCode debug_keyboard_moveCameraUp_keyCode = Project001::KeyCode::KEY_CODE_KP_8;
    static const Project001::KeyCode debug_keyboard_moveCameraDown_keyCode = Project001::KeyCode::KEY_CODE_KP_2;

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

    // Game Constants ----------------------------------------------------------

    unsigned int randomNumberSeed = 777;

    const float ground_size = 512.0f;

    const float deadzone_size = 1024.0f;

    const float maxStage_size = 1088.0f;

    const float penguin_collisionRadius = 12.0f;
    const float penguin_grabOffset = 22.0f;
    const float penguin_grabRadius = 18.0f;
    const float penguin_throwSpeed_s = 256.0f;

    const float snowball_landFriction = 8.0f;
    const float snowball_waterFriction = 16.0f;
    const float snowball_angularFriction = glm::pi<float>();

    // Resources ---------------------------------------------------------------

    // Pixel Font Resources
    const Project001::FontData* pixelFont_fontDataPtr = nullptr;
    const Project001::TextureData* pixelFont_textureDataPtr = nullptr;
    unsigned int pixelFont_textureId = static_cast<unsigned int>(-1);

    // General Resources
    Project001::MeshData* circle_meshDataPtr = nullptr;
    Project001::MeshData* orientationArrow_meshDataPtr = nullptr;
    Project001::MeshData* hallowCircle_meshDataPtr = nullptr;

    // Main Menu Resources
    Project001::MeshData* authorText_meshDataPtr = nullptr;
    Project001::MeshData* introText_meshDataPtr = nullptr;
    Project001::MeshData* startText_meshDataPtr = nullptr;
    Project001::MeshData* titleText_meshDataPtr = nullptr;

    // Stage Resources
    Project001::MeshData* ground_meshDataPtr = nullptr;
    Project001::MeshData* groundCollision_meshDataPtr = nullptr;

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

    // Ui Resources
    Project001::MeshData* uiLeftBackground_meshDataPtr = nullptr;
    Project001::MeshData* uiLeftText01_meshDataPtr = nullptr;

    Project001::MeshData* uiMiddleBackground_meshDataPtr = nullptr;
    Project001::MeshData* uiMiddleText01_meshDataPtr = nullptr;

    Project001::MeshData* uiRightBackground_meshDataPtr = nullptr;
    Project001::MeshData* uiRightText01_meshDataPtr = nullptr;

    Project001::MeshData* uiPauseBackground_meshDataPtr = nullptr;
    Project001::MeshData* uiPauseText01_meshDataPtr = nullptr;
    Project001::MeshData* uiPauseText02_meshDataPtr = nullptr;
    Project001::MeshData* uiPauseText03_meshDataPtr = nullptr;
};