#pragma once

#include <string>



namespace Project001
{	
	enum class EventType
	{
		EVENT_TYPE_NONE = 0,

		EVENT_TYPE_KEY,
		EVENT_TYPE_MOUSE_BUTTON,
		EVENT_TYPE_CURSOR_POS,
		EVENT_TYPE_CURSOR_ENTER,
		EVENT_TYPE_SCROLL,
		EVENT_TYPE_WINDOW_CLOSE,
		EVENT_TYPE_WINDOW_FOCUS,
		EVENT_TYPE_FRAMEBUFFER_SIZE,

		EVENT_TYPE_UPDATE
	};

	static std::string EventTypeToString(EventType eventType)
	{
		switch (eventType)
		{
		case EventType::EVENT_TYPE_NONE:
		{
			return std::string("EVENT_TYPE_NONE");
		}
		case EventType::EVENT_TYPE_KEY:
		{
			return std::string("EVENT_TYPE_KEY");
		}
		case EventType::EVENT_TYPE_MOUSE_BUTTON:
		{
			return std::string("EVENT_TYPE_MOUSE_BUTTON");
		}
		case EventType::EVENT_TYPE_CURSOR_POS:
		{
			return std::string("EVENT_TYPE_CURSOR_POS");
		}
		case EventType::EVENT_TYPE_CURSOR_ENTER:
		{
			return std::string("EVENT_TYPE_CURSOR_ENTER");
		}
		case EventType::EVENT_TYPE_SCROLL:
		{
			return std::string("EVENT_TYPE_SCROLL");
		}
		case EventType::EVENT_TYPE_WINDOW_CLOSE:
		{
			return std::string("EVENT_TYPE_WINDOW_CLOSE");
		}
		case EventType::EVENT_TYPE_WINDOW_FOCUS:
		{
			return std::string("EVENT_TYPE_WINDOW_FOCUS");
		}
		case EventType::EVENT_TYPE_FRAMEBUFFER_SIZE:
		{
			return std::string("EVENT_TYPE_FRAMEBUFFER_SIZE");
		}
		case EventType::EVENT_TYPE_UPDATE:
		{
			return std::string("EVENT_TYPE_UPDATE");
		}
		}
		return std::string("UNKNOWN");
	}
	
	// same as the GLFW key actions
	enum class ButtonAction
	{
		KEY_ACTION_RELEASE = 0,
		KEY_ACTION_PRESS = 1,
		KEY_ACTION_REPEAT = 2
	};

	static std::string ButtonActionToString(ButtonAction buttonAction)
	{
		switch(buttonAction)
		{
		case ButtonAction::KEY_ACTION_RELEASE:
		{
			return std::string("KEY_ACTION_RELEASE");
		}
		case ButtonAction::KEY_ACTION_PRESS:
		{
			return std::string("KEY_ACTION_PRESS");
		}
		case ButtonAction::KEY_ACTION_REPEAT:
		{
			return std::string("KEY_ACTION_REPEAT");
		}
		}
		return std::string("UNKNOWN");
	}
	
