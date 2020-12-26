
#include "RendererTestWidget.h"

#include "glm/gtc/constants.hpp" // need for pi

#include "Engine/Application.h"
#include "Engine/Logger.h"
#include "Engine/Renderer.h"
#include "Engine/Stores.h"
#include "Engine/Window.h"



namespace Project001
{
	// public ------------------------------------------------------------------
	
	RendererTestWidget::RendererTestWidget()
		: applicationPtr_(nullptr)
		, windowPtr_(nullptr)
		, storesPtr_(nullptr)
		, rendererPtr_(nullptr)
		, widgetContainerPtr_(nullptr)
		, cameraPosition_(glm::vec3(0.0f, 0.0f, 2.0f))
		, cameraOrientation_(1.0f, 0.0f, 0.0f, 0.0f)
		, fieldOfVisionDegrees_(45.0f)
		, aspectRatio_(1.0f)
		, nearCutOff_(0.1f)
		, farCutOff_(10.0f)
		, moveLeftKeyCode_(KeyCode::KEY_CODE_A)
		, moveRightKeyCode_(KeyCode::KEY_CODE_D)
		, moveUpKeyCode_(KeyCode::KEY_CODE_W)
		, moveDownKeyCode_(KeyCode::KEY_CODE_S)
		, rollLeftKeyCode_(KeyCode::KEY_CODE_Q)
		, rollRightKeyCode_(KeyCode::KEY_CODE_E)
		, movingLeft_(false)
		, movingRight_(false)
		, movingUp_(false)
		, movingDown_(false)
		, rollingLeft_(false)
		, rollingRight_(false)
		, mouseButton1_(MouseButton::MOUSE_BUTTON_RIGHT)
		, mouseButton2_(MouseButton::MOUSE_BUTTON_LEFT)
		, mouseButton1Down_(false)
		, mouseButton2Down_(false)
		, lastCursorPositionX_(0.0f)
		, lastCursorPositionY_(0.0f)
		, cursorPositionX_(0.0f)
		, cursorPositionY_(0.0f)
		, testValue00_(0.99f)
		, testValue01_(0.97f)
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

		windowPtr_->SetAspectRatio(applicationPtr->windowWidth_, applicationPtr->windowHeight_);
		aspectRatio_ = (float)applicationPtr->windowWidth_ / (float)applicationPtr->windowHeight_;

		storesPtr_->LoadOBJFile("Cube", "../Models/Cube.obj");
		//rendererPtr_->AddMesh(storesPtr_->GetMesh("Cube"));

		storesPtr_->LoadTextureFile("Cube", "../Textures/Cube.png");
		rendererPtr_->AddTexture(storesPtr_->GetTexture("Cube"), 0);

		storesPtr_->LoadTextureFile("CounterclockwiseDie", "../Textures/CounterclockwiseDie.png");
		rendererPtr_->AddTexture(storesPtr_->GetTexture("CounterclockwiseDie"), 1);

		storesPtr_->LoadTextureFile("RubiksCube", "../Textures/RubiksCube.png");
		rendererPtr_->AddTexture(storesPtr_->GetTexture("RubiksCube"), 2);

		storesPtr_->LoadTextureFile("NumberDie", "../Textures/NumberDie.png");
		rendererPtr_->AddTexture(storesPtr_->GetTexture("NumberDie"), 3);

		ModelData testModel;
		testModel.meshDataPtr_ = storesPtr_->GetMesh("Cube");
		testModel.color_ = glm::vec4(1.0f, 0.5f, 0.5f, 1.0f);
		testModel.textureIndex_ = -1.0f;
		testModel.specularIndex_ = -1.0f;
		testModel.shininess_ = 32.0f;
		testModel.position_ = glm::vec3(0.0f, 0.0f, 0.0f);
		testModel.orientation_ = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		rendererPtr_->AddModel(&testModel);

		testModel.color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		testModel.textureIndex_ = 0.0f;
		testModel.position_ = glm::vec3(0.0f, 1.0f, 0.0f);
		rendererPtr_->AddModel(&testModel);

		testModel.textureIndex_ = 1.0f;
		testModel.position_ = glm::vec3(1.0f, 0.0f, 0.0f);
		testModel.orientation_ = glm::rotate(testModel.orientation_, glm::pi<float>() / 4.0f, s_worldUp_);
		rendererPtr_->AddModel(&testModel);

		testModel.textureIndex_ = 2.0f;
		testModel.position_ = glm::vec3(0.0f, -1.0f, 0.0f);
		testModel.orientation_ = glm::rotate(testModel.orientation_, glm::pi<float>() / 4.0f, s_worldRight_);
		rendererPtr_->AddModel(&testModel);
		
		testModel.textureIndex_ = 3.0f;
		testModel.position_ = glm::vec3(-1.0f, 0.0f, 0.0f);
		testModel.orientation_ = glm::rotate(testModel.orientation_, glm::pi<float>() / 4.0f, s_worldForward_);
		rendererPtr_->AddModel(&testModel);

