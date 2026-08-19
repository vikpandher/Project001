// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-08-18

#include "SharedApplicationData.h"

#include "Window.h"



const char* ControlSchemeInfo::ControlSchemeIndexToString(size_t controlSchemeIndex)
{
    switch (controlSchemeIndex)
    {
        case ControlSchemeInfo::s_controlSchemeIndex_keyboard1:
        {
            return "KEYBOARD_1";
            break;
        }
        case ControlSchemeInfo::s_controlSchemeIndex_keyboard2:
        {
            return "KEYBOARD_2";
            break;
        }
        case ControlSchemeInfo::s_controlSchemeIndex_controller1:
        {
            return "CONTROLLER_1";
            break;
        }
        case ControlSchemeInfo::s_controlSchemeIndex_controller2:
        {
            return "CONTROLLER_2";
            break;
        }
        case ControlSchemeInfo::s_controlSchemeIndex_controller3:
        {
            return "CONTROLLER_3";
            break;
        }
        case ControlSchemeInfo::s_controlSchemeIndex_controller4:
        {
            return "CONTROLLER_4";
            break;
        }
    }
    return "OFF";
}

size_t ControlSchemeInfo::StringToControlSchemeIndex(const std::string& str)
{
    const std::string prefix = "CONTROL_SCHEME_";
    if (str.length() < prefix.length() ||
        str.find(prefix, 0) != 0)
    {
        return ControlSchemeInfo::s_controlSchemeIndex_unknown;
    }

    std::string value = str.substr(prefix.length());

    if (value == "KEYBOARD_1")
    {
        return ControlSchemeInfo::s_controlSchemeIndex_keyboard1;
    }
    else if (value == "KEYBOARD_2")
    {
        return ControlSchemeInfo::s_controlSchemeIndex_keyboard2;
    }
    else if (value == "CONTROLLER_1")
    {
        return ControlSchemeInfo::s_controlSchemeIndex_controller1;
    }
    else if (value == "CONTROLLER_2")
    {
        return ControlSchemeInfo::s_controlSchemeIndex_controller2;
    }
    else if (value == "CONTROLLER_3")
    {
        return ControlSchemeInfo::s_controlSchemeIndex_controller3;
    }
    else if (value == "CONTROLLER_4")
    {
        return ControlSchemeInfo::s_controlSchemeIndex_controller4;
    }

    return ControlSchemeInfo::s_controlSchemeIndex_unknown;
}