	// same as the GLFW keboard key codes
	enum class KeyCode
	{
		KEY_CODE_UNKNOWN = -1,
		KEY_CODE_SPACE = 32,
		KEY_CODE_APOSTROPHE = 39, /* ' */
		KEY_CODE_COMMA = 44, /* , */
		KEY_CODE_MINUS = 45, /* - */
		KEY_CODE_PERIOD = 46, /* . */
		KEY_CODE_SLASH = 47, /* / */
		KEY_CODE_0 = 48,
		KEY_CODE_1 = 49,
		KEY_CODE_2 = 50,
		KEY_CODE_3 = 51,
		KEY_CODE_4 = 52,
		KEY_CODE_5 = 53,
		KEY_CODE_6 = 54,
		KEY_CODE_7 = 55,
		KEY_CODE_8 = 56,
		KEY_CODE_9 = 57,
		KEY_CODE_SEMICOLON = 59, /* ; */
		KEY_CODE_EQUAL = 61, /* = */
		KEY_CODE_A = 65,
		KEY_CODE_B = 66,
		KEY_CODE_C = 67,
		KEY_CODE_D = 68,
		KEY_CODE_E = 69,
		KEY_CODE_F = 70,
		KEY_CODE_G = 71,
		KEY_CODE_H = 72,
		KEY_CODE_I = 73,
		KEY_CODE_J = 74,
		KEY_CODE_K = 75,
		KEY_CODE_L = 76,
		KEY_CODE_M = 77,
		KEY_CODE_N = 78,
		KEY_CODE_O = 79,
		KEY_CODE_P = 80,
		KEY_CODE_Q = 81,
		KEY_CODE_R = 82,
		KEY_CODE_S = 83,
		KEY_CODE_T = 84,
		KEY_CODE_U = 85,
		KEY_CODE_V = 86,
		KEY_CODE_W = 87,
		KEY_CODE_X = 88,
		KEY_CODE_Y = 89,
		KEY_CODE_Z = 90,
		KEY_CODE_LEFT_BRACKET = 91, /* [ */
		KEY_CODE_BACKSLASH = 92, /* \ */
		KEY_CODE_RIGHT_BRACKET = 93, /* ] */
		KEY_CODE_GRAVE_ACCENT = 96, /* ` */
		KEY_CODE_WORLD_1 = 161, /* non-US #1 */
		KEY_CODE_WORLD_2 = 162, /* non-US #2 */
		KEY_CODE_ESCAPE = 256,
		KEY_CODE_ENTER = 257,
		KEY_CODE_TAB = 258,
		KEY_CODE_BACKSPACE = 259,
		KEY_CODE_INSERT = 260,
		KEY_CODE_DELETE = 261,
		KEY_CODE_RIGHT = 262,
		KEY_CODE_LEFT = 263,
		KEY_CODE_DOWN = 264,
		KEY_CODE_UP = 265,
		KEY_CODE_PAGE_UP = 266,
		KEY_CODE_PAGE_DOWN = 267,
		KEY_CODE_HOME = 268,
		KEY_CODE_END = 269,
		KEY_CODE_CAPS_LOCK = 280,
		KEY_CODE_SCROLL_LOCK = 281,
		KEY_CODE_NUM_LOCK = 282,
		KEY_CODE_PRINT_SCREEN = 283,
		KEY_CODE_PAUSE = 284,
		KEY_CODE_F1 = 290,
		KEY_CODE_F2 = 291,
		KEY_CODE_F3 = 292,
		KEY_CODE_F4 = 293,
		KEY_CODE_F5 = 294,
		KEY_CODE_F6 = 295,
		KEY_CODE_F7 = 296,
		KEY_CODE_F8 = 297,
		KEY_CODE_F9 = 298,
		KEY_CODE_F10 = 299,
		KEY_CODE_F11 = 300,
		KEY_CODE_F12 = 301,
		KEY_CODE_F13 = 302,
		KEY_CODE_F14 = 303,
		KEY_CODE_F15 = 304,
		KEY_CODE_F16 = 305,
		KEY_CODE_F17 = 306,
		KEY_CODE_F18 = 307,
		KEY_CODE_F19 = 308,
		KEY_CODE_F20 = 309,
		KEY_CODE_F21 = 310,
		KEY_CODE_F22 = 311,
		KEY_CODE_F23 = 312,
		KEY_CODE_F24 = 313,
		KEY_CODE_F25 = 314,
		KEY_CODE_KP_0 = 320,
		KEY_CODE_KP_1 = 321,
		KEY_CODE_KP_2 = 322,
		KEY_CODE_KP_3 = 323,
		KEY_CODE_KP_4 = 324,
		KEY_CODE_KP_5 = 325,
		KEY_CODE_KP_6 = 326,
		KEY_CODE_KP_7 = 327,
		KEY_CODE_KP_8 = 328,
		KEY_CODE_KP_9 = 329,
		KEY_CODE_KP_DECIMAL = 330,
		KEY_CODE_KP_DIVIDE = 331,
		KEY_CODE_KP_MULTIPLY = 332,
		KEY_CODE_KP_SUBTRACT = 333,
		KEY_CODE_KP_ADD = 334,
		KEY_CODE_KP_ENTER = 335,
		KEY_CODE_KP_EQUAL = 336,
		KEY_CODE_LEFT_SHIFT = 340,
		KEY_CODE_LEFT_CONTROL = 341,
		KEY_CODE_LEFT_ALT = 342,
		KEY_CODE_LEFT_SUPER = 343,
		KEY_CODE_RIGHT_SHIFT = 344,
		KEY_CODE_RIGHT_CONTROL = 345,
		KEY_CODE_RIGHT_ALT = 346,
		KEY_CODE_RIGHT_SUPER = 347,
		KEY_CODE_MENU = 348,
	};

