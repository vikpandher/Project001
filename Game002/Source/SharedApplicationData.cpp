// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-06-29

#include "SharedApplicationData.h"

#include "Window.h"



PlayerInfo::ControlScheme PlayerInfo::StringToControlScheme(const std::string& str)
{
    const std::string prefix = "CONTROL_SCHEME_";
    if (str.length() < prefix.length() ||
        str.find(prefix, 0) != 0)
    {
        return ControlScheme::CONTROL_SCHEME_UNKNOWN;
    }

    std::string value = str.substr(prefix.length());

    if (value == "KEYBOARD_1")
    {
        return ControlScheme::CONTROL_SCHEME_KEYBOARD_1;
    }
    else if (value == "KEYBOARD_2")
    {
        return ControlScheme::CONTROL_SCHEME_KEYBOARD_2;
    }
    else if (value == "CONTROLLER_1")
    {
        return ControlScheme::CONTROL_SCHEME_CONTROLLER_1;
    }
    else if (value == "CONTROLLER_2")
    {
        return ControlScheme::CONTROL_SCHEME_CONTROLLER_2;
    }
    else if (value == "CONTROLLER_3")
    {
        return ControlScheme::CONTROL_SCHEME_CONTROLLER_3;
    }
    else if (value == "CONTROLLER_4")
    {
        return ControlScheme::CONTROL_SCHEME_CONTROLLER_4;
    }

    return ControlScheme::CONTROL_SCHEME_UNKNOWN;
}

void SharedApplicationData::UpdateKeyboardButtonPresses(const Project001::KeyEvent& keyEvent)
{
    const Project001::KeyCode& keyCode = keyEvent.keyCode;
    const Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    const Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    for (size_t i = 0; i < s_player_count; ++i)
    {
        PlayerInfo& playerInfo = playerInfos[i];

        if (playerInfo.controlScheme == PlayerInfo::ControlScheme::CONTROL_SCHEME_KEYBOARD_1)
        {
            if (keyCode == keyboard_1_start_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_1_start_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_1_start_pressed = false;
            }
            if (keyCode == keyboard_1_pause_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_1_pause_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_1_pause_pressed = false;
            }
            if (keyCode == keyboard_1_quit_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_1_quit_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_1_quit_pressed = false;
            }
            if (keyCode == keyboard_1_left_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_1_left_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_1_left_pressed = false;
            }
            if (keyCode == keyboard_1_right_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_1_right_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_1_right_pressed = false;
            }
            if (keyCode == keyboard_1_up_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_1_up_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_1_up_pressed = false;
            }
            if (keyCode == keyboard_1_down_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_1_down_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_1_down_pressed = false;
            }
            if (keyCode == keyboard_1_grab_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_1_grab_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_1_grab_pressed = false;
            }
            if (keyCode == keyboard_1_throw_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_1_throw_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_1_throw_pressed = false;
            }
        }
        else if (playerInfo.controlScheme == PlayerInfo::ControlScheme::CONTROL_SCHEME_KEYBOARD_2)
        {
            if (keyCode == keyboard_2_start_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_2_start_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_2_start_pressed = false;
            }
            if (keyCode == keyboard_2_pause_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_2_pause_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_2_pause_pressed = false;
            }
            if (keyCode == keyboard_2_quit_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_2_quit_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_2_quit_pressed = false;
            }
            if (keyCode == keyboard_2_left_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_2_left_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_2_left_pressed = false;
            }
            if (keyCode == keyboard_2_right_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_2_right_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_2_right_pressed = false;
            }
            if (keyCode == keyboard_2_up_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_2_up_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_2_up_pressed = false;
            }
            if (keyCode == keyboard_2_down_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_2_down_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_2_down_pressed = false;
            }
            if (keyCode == keyboard_2_grab_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_2_grab_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_2_grab_pressed = false;
            }
            if (keyCode == keyboard_2_throw_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_2_throw_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_2_throw_pressed = false;
            }
        }
    }

    if (keyCode == debug_keyboard_toggleDebugCamera_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_toggleDebugCamera_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_toggleDebugCamera_pressed = false;
    }
    if (keyCode == debug_keyboard_toggleCameraLock_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_toggleCameraLock_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_toggleCameraLock_pressed = false;
    }
    if (keyCode == debug_keyboard_pitchCameraUp_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_pitchCameraUp_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_pitchCameraUp_pressed = false;
    }
    if (keyCode == debug_keyboard_setCameraPitch1_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_setCameraPitch1_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_setCameraPitch1_pressed = false;
    }
    if (keyCode == debug_keyboard_setCameraPitch2_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_setCameraPitch2_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_setCameraPitch2_pressed = false;
    }
    if (keyCode == debug_keyboard_pitchCameraDown_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_pitchCameraDown_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_pitchCameraDown_pressed = false;
    }
    if (keyCode == debug_keyboard_moveCameraLeft_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_moveCameraLeft_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_moveCameraLeft_pressed = false;
    }
    if (keyCode == debug_keyboard_moveCameraRight_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_moveCameraRight_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_moveCameraRight_pressed = false;
    }
    if (keyCode == debug_keyboard_moveCameraUp_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_moveCameraUp_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_moveCameraUp_pressed = false;
    }
    if (keyCode == debug_keyboard_moveCameraDown_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_moveCameraDown_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_moveCameraDown_pressed = false;
    }
}