		// DirectionalLight directionalLight = DirectionalLight();
		// directionalLight.direction = glm::vec3(0.0f, 0.0f, 0.0f);
		// directionalLight.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
		// directionalLight.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		// directionalLight.specular = glm::vec3(0.25f, 0.25f, 0.25f);
		// rendererPtr_->SetDirectionalLight(directionalLight);

		// PointLight pointLight00 = PointLight();
		// pointLight00.position = glm::vec3(1.0f, 0.0f, 1.0f);
		// pointLight00.constant = 1.0f;
		// pointLight00.linear = 0.1f;
		// pointLight00.quadratic = 0.0f;
		// pointLight00.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
		// pointLight00.diffuse = glm::vec3(0.25f, 0.25f, 0.25f);
		// pointLight00.specular = glm::vec3(0.5f, 0.0f, 0.0f);
		// rendererPtr_->SetPointLight(pointLight00, 0);

		// PointLight pointLight01 = PointLight();
		// pointLight01.position = glm::vec3(-2.0f, 0.0f, -2.0f);
		// pointLight01.constant = 1.0f;
		// pointLight01.linear = 0.1f;
		// pointLight01.quadratic = 0.0f;
		// pointLight01.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
		// pointLight01.diffuse = glm::vec3(0.25f, 0.25f, 0.25f);
		// pointLight01.specular = glm::vec3(0.0f, 0.5f, 0.0f);
		// rendererPtr_->SetPointLight(pointLight01, 1);

		SpotLight spotLight00;
		spotLight00.position = glm::vec3(0.0f, 0.0f, 5.0f);
		spotLight00.direction = glm::vec3(0.0f, 0.0f, -1.0f);
		spotLight00.cutOff = 0.99f;
		spotLight00.outerCutOff = 0.97f;

		spotLight00.constant = 1.0f;
		spotLight00.linear = 0.0f;
		spotLight00.quadratic = 0.0f;

		spotLight00.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
		spotLight00.diffuse = glm::vec3(0.25f, 0.25f, 0.25f);
		spotLight00.specular = glm::vec3(0.5f, 0.5f, 0.5f);

		rendererPtr_->SetSpotLight(spotLight00, 0);
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