	static std::string KeyCodeToString(KeyCode keyCode)
	{
		switch (keyCode)
		{
		case KeyCode::KEY_CODE_UNKNOWN:
		{
			return std::string("KEY_CODE_UNKNOWN");
		}
		case KeyCode::KEY_CODE_SPACE:
		{
			return std::string("KEY_CODE_SPACE");
		}
		case KeyCode::KEY_CODE_APOSTROPHE:
		{
			return std::string("KEY_CODE_APOSTROPHE");
		}
		case KeyCode::KEY_CODE_COMMA:
		{
			return std::string("KEY_CODE_COMMA");
		}
		case KeyCode::KEY_CODE_MINUS:
		{
			return std::string("KEY_CODE_MINUS");
		}
		case KeyCode::KEY_CODE_PERIOD:
		{
			return std::string("KEY_CODE_PERIOD");
		}
		case KeyCode::KEY_CODE_SLASH:
		{
			return std::string("KEY_CODE_SLASH");
		}
		case KeyCode::KEY_CODE_0:
		{
			return std::string("KEY_CODE_0");
		}
		case KeyCode::KEY_CODE_1:
		{
			return std::string("KEY_CODE_1");
		}
		case KeyCode::KEY_CODE_2:
		{
			return std::string("KEY_CODE_2");
		}
		case KeyCode::KEY_CODE_3:
		{
			return std::string("KEY_CODE_3");
		}
		case KeyCode::KEY_CODE_4:
		{
			return std::string("KEY_CODE_4");
		}
		case KeyCode::KEY_CODE_5:
		{
			return std::string("KEY_CODE_5");
		}
		case KeyCode::KEY_CODE_6:
		{
			return std::string("KEY_CODE_6");
		}
		case KeyCode::KEY_CODE_7:
		{
			return std::string("KEY_CODE_7");
		}
		case KeyCode::KEY_CODE_8:
		{
			return std::string("KEY_CODE_8");
		}
		case KeyCode::KEY_CODE_9:
		{
			return std::string("KEY_CODE_9");
		}
		case KeyCode::KEY_CODE_SEMICOLON:
		{
			return std::string("KEY_CODE_SEMICOLON");
		}
		case KeyCode::KEY_CODE_EQUAL:
		{
			return std::string("KEY_CODE_EQUAL");
		}
		case KeyCode::KEY_CODE_A:
		{
			return std::string("KEY_CODE_A");
		}
		case KeyCode::KEY_CODE_B:
		{
			return std::string("KEY_CODE_B");
		}
		case KeyCode::KEY_CODE_C:
		{
			return std::string("KEY_CODE_C");
		}
		case KeyCode::KEY_CODE_D:
		{
			return std::string("KEY_CODE_D");
		}
		case KeyCode::KEY_CODE_E:
		{
			return std::string("KEY_CODE_E");
		}
		case KeyCode::KEY_CODE_F:
		{
			return std::string("KEY_CODE_F");
		}
		case KeyCode::KEY_CODE_G:
		{
			return std::string("KEY_CODE_G");
		}
		case KeyCode::KEY_CODE_H:
		{
			return std::string("KEY_CODE_H");
		}
		case KeyCode::KEY_CODE_I:
		{
			return std::string("KEY_CODE_I");
		}
		case KeyCode::KEY_CODE_J:
		{
			return std::string("KEY_CODE_J");
		}
		case KeyCode::KEY_CODE_K:
		{
			return std::string("KEY_CODE_K");
		}
		case KeyCode::KEY_CODE_L:
		{
			return std::string("KEY_CODE_L");
		}
		case KeyCode::KEY_CODE_M:
		{
			return std::string("KEY_CODE_M");
		}
		case KeyCode::KEY_CODE_N:
		{
			return std::string("KEY_CODE_N");
		}
		case KeyCode::KEY_CODE_O:
		{
			return std::string("KEY_CODE_O");
		}
		case KeyCode::KEY_CODE_P:
		{
			return std::string("KEY_CODE_P");
		}
		case KeyCode::KEY_CODE_Q:
		{
			return std::string("KEY_CODE_Q");
		}
		case KeyCode::KEY_CODE_R:
		{
			return std::string("KEY_CODE_R");
		}
		case KeyCode::KEY_CODE_S:
		{
			return std::string("KEY_CODE_S");
		}
		case KeyCode::KEY_CODE_T:
		{
			return std::string("KEY_CODE_T");
		}
		case KeyCode::KEY_CODE_U:
		{
			return std::string("KEY_CODE_U");
		}
		case KeyCode::KEY_CODE_V:
		{
			return std::string("KEY_CODE_V");
		}
		case KeyCode::KEY_CODE_W:
		{
			return std::string("KEY_CODE_W");
		}
		case KeyCode::KEY_CODE_X:
		{
			return std::string("KEY_CODE_X");
		}
		case KeyCode::KEY_CODE_Y:
		{
			return std::string("KEY_CODE_Y");
		}
		case KeyCode::KEY_CODE_Z:
		{
			return std::string("KEY_CODE_Z");
		}
		case KeyCode::KEY_CODE_LEFT_BRACKET:
		{
			return std::string("KEY_CODE_LEFT_BRACKET");
		}
		case KeyCode::KEY_CODE_BACKSLASH:
		{
			return std::string("KEY_CODE_BACKSLASH");
		}
		case KeyCode::KEY_CODE_RIGHT_BRACKET:
		{
			return std::string("KEY_CODE_RIGHT_BRACKET");
		}
		case KeyCode::KEY_CODE_GRAVE_ACCENT:
		{
			return std::string("KEY_CODE_GRAVE_ACCENT");
		}
		case KeyCode::KEY_CODE_WORLD_1:
		{
			return std::string("KEY_CODE_WORLD_1");
		}
		case KeyCode::KEY_CODE_WORLD_2:
		{
			return std::string("KEY_CODE_WORLD_2");
		}
		case KeyCode::KEY_CODE_ESCAPE:
		{
			return std::string("KEY_CODE_ESCAPE");
		}
		case KeyCode::KEY_CODE_ENTER:
		{
			return std::string("KEY_CODE_ENTER");
		}
		case KeyCode::KEY_CODE_TAB:
		{
			return std::string("KEY_CODE_TAB");
		}
		case KeyCode::KEY_CODE_BACKSPACE:
		{
			return std::string("KEY_CODE_BACKSPACE");
		}
		case KeyCode::KEY_CODE_INSERT:
		{
			return std::string("KEY_CODE_INSERT");
		}
		case KeyCode::KEY_CODE_DELETE:
		{
			return std::string("KEY_CODE_DELETE");
		}
		case KeyCode::KEY_CODE_RIGHT:
		{
			return std::string("KEY_CODE_RIGHT");
		}
		case KeyCode::KEY_CODE_LEFT:
		{
			return std::string("KEY_CODE_LEFT");
		}
		case KeyCode::KEY_CODE_DOWN:
		{
			return std::string("KEY_CODE_DOWN");
		}
		case KeyCode::KEY_CODE_UP:
		{
			return std::string("KEY_CODE_UP");
		}
		case KeyCode::KEY_CODE_PAGE_UP:
		{
			return std::string("KEY_CODE_PAGE_UP");
		}
		case KeyCode::KEY_CODE_PAGE_DOWN:
		{
			return std::string("KEY_CODE_PAGE_DOWN");
		}
		case KeyCode::KEY_CODE_HOME:
		{
			return std::string("KEY_CODE_HOME");
		}
		case KeyCode::KEY_CODE_END:
		{
			return std::string("KEY_CODE_END");
		}
		case KeyCode::KEY_CODE_CAPS_LOCK:
		{
			return std::string("KEY_CODE_CAPS_LOCK");
		}
		case KeyCode::KEY_CODE_SCROLL_LOCK:
		{
			return std::string("KEY_CODE_SCROLL_LOCK");
		}
		case KeyCode::KEY_CODE_NUM_LOCK:
		{
			return std::string("KEY_CODE_NUM_LOCK");
		}
		case KeyCode::KEY_CODE_PRINT_SCREEN:
		{
			return std::string("KEY_CODE_PRINT_SCREEN");
		}
		case KeyCode::KEY_CODE_PAUSE:
		{
			return std::string("KEY_CODE_PAUSE");
		}
		case KeyCode::KEY_CODE_F1:
		{
			return std::string("KEY_CODE_F1");
		}
		case KeyCode::KEY_CODE_F2:
		{
			return std::string("KEY_CODE_F2");
		}
		case KeyCode::KEY_CODE_F3:
		{
			return std::string("KEY_CODE_F3");
		}
		case KeyCode::KEY_CODE_F4:
		{
			return std::string("KEY_CODE_F4");
		}
		case KeyCode::KEY_CODE_F5:
		{
			return std::string("KEY_CODE_F5");
		}
		case KeyCode::KEY_CODE_F6:
		{
			return std::string("KEY_CODE_F6");
		}
		case KeyCode::KEY_CODE_F7:
		{
			return std::string("KEY_CODE_F7");
		}
		case KeyCode::KEY_CODE_F8:
		{
			return std::string("KEY_CODE_F8");
		}
		case KeyCode::KEY_CODE_F9:
		{
			return std::string("KEY_CODE_F9");
		}
		case KeyCode::KEY_CODE_F10:
		{
			return std::string("KEY_CODE_F10");
		}
		case KeyCode::KEY_CODE_F11:
		{
			return std::string("KEY_CODE_F11");
		}
		case KeyCode::KEY_CODE_F12:
		{
			return std::string("KEY_CODE_F12");
		}
		case KeyCode::KEY_CODE_F13:
		{
			return std::string("KEY_CODE_F13");
		}
		case KeyCode::KEY_CODE_F14:
		{
			return std::string("KEY_CODE_F14");
		}
		case KeyCode::KEY_CODE_F15:
		{
			return std::string("KEY_CODE_F15");
		}
		case KeyCode::KEY_CODE_F16:
		{
			return std::string("KEY_CODE_F16");
		}
		case KeyCode::KEY_CODE_F17:
		{
			return std::string("KEY_CODE_F17");
		}
		case KeyCode::KEY_CODE_F18:
		{
			return std::string("KEY_CODE_F18");
		}
		case KeyCode::KEY_CODE_F19:
		{
			return std::string("KEY_CODE_F19");
		}
		case KeyCode::KEY_CODE_F20:
		{
			return std::string("KEY_CODE_F20");
		}
		case KeyCode::KEY_CODE_F21:
		{
			return std::string("KEY_CODE_F21");
		}
		case KeyCode::KEY_CODE_F22:
		{
			return std::string("KEY_CODE_F22");
		}
		case KeyCode::KEY_CODE_F23:
		{
			return std::string("KEY_CODE_F23");
		}
		case KeyCode::KEY_CODE_F24:
		{
			return std::string("KEY_CODE_F24");
		}
		case KeyCode::KEY_CODE_F25:
		{
			return std::string("KEY_CODE_F25");
		}
		case KeyCode::KEY_CODE_KP_0:
		{
			return std::string("KEY_CODE_KP_0");
		}
		case KeyCode::KEY_CODE_KP_1:
		{
			return std::string("KEY_CODE_KP_1");
		}
		case KeyCode::KEY_CODE_KP_2:
		{
			return std::string("KEY_CODE_KP_2");
		}
		case KeyCode::KEY_CODE_KP_3:
		{
			return std::string("KEY_CODE_KP_3");
		}
		case KeyCode::KEY_CODE_KP_4:
		{
			return std::string("KEY_CODE_KP_4");
		}
		case KeyCode::KEY_CODE_KP_5:
		{
			return std::string("KEY_CODE_KP_5");
		}
		case KeyCode::KEY_CODE_KP_6:
		{
			return std::string("KEY_CODE_KP_6");
		}
		case KeyCode::KEY_CODE_KP_7:
		{
			return std::string("KEY_CODE_KP_7");
		}
		case KeyCode::KEY_CODE_KP_8:
		{
			return std::string("KEY_CODE_KP_8");
		}
		case KeyCode::KEY_CODE_KP_9:
		{
			return std::string("KEY_CODE_KP_9");
		}
		case KeyCode::KEY_CODE_KP_DECIMAL:
		{
			return std::string("KEY_CODE_KP_DECIMAL");
		}
		case KeyCode::KEY_CODE_KP_DIVIDE:
		{
			return std::string("KEY_CODE_KP_DIVIDE");
		}
		case KeyCode::KEY_CODE_KP_MULTIPLY:
		{
			return std::string("KEY_CODE_KP_MULTIPLY");
		}
		case KeyCode::KEY_CODE_KP_SUBTRACT:
		{
			return std::string("KEY_CODE_KP_SUBTRACT");
		}
		case KeyCode::KEY_CODE_KP_ADD:
		{
			return std::string("KEY_CODE_KP_ADD");
		}
		case KeyCode::KEY_CODE_KP_ENTER:
		{
			return std::string("KEY_CODE_KP_ENTER");
		}
		case KeyCode::KEY_CODE_KP_EQUAL:
		{
			return std::string("KEY_CODE_KP_EQUAL");
		}
		case KeyCode::KEY_CODE_LEFT_SHIFT:
		{
			return std::string("KEY_CODE_LEFT_SHIFT");
		}
		case KeyCode::KEY_CODE_LEFT_CONTROL:
		{
			return std::string("KEY_CODE_LEFT_CONTROL");
		}
		case KeyCode::KEY_CODE_LEFT_ALT:
		{
			return std::string("KEY_CODE_LEFT_ALT");
		}
		case KeyCode::KEY_CODE_LEFT_SUPER:
		{
			return std::string("KEY_CODE_LEFT_SUPER");
		}
		case KeyCode::KEY_CODE_RIGHT_SHIFT:
		{
			return std::string("KEY_CODE_RIGHT_SHIFT");
		}
		case KeyCode::KEY_CODE_RIGHT_CONTROL:
		{
			return std::string("KEY_CODE_RIGHT_CONTROL");
		}
		case KeyCode::KEY_CODE_RIGHT_ALT:
		{
			return std::string("KEY_CODE_RIGHT_ALT");
		}
		case KeyCode::KEY_CODE_RIGHT_SUPER:
		{
			return std::string("KEY_CODE_RIGHT_SUPER");
		}
		case KeyCode::KEY_CODE_MENU:
		{
			return std::string("KEY_CODE_MENU");
		}
		}
		return std::string("UNKNOWN");
	}