void SharedApplicationData::UpdateKeyboardButtonPresses(const Project001::KeyEvent& keyEvent)
{
    const Project001::KeyCode& keyCode = keyEvent.keyCode;
    const Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    const Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (keyCode == keyboard_1_pause_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_1_pause_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_1_pause_pressed = false;
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
    if (keyCode == keyboard_1_drop_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_1_drop_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_1_drop_pressed = false;
    }

    if (keyCode == keyboard_2_pause_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_2_pause_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_2_pause_pressed = false;
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
    if (keyCode == keyboard_2_drop_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_2_drop_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_2_drop_pressed = false;
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
    if (keyCode == debug_keyboard_left_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_left_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_left_pressed = false;
    }
    if (keyCode == debug_keyboard_right_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_right_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_right_pressed = false;
    }
    if (keyCode == debug_keyboard_up_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_up_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_up_pressed = false;
    }
    if (keyCode == debug_keyboard_down_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_down_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_down_pressed = false;
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
    {
        ControlSchemeInfo& controlSchemeInfo = controlSchemeInfos[ControlSchemeInfo::s_controlSchemeIndex_keyboard1];

        if (keyboard_1_pause_pressed) controlSchemeInfo.pause_pressCount++;
        else controlSchemeInfo.pause_pressCount = 0;

        if (keyboard_1_left_pressed) controlSchemeInfo.left_pressCount++;
        else controlSchemeInfo.left_pressCount = 0;

        if (keyboard_1_right_pressed) controlSchemeInfo.right_pressCount++;
        else controlSchemeInfo.right_pressCount = 0;

        if (keyboard_1_up_pressed) controlSchemeInfo.up_pressCount++;
        else controlSchemeInfo.up_pressCount = 0;

        if (keyboard_1_down_pressed) controlSchemeInfo.down_pressCount++;
        else controlSchemeInfo.down_pressCount = 0;

        if (keyboard_1_grab_pressed) controlSchemeInfo.grab_pressCount++;
        else controlSchemeInfo.grab_pressCount = 0;

        if (keyboard_1_drop_pressed) controlSchemeInfo.drop_pressCount++;
        else controlSchemeInfo.drop_pressCount = 0;
    }

    {
        ControlSchemeInfo& controlSchemeInfo = controlSchemeInfos[ControlSchemeInfo::s_controlSchemeIndex_keyboard2];

        if (keyboard_2_pause_pressed) controlSchemeInfo.pause_pressCount++;
        else controlSchemeInfo.pause_pressCount = 0;

        if (keyboard_2_left_pressed) controlSchemeInfo.left_pressCount++;
        else controlSchemeInfo.left_pressCount = 0;

        if (keyboard_2_right_pressed) controlSchemeInfo.right_pressCount++;
        else controlSchemeInfo.right_pressCount = 0;

        if (keyboard_2_up_pressed) controlSchemeInfo.up_pressCount++;
        else controlSchemeInfo.up_pressCount = 0;

        if (keyboard_2_down_pressed) controlSchemeInfo.down_pressCount++;
        else controlSchemeInfo.down_pressCount = 0;

        if (keyboard_2_grab_pressed) controlSchemeInfo.grab_pressCount++;
        else controlSchemeInfo.grab_pressCount = 0;

        if (keyboard_2_drop_pressed) controlSchemeInfo.drop_pressCount++;
        else controlSchemeInfo.drop_pressCount = 0;
    }

    {
        ControlSchemeInfo& controlSchemeInfo = controlSchemeInfos[ControlSchemeInfo::s_controlSchemeIndex_controller1];

        std::vector<bool> buttonValues;
        windowPtr->GetJoystickButtons(0, buttonValues);

        if (buttonValues.size() > controller_1_pause_buttonIndex && buttonValues[controller_1_pause_buttonIndex]) controlSchemeInfo.pause_pressCount++;
        else  controlSchemeInfo.pause_pressCount = 0;

        if (buttonValues.size() > controller_1_left_buttonIndex && buttonValues[controller_1_left_buttonIndex]) controlSchemeInfo.left_pressCount++;
        else controlSchemeInfo.left_pressCount = 0;

        if (buttonValues.size() > controller_1_right_buttonIndex && buttonValues[controller_1_right_buttonIndex]) controlSchemeInfo.right_pressCount++;
        else controlSchemeInfo.right_pressCount = 0;

        if (buttonValues.size() > controller_1_up_buttonIndex && buttonValues[controller_1_up_buttonIndex]) controlSchemeInfo.up_pressCount++;
        else controlSchemeInfo.up_pressCount = 0;

        if (buttonValues.size() > controller_1_down_buttonIndex && buttonValues[controller_1_down_buttonIndex]) controlSchemeInfo.down_pressCount++;
        else controlSchemeInfo.down_pressCount = 0;

        if (buttonValues.size() > controller_1_grab_buttonIndex && buttonValues[controller_1_grab_buttonIndex]) controlSchemeInfo.grab_pressCount++;
        else controlSchemeInfo.grab_pressCount = 0;

        if (buttonValues.size() > controller_1_drop_buttonIndex && buttonValues[controller_1_drop_buttonIndex]) controlSchemeInfo.drop_pressCount++;
        else controlSchemeInfo.drop_pressCount = 0;

        std::vector<float> axisValues;
        windowPtr->GetJoystickAxis(0, axisValues);

        if (axisValues.size() > controller_1_moveRightLeft_axisIndex) controlSchemeInfo.leftRightAxisValue = axisValues[controller_1_moveRightLeft_axisIndex];

        if (axisValues.size() > controller_1_moveDownUp_axisIndex) controlSchemeInfo.upDownAxisValue = -axisValues[controller_1_moveDownUp_axisIndex];
    }

    {
        ControlSchemeInfo& controlSchemeInfo = controlSchemeInfos[ControlSchemeInfo::s_controlSchemeIndex_controller2];

        std::vector<bool> buttonValues;
        windowPtr->GetJoystickButtons(1, buttonValues);

        if (buttonValues.size() > controller_2_pause_buttonIndex && buttonValues[controller_2_pause_buttonIndex]) controlSchemeInfo.pause_pressCount++;
        else  controlSchemeInfo.pause_pressCount = 0;

        if (buttonValues.size() > controller_2_left_buttonIndex && buttonValues[controller_2_left_buttonIndex]) controlSchemeInfo.left_pressCount++;
        else controlSchemeInfo.left_pressCount = 0;

        if (buttonValues.size() > controller_2_right_buttonIndex && buttonValues[controller_2_right_buttonIndex]) controlSchemeInfo.right_pressCount++;
        else controlSchemeInfo.right_pressCount = 0;

        if (buttonValues.size() > controller_2_up_buttonIndex && buttonValues[controller_2_up_buttonIndex]) controlSchemeInfo.up_pressCount++;
        else controlSchemeInfo.up_pressCount = 0;

        if (buttonValues.size() > controller_2_down_buttonIndex && buttonValues[controller_2_down_buttonIndex]) controlSchemeInfo.down_pressCount++;
        else controlSchemeInfo.down_pressCount = 0;

        if (buttonValues.size() > controller_2_grab_buttonIndex && buttonValues[controller_2_grab_buttonIndex]) controlSchemeInfo.grab_pressCount++;
        else controlSchemeInfo.grab_pressCount = 0;

        if (buttonValues.size() > controller_2_drop_buttonIndex && buttonValues[controller_2_drop_buttonIndex]) controlSchemeInfo.drop_pressCount++;
        else controlSchemeInfo.drop_pressCount = 0;

        std::vector<float> axisValues;
        windowPtr->GetJoystickAxis(1, axisValues);

        if (axisValues.size() > controller_2_moveRightLeft_axisIndex) controlSchemeInfo.leftRightAxisValue = axisValues[controller_2_moveRightLeft_axisIndex];

        if (axisValues.size() > controller_2_moveDownUp_axisIndex) controlSchemeInfo.upDownAxisValue = -axisValues[controller_2_moveDownUp_axisIndex];
    }

    {
        ControlSchemeInfo& controlSchemeInfo = controlSchemeInfos[ControlSchemeInfo::s_controlSchemeIndex_controller3];

        std::vector<bool> buttonValues;
        windowPtr->GetJoystickButtons(1, buttonValues);

        if (buttonValues.size() > controller_3_pause_buttonIndex && buttonValues[controller_3_pause_buttonIndex]) controlSchemeInfo.pause_pressCount++;
        else  controlSchemeInfo.pause_pressCount = 0;

        if (buttonValues.size() > controller_3_left_buttonIndex && buttonValues[controller_3_left_buttonIndex]) controlSchemeInfo.left_pressCount++;
        else controlSchemeInfo.left_pressCount = 0;

        if (buttonValues.size() > controller_3_right_buttonIndex && buttonValues[controller_3_right_buttonIndex]) controlSchemeInfo.right_pressCount++;
        else controlSchemeInfo.right_pressCount = 0;

        if (buttonValues.size() > controller_3_up_buttonIndex && buttonValues[controller_3_up_buttonIndex]) controlSchemeInfo.up_pressCount++;
        else controlSchemeInfo.up_pressCount = 0;

        if (buttonValues.size() > controller_3_down_buttonIndex && buttonValues[controller_3_down_buttonIndex]) controlSchemeInfo.down_pressCount++;
        else controlSchemeInfo.down_pressCount = 0;

        if (buttonValues.size() > controller_3_grab_buttonIndex && buttonValues[controller_3_grab_buttonIndex]) controlSchemeInfo.grab_pressCount++;
        else controlSchemeInfo.grab_pressCount = 0;

        if (buttonValues.size() > controller_3_drop_buttonIndex && buttonValues[controller_3_drop_buttonIndex]) controlSchemeInfo.drop_pressCount++;
        else controlSchemeInfo.drop_pressCount = 0;

        std::vector<float> axisValues;
        windowPtr->GetJoystickAxis(2, axisValues);

        if (axisValues.size() > controller_3_moveRightLeft_axisIndex) controlSchemeInfo.leftRightAxisValue = axisValues[controller_3_moveRightLeft_axisIndex];

        if (axisValues.size() > controller_3_moveDownUp_axisIndex) controlSchemeInfo.upDownAxisValue = -axisValues[controller_3_moveDownUp_axisIndex];
    }

    {
        ControlSchemeInfo& controlSchemeInfo = controlSchemeInfos[ControlSchemeInfo::s_controlSchemeIndex_controller4];

        std::vector<bool> buttonValues;
        windowPtr->GetJoystickButtons(3, buttonValues);

        if (buttonValues.size() > controller_4_pause_buttonIndex && buttonValues[controller_4_pause_buttonIndex]) controlSchemeInfo.pause_pressCount++;
        else  controlSchemeInfo.pause_pressCount = 0;

        if (buttonValues.size() > controller_4_left_buttonIndex && buttonValues[controller_4_left_buttonIndex]) controlSchemeInfo.left_pressCount++;
        else controlSchemeInfo.left_pressCount = 0;

        if (buttonValues.size() > controller_4_right_buttonIndex && buttonValues[controller_4_right_buttonIndex]) controlSchemeInfo.right_pressCount++;
        else controlSchemeInfo.right_pressCount = 0;

        if (buttonValues.size() > controller_4_up_buttonIndex && buttonValues[controller_4_up_buttonIndex]) controlSchemeInfo.up_pressCount++;
        else controlSchemeInfo.up_pressCount = 0;

        if (buttonValues.size() > controller_4_down_buttonIndex && buttonValues[controller_4_down_buttonIndex]) controlSchemeInfo.down_pressCount++;
        else controlSchemeInfo.down_pressCount = 0;

        if (buttonValues.size() > controller_4_grab_buttonIndex && buttonValues[controller_4_grab_buttonIndex]) controlSchemeInfo.grab_pressCount++;
        else controlSchemeInfo.grab_pressCount = 0;

        if (buttonValues.size() > controller_4_drop_buttonIndex && buttonValues[controller_4_drop_buttonIndex]) controlSchemeInfo.drop_pressCount++;
        else controlSchemeInfo.drop_pressCount = 0;

        std::vector<float> axisValues;
        windowPtr->GetJoystickAxis(3, axisValues);

        if (axisValues.size() > controller_4_moveRightLeft_axisIndex) controlSchemeInfo.leftRightAxisValue = axisValues[controller_4_moveRightLeft_axisIndex];

        if (axisValues.size() > controller_4_moveDownUp_axisIndex) controlSchemeInfo.upDownAxisValue = -axisValues[controller_4_moveDownUp_axisIndex];
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

    if (debug_keyboard_left_pressed) debug_keyboard_left_pressCount++;
    else debug_keyboard_left_pressCount = 0;

    if (debug_keyboard_right_pressed) debug_keyboard_right_pressCount++;
    else debug_keyboard_right_pressCount = 0;

    if (debug_keyboard_up_pressed) debug_keyboard_up_pressCount++;
    else debug_keyboard_up_pressCount = 0;

    if (debug_keyboard_down_pressed) debug_keyboard_down_pressCount++;
    else debug_keyboard_down_pressCount = 0;

    if (debug_mouse_grab_pressed) debug_mouse_grab_pressCount++;
    else debug_mouse_grab_pressCount = 0;

    if (debug_mouse_grab_released) debug_mouse_grab_releaseCount++;
    else debug_mouse_grab_releaseCount = 0;
}