			if (keyEvent.keyCode == rollLeftKeyCode_)
			{
				rollingLeft_ = actionValue_;
			}
			else if (keyEvent.keyCode == moveLeftKeyCode_)
			{
				movingLeft_ = actionValue_;
			}
			else if (keyEvent.keyCode == moveRightKeyCode_)
			{
				movingRight_ = actionValue_;
			}
			else if (keyEvent.keyCode == rollRightKeyCode_)
			{
				rollingRight_ = actionValue_;
			}
			else if (keyEvent.keyCode == moveUpKeyCode_)
			{
				movingUp_ = actionValue_;
			}
			else if (keyEvent.keyCode == moveDownKeyCode_)
			{
				movingDown_ = actionValue_;
			}
			else if (keyEvent.keyCode == KeyCode::KEY_CODE_1 && keyEvent.buttonAction == ButtonAction::KEY_ACTION_RELEASE)
			{
				testValue00_ += 0.01f;
				OutputTestValues();
			}
			else if (keyEvent.keyCode == KeyCode::KEY_CODE_2 && keyEvent.buttonAction == ButtonAction::KEY_ACTION_RELEASE)
			{
				testValue00_ -= 0.01f;
				OutputTestValues();
			}
			else if (keyEvent.keyCode == KeyCode::KEY_CODE_3 && keyEvent.buttonAction == ButtonAction::KEY_ACTION_RELEASE)
			{
				testValue01_ += 0.01f;
				OutputTestValues();
			}
			else if (keyEvent.keyCode == KeyCode::KEY_CODE_4 && keyEvent.buttonAction == ButtonAction::KEY_ACTION_RELEASE)
			{
				testValue01_ -= 0.01f;
				OutputTestValues();
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
			else if (mouseButtonEvent.mouseButton == mouseButton2_)
			{
				mouseButton2Down_ = actionValue_;
			}
		}
		else if (event.GetEventType() == EventType::EVENT_TYPE_CURSOR_POS)
		{
			CursorPosEvent cursorPosEvent = dynamic_cast<CursorPosEvent&>(event);

			cursorPositionX_ = cursorPosEvent.xPos;
			cursorPositionY_ = cursorPosEvent.yPos;

			if (mouseButton1Down_)
			{
				float xOffset = cursorPositionX_ - lastCursorPositionX_;
				float yOffset = lastCursorPositionY_ - cursorPositionY_; // reversed since y-coordinates go from bottom to top

				float sensitivity = 0.005f;
				xOffset *= sensitivity;
				yOffset *= sensitivity;

				float cameraYaw = xOffset;
				float cameraPitch = yOffset;

				glm::vec3 cameraUp = s_worldUp_ * cameraOrientation_;
				glm::vec3 cameraRight = s_worldRight_ * cameraOrientation_;

				cameraOrientation_ = glm::rotate(cameraOrientation_, cameraYaw, cameraUp);
				cameraOrientation_ = glm::rotate(cameraOrientation_, -1.0f * cameraPitch, cameraRight);
			}

			lastCursorPositionX_ = cursorPositionX_;
			lastCursorPositionY_ = cursorPositionY_;
		}
		else if (event.GetEventType() == EventType::EVENT_TYPE_SCROLL)
		{
			ScrollEvent scrollEvent = dynamic_cast<ScrollEvent&>(event);

			float xOffset = scrollEvent.xOffset;
			float yOffset = scrollEvent.yOffset;
				
			float sensitivity = 0.25f;
			xOffset *= sensitivity;
			yOffset *= sensitivity;

			glm::vec3 cameraForward = s_worldForward_ * cameraOrientation_;

			cameraPosition_ += yOffset * cameraForward;
		}
		else if (event.GetEventType() == EventType::EVENT_TYPE_FRAMEBUFFER_SIZE)
		{
			FrameBufferSizeEvent frameBufferSizeEvent = dynamic_cast<FrameBufferSizeEvent&>(event);
			
			int height = frameBufferSizeEvent.height;
			int width = frameBufferSizeEvent.width;
			
			int adjustedHeight = (int)(width / aspectRatio_);
			int adjustedWidth = (int)(height * aspectRatio_);

			if (adjustedWidth > width)
			{
				adjustedWidth = width;
			}

			if (adjustedHeight > height)
			{
				adjustedHeight = height;
			}

			int lowerLeftX = (width - adjustedWidth) / 2;
			int lowerLeftY = (height - adjustedHeight) / 2;
			
			rendererPtr_->SetViewportSize(lowerLeftX, lowerLeftY, adjustedWidth, adjustedHeight);
		}
	}

	void RendererTestWidget::OnUpdate(double frameTimestep)
	{
		glm::mat3 rotationMatrix = glm::mat3_cast(cameraOrientation_);

		glm::vec3 directionRight = s_worldRight_ * cameraOrientation_;
		glm::vec3 directionUp = s_worldUp_ * cameraOrientation_;
		glm::vec3 directionForward = s_worldForward_ * cameraOrientation_;
		
		float cameraTranslationSpeed = 2.5f * (float)frameTimestep;
		float cameraRotationSpeed = 2.5f * (float)frameTimestep;

		if (movingLeft_)
		{
			cameraPosition_ -= cameraTranslationSpeed * directionRight;
		}

		if (movingRight_)
		{
			cameraPosition_ += cameraTranslationSpeed * directionRight;
		}

		if (movingUp_)
		{
			cameraPosition_ += cameraTranslationSpeed * directionUp;
		}

		if (movingDown_)
		{
			cameraPosition_ -= cameraTranslationSpeed * directionUp;
		}

		if (rollingLeft_)
		{
			cameraOrientation_ = glm::rotate(cameraOrientation_, cameraRotationSpeed, directionForward);
		}
		if (rollingRight_)
		{
			cameraOrientation_ = glm::rotate(cameraOrientation_, -1.0f * cameraRotationSpeed, directionForward);
		}
		
		glm::mat4 viewMatrix = glm::mat4(rotationMatrix);
		viewMatrix = glm::translate(viewMatrix, cameraPosition_ * -1.0f);

		glm::mat4 projectionMatrix = glm::perspective(glm::radians(fieldOfVisionDegrees_), aspectRatio_, nearCutOff_, farCutOff_);
		//glm::mat4 projectionMatrix = glm::ortho(-5.f, 5.f, -5.f, 5.f, -5.f, 5.f);

		SpotLight spotLight01;
		spotLight01.position = cameraPosition_;
		spotLight01.direction = directionForward;
		spotLight01.cutOff = testValue00_;
		spotLight01.outerCutOff = testValue01_;

		spotLight01.constant = 1.0f;
		spotLight01.linear = 0.0f;
		spotLight01.quadratic = 0.0f;

		spotLight01.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
		spotLight01.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		spotLight01.specular = glm::vec3(0.5f, 0.5f, 0.5f);

		rendererPtr_->SetSpotLight(spotLight01, 1);

		applicationPtr_->rendererPtr_->SetViewMatrix(viewMatrix);
		applicationPtr_->rendererPtr_->SetViewPosition(cameraPosition_);
		applicationPtr_->rendererPtr_->SetProjectionMatrix(projectionMatrix);
	}

	// protected: --------------------------------------------------------------

	void RendererTestWidget::OutputTestValues()
	{
		Logger::Message("testValue00_ = %f", testValue00_);
		Logger::Message("testValue01_ = %f", testValue01_);
		Logger::Message("--------------------------------------------------------------------------------");
	}

	// NOTE:
	// positive x is to the right, positive y is up, and positive z is out of the screen

	glm::vec3 RendererTestWidget::s_worldForward_(0.0f, 0.0f, -1.0f); // used for Roll "right hand rule"
	glm::vec3 RendererTestWidget::s_worldUp_(0.0f, 1.0f, 0.0f); // used for Yaw "right hand rule"
	glm::vec3 RendererTestWidget::s_worldRight_(1.0f, 0.0f, 0.0f); // used for Pitch "right hand rule"
}