	// same as the GLFW keyboard modifiers
	typedef unsigned int KeyModifier;
	const KeyModifier MASK_KEY_MODIFIER_SHIFT = (1 << 0);
	const KeyModifier MASK_KEY_MODIFIER_CONTROL = (1 << 1);
	const KeyModifier MASK_KEY_MODIFIER_ALT = (1 << 2);
	const KeyModifier MASK_KEY_MODIFIER_SUPER = (1 << 3);
	const KeyModifier MASK_KEY_MODIFIER_CAPS_LOCK = (1 << 4);
	const KeyModifier MASK_KEY_MODIFIER_NUM_LOCK = (1 << 5);

	static std::string KeyModifierToString(KeyModifier keyModifier)
	{
		std::string returnString("KeyModifiers:");
		if (keyModifier & MASK_KEY_MODIFIER_SHIFT)
		{
			returnString += " MASK_KEY_MODIFIER_SHIFT";
		}
		if (keyModifier & MASK_KEY_MODIFIER_CONTROL)
		{
			returnString += " MASK_KEY_MODIFIER_CONTROL";
		}
		if (keyModifier & MASK_KEY_MODIFIER_ALT)
		{
			returnString += " MASK_KEY_MODIFIER_ALT";
		}
		if (keyModifier & MASK_KEY_MODIFIER_SUPER)
		{
			returnString += " MASK_KEY_MODIFIER_SUPER";
		}
		if (keyModifier & MASK_KEY_MODIFIER_CAPS_LOCK)
		{
			returnString += " MASK_KEY_MODIFIER_CAPS_LOCK";
		}
		if (keyModifier & MASK_KEY_MODIFIER_NUM_LOCK)
		{
			returnString += " MASK_KEY_MODIFIER_NUM_LOCK";
		}
		if (returnString.size() == 0)
		{
			returnString += " NONE";
		}
		return returnString;
	}

