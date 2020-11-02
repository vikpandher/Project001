
#include "RendererTestWidget.h"

#include "glm/gtc/constants.hpp" // need for pi
#include "glm/gtc/quaternion.hpp"

#include "Engine/Application.h"
#include "Engine/Logger.h"
#include "Engine/Renderer.h"
#include "Engine/Stores.h"



namespace Project001
{
	// public ------------------------------------------------------------------
	
	RendererTestWidget::RendererTestWidget()
		: applicationPtr_(nullptr)
		, windowPtr_(nullptr)
		, storesPtr_(nullptr)
		, rendererPtr_(nullptr)
		, widgetContainerPtr_(nullptr)
		, cameraPosition_(glm::vec3(0.0f, 0.0f, 3.0f))
		, cameraOrientation_(1.0f, 0.0f, 0.0f, 0.0f)
		, fieldOfVisionDegrees_(45.0f)
		, aspectRatio_(1.0f)
		, nearCutOff_(0.1f)
		, farCutOff_(100.0f)
		, moveForwardKeyCode_(KeyCode::KEY_CODE_Q)
		, moveBackKeyCode_(KeyCode::KEY_CODE_E)
		, moveLeftKeyCode_(KeyCode::KEY_CODE_A)
		, moveRightKeyCode_(KeyCode::KEY_CODE_D)
		, moveUpKeyCode_(KeyCode::KEY_CODE_W)
		, moveDownKeyCode_(KeyCode::KEY_CODE_S)
		, movingForward_(false)
		, movingLeft_(false)
		, movingRight_(false)
		, movingBack_(false)
		, movingUp_(false)
		, movingDown_(false)
		, mouseButton1_(MouseButton::MOUSE_BUTTON_RIGHT)
		, mouseButton1Down_(false)
		, lastCursorPositionX_(0.0f)
		, lastCursorPositionY_(0.0f)
		, cursorPositionX_(0.0f)
		, cursorPositionY_(0.0f)
	{
	}

	RendererTestWidget::~RendererTestWidget()
	{

	}

	void RendererTestWidget::Initialize(Application* applicationPtr)
	{
		applicationPtr_ = applicationPtr;
		windowPtr_ = applicationPtr->windowPtr_;
		storesPtr_ = applicationPtr->storesPtr_;
		rendererPtr_ = applicationPtr->rendererPtr_;
		widgetContainerPtr_ = &(applicationPtr->widgetMap_);

		aspectRatio_ = (float)applicationPtr->windowWidth_ / (float)applicationPtr->windowHeight_;

		storesPtr_->LoadOBJFile("Cube", "../Models/Cube.obj");
		rendererPtr_->AddMesh(storesPtr_->GetMesh("Cube"));

		storesPtr_->LoadTextureFile("Cube", "../Textures/Cube.png");
		rendererPtr_->AddTexture(storesPtr_->GetTexture("Cube"), 0);

		storesPtr_->LoadTextureFile("CounterclockwiseDie", "../Textures/CounterclockwiseDie.png");
		rendererPtr_->AddTexture(storesPtr_->GetTexture("CounterclockwiseDie"), 1);

		storesPtr_->LoadTextureFile("RubiksCube", "../Textures/RubiksCube.png");
		rendererPtr_->AddTexture(storesPtr_->GetTexture("RubiksCube"), 2);

		storesPtr_->LoadTextureFile("NumberDie", "../Textures/NumberDie.png");
		rendererPtr_->AddTexture(storesPtr_->GetTexture("NumberDie"), 3);
	}