void SharedApplicationData::UpdateMouseButtonPresses(const Project001::MouseButtonEvent& mouseButtonEvent)
{
    const Project001::MouseButton& mouseButton = mouseButtonEvent.mouseButton;
    const Project001::ButtonAction& buttonAction = mouseButtonEvent.buttonAction;
    const Project001::KeyModifier& keyModifier = mouseButtonEvent.keyModifier;

    if (mouseButton == debug_mouse_grab_mouseButton)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS)
        {
            debug_mouse_grab_pressed = true;
            debug_mouse_grab_released = false;
        }
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
        {
            debug_mouse_grab_pressed = false;
            debug_mouse_grab_released = true;
        }
    }
}

void SharedApplicationData::UpdateButtonPressCounts(const Project001::Window* windowPtr)
{
    for (size_t i = 0; i < s_player_count; ++i)
    {
        PlayerInfo& playerInfo = playerInfos[i];

        if (playerInfo.controlScheme == PlayerInfo::ControlScheme::CONTROL_SCHEME_KEYBOARD_1)
        {
            if (keyboard_1_start_pressed) playerInfo.start_pressCount++;
            else playerInfo.start_pressCount = 0;

            if (keyboard_1_pause_pressed) playerInfo.pause_pressCount++;
            else playerInfo.pause_pressCount = 0;

            if (keyboard_1_quit_pressed) playerInfo.quit_pressCount++;
            else playerInfo.quit_pressCount = 0;

            if (keyboard_1_left_pressed) playerInfo.left_pressCount++;
            else playerInfo.left_pressCount = 0;

            if (keyboard_1_right_pressed) playerInfo.right_pressCount++;
            else playerInfo.right_pressCount = 0;

            if (keyboard_1_up_pressed) playerInfo.up_pressCount++;
            else playerInfo.up_pressCount = 0;

            if (keyboard_1_down_pressed) playerInfo.down_pressCount++;
            else playerInfo.down_pressCount = 0;

            if (keyboard_1_grab_pressed) playerInfo.grab_pressCount++;
            else playerInfo.grab_pressCount = 0;

            if (keyboard_1_throw_pressed) playerInfo.throw_pressCount++;
            else playerInfo.throw_pressCount = 0;
        }
        else if (playerInfo.controlScheme == PlayerInfo::ControlScheme::CONTROL_SCHEME_KEYBOARD_2)
        {
            if (keyboard_2_start_pressed) playerInfo.start_pressCount++;
            else playerInfo.start_pressCount = 0;

            if (keyboard_2_pause_pressed) playerInfo.pause_pressCount++;
            else playerInfo.pause_pressCount = 0;

            if (keyboard_2_quit_pressed) playerInfo.quit_pressCount++;
            else playerInfo.quit_pressCount = 0;

            if (keyboard_2_left_pressed) playerInfo.left_pressCount++;
            else playerInfo.left_pressCount = 0;

            if (keyboard_2_right_pressed) playerInfo.right_pressCount++;
            else playerInfo.right_pressCount = 0;

            if (keyboard_2_up_pressed) playerInfo.up_pressCount++;
            else playerInfo.up_pressCount = 0;

            if (keyboard_2_down_pressed) playerInfo.down_pressCount++;
            else playerInfo.down_pressCount = 0;

            if (keyboard_2_grab_pressed) playerInfo.grab_pressCount++;
            else playerInfo.grab_pressCount = 0;

            if (keyboard_2_throw_pressed) playerInfo.throw_pressCount++;
            else playerInfo.throw_pressCount = 0;
        }
        else if (playerInfo.controlScheme == PlayerInfo::ControlScheme::CONTROL_SCHEME_CONTROLLER_1)
        {
            std::vector<bool> buttonValues;
            windowPtr->GetJoystickButtons(0, buttonValues);

            if (buttonValues.size() > controller_1_start_buttonIndex && buttonValues[controller_1_start_buttonIndex]) playerInfo.start_pressCount++;
            else  playerInfo.start_pressCount = 0;

            if (buttonValues.size() > controller_1_pause_buttonIndex && buttonValues[controller_1_pause_buttonIndex]) playerInfo.pause_pressCount++;
            else playerInfo.pause_pressCount = 0;

            if (buttonValues.size() > controller_1_quit_buttonIndex && buttonValues[controller_1_quit_buttonIndex]) playerInfo.quit_pressCount++;
            else playerInfo.quit_pressCount = 0;

            if (buttonValues.size() > controller_1_left_buttonIndex && buttonValues[controller_1_left_buttonIndex]) playerInfo.left_pressCount++;
            else playerInfo.left_pressCount = 0;

            if (buttonValues.size() > controller_1_right_buttonIndex && buttonValues[controller_1_right_buttonIndex]) playerInfo.right_pressCount++;
            else playerInfo.right_pressCount = 0;

            if (buttonValues.size() > controller_1_up_buttonIndex && buttonValues[controller_1_up_buttonIndex]) playerInfo.up_pressCount++;
            else playerInfo.up_pressCount = 0;

            if (buttonValues.size() > controller_1_down_buttonIndex && buttonValues[controller_1_down_buttonIndex]) playerInfo.down_pressCount++;
            else playerInfo.down_pressCount = 0;

            if (buttonValues.size() > controller_1_grab_buttonIndex && buttonValues[controller_1_grab_buttonIndex]) playerInfo.grab_pressCount++;
            else playerInfo.grab_pressCount = 0;

            if (buttonValues.size() > controller_1_throw_buttonIndex && buttonValues[controller_1_throw_buttonIndex]) playerInfo.throw_pressCount++;
            else playerInfo.throw_pressCount = 0;

            std::vector<float> axisValues;
            windowPtr->GetJoystickAxis(0, axisValues);

            if (axisValues.size() > controller_1_moveRightLeft_axisIndex) playerInfo.leftRightAxisValue = axisValues[controller_1_moveRightLeft_axisIndex];

            if (axisValues.size() > controller_1_moveDownUp_axisIndex) playerInfo.upDownAxisValue = -axisValues[controller_1_moveDownUp_axisIndex];
        }
        else if (playerInfo.controlScheme == PlayerInfo::ControlScheme::CONTROL_SCHEME_CONTROLLER_2)
        {
            std::vector<bool> buttonValues;
            windowPtr->GetJoystickButtons(1, buttonValues);

            if (buttonValues.size() > controller_2_start_buttonIndex && buttonValues[controller_2_start_buttonIndex]) playerInfo.start_pressCount++;
            else  playerInfo.start_pressCount = 0;

            if (buttonValues.size() > controller_2_pause_buttonIndex && buttonValues[controller_2_pause_buttonIndex]) playerInfo.pause_pressCount++;
            else playerInfo.pause_pressCount = 0;

            if (buttonValues.size() > controller_2_quit_buttonIndex && buttonValues[controller_2_quit_buttonIndex]) playerInfo.quit_pressCount++;
            else playerInfo.quit_pressCount = 0;

            if (buttonValues.size() > controller_2_left_buttonIndex && buttonValues[controller_2_left_buttonIndex]) playerInfo.left_pressCount++;
            else playerInfo.left_pressCount = 0;

            if (buttonValues.size() > controller_2_right_buttonIndex && buttonValues[controller_2_right_buttonIndex]) playerInfo.right_pressCount++;
            else playerInfo.right_pressCount = 0;

            if (buttonValues.size() > controller_2_up_buttonIndex && buttonValues[controller_2_up_buttonIndex]) playerInfo.up_pressCount++;
            else playerInfo.up_pressCount = 0;

            if (buttonValues.size() > controller_2_down_buttonIndex && buttonValues[controller_2_down_buttonIndex]) playerInfo.down_pressCount++;
            else playerInfo.down_pressCount = 0;

            if (buttonValues.size() > controller_2_grab_buttonIndex && buttonValues[controller_2_grab_buttonIndex]) playerInfo.grab_pressCount++;
            else playerInfo.grab_pressCount = 0;

            if (buttonValues.size() > controller_2_throw_buttonIndex && buttonValues[controller_2_throw_buttonIndex]) playerInfo.throw_pressCount++;
            else playerInfo.throw_pressCount = 0;

            std::vector<float> axisValues;
            windowPtr->GetJoystickAxis(1, axisValues);

            if (axisValues.size() > controller_2_moveRightLeft_axisIndex) playerInfo.leftRightAxisValue = axisValues[controller_2_moveRightLeft_axisIndex];

            if (axisValues.size() > controller_2_moveDownUp_axisIndex) playerInfo.upDownAxisValue = -axisValues[controller_2_moveDownUp_axisIndex];
        }
        else if (playerInfo.controlScheme == PlayerInfo::ControlScheme::CONTROL_SCHEME_CONTROLLER_3)
        {
            std::vector<bool> buttonValues;
            windowPtr->GetJoystickButtons(2, buttonValues);

            if (buttonValues.size() > controller_3_start_buttonIndex && buttonValues[controller_3_start_buttonIndex]) playerInfo.start_pressCount++;
            else  playerInfo.start_pressCount = 0;

            if (buttonValues.size() > controller_3_pause_buttonIndex && buttonValues[controller_3_pause_buttonIndex]) playerInfo.pause_pressCount++;
            else playerInfo.pause_pressCount = 0;

            if (buttonValues.size() > controller_3_quit_buttonIndex && buttonValues[controller_3_quit_buttonIndex]) playerInfo.quit_pressCount++;
            else playerInfo.quit_pressCount = 0;

            if (buttonValues.size() > controller_3_left_buttonIndex && buttonValues[controller_3_left_buttonIndex]) playerInfo.left_pressCount++;
            else playerInfo.left_pressCount = 0;

            if (buttonValues.size() > controller_3_right_buttonIndex && buttonValues[controller_3_right_buttonIndex]) playerInfo.right_pressCount++;
            else playerInfo.right_pressCount = 0;

            if (buttonValues.size() > controller_3_up_buttonIndex && buttonValues[controller_3_up_buttonIndex]) playerInfo.up_pressCount++;
            else playerInfo.up_pressCount = 0;

            if (buttonValues.size() > controller_3_down_buttonIndex && buttonValues[controller_3_down_buttonIndex]) playerInfo.down_pressCount++;
            else playerInfo.down_pressCount = 0;

            if (buttonValues.size() > controller_3_grab_buttonIndex && buttonValues[controller_3_grab_buttonIndex]) playerInfo.grab_pressCount++;
            else playerInfo.grab_pressCount = 0;

            if (buttonValues.size() > controller_3_throw_buttonIndex && buttonValues[controller_3_throw_buttonIndex]) playerInfo.throw_pressCount++;
            else playerInfo.throw_pressCount = 0;

            std::vector<float> axisValues;
            windowPtr->GetJoystickAxis(2, axisValues);

            if (axisValues.size() > controller_3_moveRightLeft_axisIndex) playerInfo.leftRightAxisValue = axisValues[controller_3_moveRightLeft_axisIndex];

            if (axisValues.size() > controller_3_moveDownUp_axisIndex) playerInfo.upDownAxisValue = -axisValues[controller_3_moveDownUp_axisIndex];
        }
        else if (playerInfo.controlScheme == PlayerInfo::ControlScheme::CONTROL_SCHEME_CONTROLLER_4)
        {
            std::vector<bool> buttonValues;
            windowPtr->GetJoystickButtons(3, buttonValues);

            if (buttonValues.size() > controller_4_start_buttonIndex && buttonValues[controller_4_start_buttonIndex]) playerInfo.start_pressCount++;
            else  playerInfo.start_pressCount = 0;

            if (buttonValues.size() > controller_4_pause_buttonIndex && buttonValues[controller_4_pause_buttonIndex]) playerInfo.pause_pressCount++;
            else playerInfo.pause_pressCount = 0;

            if (buttonValues.size() > controller_4_quit_buttonIndex && buttonValues[controller_4_quit_buttonIndex]) playerInfo.quit_pressCount++;
            else playerInfo.quit_pressCount = 0;

            if (buttonValues.size() > controller_4_left_buttonIndex && buttonValues[controller_4_left_buttonIndex]) playerInfo.left_pressCount++;
            else playerInfo.left_pressCount = 0;

            if (buttonValues.size() > controller_4_right_buttonIndex && buttonValues[controller_4_right_buttonIndex]) playerInfo.right_pressCount++;
            else playerInfo.right_pressCount = 0;

            if (buttonValues.size() > controller_4_up_buttonIndex && buttonValues[controller_4_up_buttonIndex]) playerInfo.up_pressCount++;
            else playerInfo.up_pressCount = 0;

            if (buttonValues.size() > controller_4_down_buttonIndex && buttonValues[controller_4_down_buttonIndex]) playerInfo.down_pressCount++;
            else playerInfo.down_pressCount = 0;

            if (buttonValues.size() > controller_4_grab_buttonIndex && buttonValues[controller_4_grab_buttonIndex]) playerInfo.grab_pressCount++;
            else playerInfo.grab_pressCount = 0;

            if (buttonValues.size() > controller_4_throw_buttonIndex && buttonValues[controller_4_throw_buttonIndex]) playerInfo.throw_pressCount++;
            else playerInfo.throw_pressCount = 0;

            std::vector<float> axisValues;
            windowPtr->GetJoystickAxis(3, axisValues);

            if (axisValues.size() > controller_4_moveRightLeft_axisIndex) playerInfo.leftRightAxisValue = axisValues[controller_4_moveRightLeft_axisIndex];

            if (axisValues.size() > controller_4_moveDownUp_axisIndex) playerInfo.upDownAxisValue = -axisValues[controller_4_moveDownUp_axisIndex];
        }
    }

    if (debug_keyboard_toggleDebugCamera_pressed) debug_keyboard_toggleDebugCamera_pressCount++;
    else debug_keyboard_toggleDebugCamera_pressCount = 0;

    if (debug_keyboard_toggleCameraLock_pressed) debug_keyboard_toggleCameraLock_pressCount++;
    else debug_keyboard_toggleCameraLock_pressCount = 0;

    if (debug_keyboard_setCameraPitch1_pressed) debug_keyboard_setCameraPitch1_pressCount++;
    else debug_keyboard_setCameraPitch1_pressCount = 0;

    if (debug_keyboard_setCameraPitch2_pressed) debug_keyboard_setCameraPitch2_pressCount++;
    else debug_keyboard_setCameraPitch2_pressCount = 0;

    if (debug_keyboard_pitchCameraUp_pressed) debug_keyboard_pitchCameraUp_pressCount++;
    else debug_keyboard_pitchCameraUp_pressCount = 0;

    if (debug_keyboard_pitchCameraDown_pressed) debug_keyboard_pitchCameraDown_pressCount++;
    else debug_keyboard_pitchCameraDown_pressCount = 0;

    if (debug_keyboard_moveCameraLeft_pressed) debug_keyboard_moveCameraLeft_pressCount++;
    else debug_keyboard_moveCameraLeft_pressCount = 0;

    if (debug_keyboard_moveCameraRight_pressed) debug_keyboard_moveCameraRight_pressCount++;
    else debug_keyboard_moveCameraRight_pressCount = 0;

    if (debug_keyboard_moveCameraUp_pressed) debug_keyboard_moveCameraUp_pressCount++;
    else debug_keyboard_moveCameraUp_pressCount = 0;

    if (debug_keyboard_moveCameraDown_pressed) debug_keyboard_moveCameraDown_pressCount++;
    else debug_keyboard_moveCameraDown_pressCount = 0;

    if (debug_mouse_grab_pressed) debug_mouse_grab_pressCount++;
    else debug_mouse_grab_pressCount = 0;

    if (debug_mouse_grab_released) debug_mouse_grab_releaseCount++;
    else debug_mouse_grab_releaseCount = 0;
}