	// same as the GLFW mouse button inputs
	enum class MouseButton
	{
		MOUSE_BUTTON_1 = 0,
		MOUSE_BUTTON_2 = 1,
		MOUSE_BUTTON_3 = 2,
		MOUSE_BUTTON_4 = 3,
		MOUSE_BUTTON_5 = 4,
		MOUSE_BUTTON_6 = 5,
		MOUSE_BUTTON_7 = 6,
		MOUSE_BUTTON_8 = 7,
		MOUSE_BUTTON_LAST = MOUSE_BUTTON_8,
		MOUSE_BUTTON_LEFT = MOUSE_BUTTON_1,
		MOUSE_BUTTON_RIGHT = MOUSE_BUTTON_2,
		MOUSE_BUTTON_MIDDLE = MOUSE_BUTTON_3
	};

	static std::string MouseButtonToString(MouseButton mouseButton)
	{
		switch (mouseButton)
		{
		case MouseButton::MOUSE_BUTTON_1:
		{
			return std::string("MOUSE_BUTTON_1");
		}
		case MouseButton::MOUSE_BUTTON_2:
		{
			return std::string("MOUSE_BUTTON_2");
		}
		case MouseButton::MOUSE_BUTTON_3:
		{
			return std::string("MOUSE_BUTTON_3");
		}
		case MouseButton::MOUSE_BUTTON_4:
		{
			return std::string("MOUSE_BUTTON_4");
		}
		case MouseButton::MOUSE_BUTTON_5:
		{
			return std::string("MOUSE_BUTTON_5");
		}
		case MouseButton::MOUSE_BUTTON_6:
		{
			return std::string("MOUSE_BUTTON_6");
		}
		case MouseButton::MOUSE_BUTTON_7:
		{
			return std::string("MOUSE_BUTTON_7");
		}
		case MouseButton::MOUSE_BUTTON_8:
		{
			return std::string("MOUSE_BUTTON_8");
		}
		}
		return std::string("UNKNOWN");
	}