	void RendererTestWidget::OnEvent(Event& event)
	{
		if (event.GetEventType() == EventType::EVENT_TYPE_KEY)
		{
			KeyEvent keyEvent = dynamic_cast<KeyEvent&>(event);
			
			bool actionValue_;
			if (keyEvent.buttonAction == ButtonAction::KEY_ACTION_RELEASE)
			{
				actionValue_ = false;
			}
			else
			{
				actionValue_ = true;
			}

			if (keyEvent.keyCode == moveForwardKeyCode_)
			{
				movingForward_ = actionValue_;
			}
			else if (keyEvent.keyCode == moveLeftKeyCode_)
			{
				movingLeft_ = actionValue_;
			}
			else if (keyEvent.keyCode == moveRightKeyCode_)
			{
				movingRight_ = actionValue_;
			}
			else if (keyEvent.keyCode == moveBackKeyCode_)
			{
				movingBack_ = actionValue_;
			}
			else if (keyEvent.keyCode == moveUpKeyCode_)
			{
				movingUp_ = actionValue_;
			}
			else if (keyEvent.keyCode == moveDownKeyCode_)
			{
				movingDown_ = actionValue_;
			}
		}
		else if (event.GetEventType() == EventType::EVENT_TYPE_MOUSE_BUTTON)
		{
			MouseButtonEvent mouseButtonEvent = dynamic_cast<MouseButtonEvent&>(event);

			bool actionValue_;
			if (mouseButtonEvent.buttonAction == ButtonAction::KEY_ACTION_RELEASE)
			{
				actionValue_ = false;
			}
			else
			{
				actionValue_ = true;
			}

			if (mouseButtonEvent.mouseButton == mouseButton1_)
			{
				mouseButton1Down_ = actionValue_;
			}
		}
		else if (event.GetEventType() == EventType::EVENT_TYPE_CURSOR_POS)
		{
			CursorPosEvent cursorPosEvent = dynamic_cast<CursorPosEvent&>(event);

			cursorPositionX_ = cursorPosEvent.xPos;
			cursorPositionY_ = cursorPosEvent.yPos;

			if (mouseButton1Down_)
			{
				float xoffset = cursorPositionX_ - lastCursorPositionX_;
				float yoffset = lastCursorPositionY_ - cursorPositionY_; // reversed since y-coordinates go from bottom to top

				float sensitivity = 0.005f;
				xoffset *= sensitivity;
				yoffset *= sensitivity;

				float cameraYaw_ = xoffset;
				float cameraPitch_ = yoffset;

				glm::vec3 cameraUp = s_worldUp_ * cameraOrientation_;
				glm::vec3 cameraRight = s_worldRight_ * cameraOrientation_;

				cameraOrientation_ = glm::rotate(cameraOrientation_, cameraYaw_, cameraUp);
				cameraOrientation_ = glm::rotate(cameraOrientation_, -cameraPitch_, cameraRight);
			}
			lastCursorPositionX_ = cursorPositionX_;
			lastCursorPositionY_ = cursorPositionY_;
		}
	}

	void RendererTestWidget::OnUpdate(double frameTimestep)
	{
		glm::mat3 rotationMatrix = glm::mat3_cast(cameraOrientation_);

		glm::vec3 directionRight = s_worldRight_ * cameraOrientation_;
		glm::vec3 directionUp = s_worldUp_ * cameraOrientation_;
		glm::vec3 directionForward = s_worldForward_ * cameraOrientation_;

		// glm::mat3 directionMatrix = glm::transpose(rotationMatrix);
		// glm::vec3 directionRight = directionMatrix[0]; // right
		// glm::vec3 directionUp = directionMatrix[1]; // up
		// glm::vec3 directionForward = directionMatrix[2] * -1.0f; // forward
		
		float cameraSpeed = 2.5f * (float)frameTimestep;

		if (movingForward_)
		{
			cameraPosition_ += cameraSpeed * directionForward;
		}
		if (movingBack_)
		{
			cameraPosition_ -= cameraSpeed * directionForward;
		}

		if (movingLeft_)
		{
			cameraPosition_ -= cameraSpeed * directionRight;
		}

		if (movingRight_)
		{
			cameraPosition_ += cameraSpeed * directionRight;
		}

		if (movingUp_)
		{
			cameraPosition_ += cameraSpeed * directionUp;
		}

		if (movingDown_)
		{
			cameraPosition_ -= cameraSpeed * directionUp;
		}
		
		glm::mat4 viewMatrix = glm::mat4(rotationMatrix);
		viewMatrix = glm::translate(viewMatrix, cameraPosition_ * -1.0f);

		glm::mat4 projectionMatrix = glm::perspective(glm::radians(fieldOfVisionDegrees_), aspectRatio_, nearCutOff_, farCutOff_);

		applicationPtr_->rendererPtr_->SetViewMatrix(viewMatrix);
		applicationPtr_->rendererPtr_->SetProjectionMatrix(projectionMatrix);
		applicationPtr_->rendererPtr_->Render();
	}
	// protected: --------------------------------------------------------------

	glm::vec3 RendererTestWidget::s_worldForward_(0.0f, 0.0f, -1.0f); // used for Roll "right hand rule"
	glm::vec3 RendererTestWidget::s_worldUp_(0.0f, 1.0f, 0.0f); // used for Yaw "right hand rule"
	glm::vec3 RendererTestWidget::s_worldRight_(1.0f, 0.0f, 0.0f); // used for Pitch "right hand rule"
}