	struct Event
	{
		Event()
			: handled(false)
		{}

		virtual EventType GetEventType() const = 0;

		bool handled;
	};

	// Maybe I'll use this...
	template<typename ChildEvent, typename Function>
	static bool DispatchEvent(Event& event, const Function& function)
	{
		if (event.GetEventType() == ChildEvent::GetStaticEventType())
		{
			(function)(static_cast<ChildEvent&>(event));
			return true;
		}
		return false;
	}

#define EVENT_TYPE_FUNCTIONS(eventType)\
	static EventType GetStaticEventType()\
	{\
		return eventType;\
	}\
	\
	EventType GetEventType() const override\
	{\
		return GetStaticEventType();\
	}

	// Events:
	// ----------------------------------------------------------------------------

	struct KeyEvent : Event
	{
		KeyEvent(KeyCode keyCode, ButtonAction buttonAction, KeyModifier keyModifier)
			: Event()
			, keyCode(keyCode)
			, buttonAction(buttonAction)
			, keyModifier(keyModifier)
		{}

		EVENT_TYPE_FUNCTIONS(EventType::EVENT_TYPE_KEY)

		KeyCode keyCode;
		ButtonAction buttonAction;
		KeyModifier keyModifier;
	};

	struct MouseButtonEvent : Event
	{
		MouseButtonEvent(MouseButton mouseButton, ButtonAction buttonAction, KeyModifier keyModifier)
			: Event()
			, mouseButton(mouseButton)
			, buttonAction(buttonAction)
			, keyModifier(keyModifier)
		{}

		EVENT_TYPE_FUNCTIONS(EventType::EVENT_TYPE_MOUSE_BUTTON)

		MouseButton mouseButton;
		ButtonAction buttonAction;
		KeyModifier keyModifier;
	};

	struct CursorPosEvent : Event
	{
		CursorPosEvent(float xPos, float yPos)
			: Event()
			, xPos(xPos)
			, yPos(yPos)
		{}
		
		EVENT_TYPE_FUNCTIONS(EventType::EVENT_TYPE_CURSOR_POS)

		// origin is at the top left
		float xPos, yPos;
	};

	struct CursorEnterEvent : Event
	{
		CursorEnterEvent(bool entered)
			: Event()
			, entered(entered)
		{}

		EVENT_TYPE_FUNCTIONS(EventType::EVENT_TYPE_CURSOR_ENTER)

		bool entered;
	};

	struct ScrollEvent : Event
	{
		ScrollEvent(float xOffset, float yOffset)
			: Event()
			, xOffset(xOffset)
			, yOffset(yOffset)
		{}

		EVENT_TYPE_FUNCTIONS(EventType::EVENT_TYPE_SCROLL)

		float xOffset, yOffset;
	};

	struct WindowCloseEvent : Event
	{
		WindowCloseEvent()
			: Event()
		{}

		EVENT_TYPE_FUNCTIONS(EventType::EVENT_TYPE_WINDOW_CLOSE)
	};

	struct WindowFocusEvent : Event
	{
		WindowFocusEvent(bool focused)
			: Event()
			, focused(focused)
		{}

		EVENT_TYPE_FUNCTIONS(EventType::EVENT_TYPE_WINDOW_FOCUS)

		bool focused;
	};

	struct FrameBufferSizeEvent : Event
	{
		FrameBufferSizeEvent(int width, int height)
			: Event()
			, width(width)
			, height(height)
		{}

		EVENT_TYPE_FUNCTIONS(EventType::EVENT_TYPE_FRAMEBUFFER_SIZE)

		int width, height;
	};

	struct UpdateEvent : Event
	{
		UpdateEvent(unsigned int threadId, double timestep_s)
			: Event()
			, threadId(threadId)
			, timestep_s(timestep_s)
		{}

		EVENT_TYPE_FUNCTIONS(EventType::EVENT_TYPE_UPDATE)

		unsigned int threadId;
		double timestep_s;
	};
}