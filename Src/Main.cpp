///
////////////////////////////////////////////////////////////////////////////////

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp""

#include <iostream>

#include "Shaders.h"
#include "Vertices.h"



void processInput(GLFWwindow* window);

void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

GLuint createShader(GLenum shaderType, const GLchar** shaderSource);
GLuint createShaderProgram(GLenum vertexShaderId, GLenum fragmentShaderId);



// window dimensions
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

// camera
glm::vec3 g_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 g_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 g_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool g_mouseControlledCamera = false;
float g_yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float g_pitch = 0.0f;
float g_lastX = 800.0f / 2.0;
float g_lastY = 600.0 / 2.0;
float g_fov = 45.0f;

// timing
float g_deltaTime = 0.0f; // time between current frame and last frame
float g_lastFrame = 0.0f;

// lighting
glm::vec3 g_lightPos(1.2f, 1.0f, 2.0f);

// =============================================================================
int main(int argc, char** argv)
{
    // initialize and configure glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 4
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 6
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // maybe one day i'll test on apple
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif



    // create glfw window
    // -------------------------------------------------------------------------
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Project001", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "ERROR::Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // Need to do this to implement mouse motion based camera controls???
    /// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // load all OpenGL function pointers with glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "ERROR::Failed to initialize GLAD" << std::endl;
        return -1;
    }



    // configure global opengl state
    // -------------------------------------------------------------------------
    // enable using the z buffer
    glEnable(GL_DEPTH_TEST);

    // enable writing to the depth buffer
    glDepthMask(GL_TRUE);

    // draw as wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    // build and compile a shader programs
    // -------------------------------------------------------------------------
    // create and compile the vertex shaders
    GLuint vertexShaderId01 = createShader(GL_VERTEX_SHADER, &g_vertexShaderSource01);
    GLuint fragmentShaderId01 = createShader(GL_FRAGMENT_SHADER, &g_fragmentShaderSource01);

    // create programs to attach and link the shaders
    GLuint shaderProgramId01 = createShaderProgram(vertexShaderId01, fragmentShaderId01);

    // flag the shaders for deletion
    glDeleteShader(vertexShaderId01);
    glDeleteShader(fragmentShaderId01);



    // create the vertex buffers and vertex array objectrs
    // -------------------------------------------------------------------------
    unsigned int vertexArrayIds[1];
    glGenVertexArrays(1, vertexArrayIds);

    // bind the vertex array object
    glBindVertexArray(vertexArrayIds[0]);

    unsigned int vertexBufferIds[1];
    glGenBuffers(1, vertexBufferIds);

    // bind an array buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferIds[0]);

    // copy vertex data into the bound array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertices01), g_vertices01, GL_STATIC_DRAW);

    // configure position attribute for the bound vertex array object
    // (index, size, type, normalized, stride, pointer)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // configure color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // configure texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // unbind the array buffer and the vertex array
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);



    // load and create textures
    // -------------------------------------------------------------------------
    unsigned int texture1;
    glGenTextures(1, &texture1);

    glBindTexture(GL_TEXTURE_2D, texture1);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // load image, create texture and generate mipmaps
    int width1;
    int height1;
    int nrChannels1;

    // tell stb_image.h to flip loaded texture's on the y-axis.
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data1 = stbi_load("../Textures/thonkCube.png", &width1, &height1, &nrChannels1, 0);
    if (data1)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width1, height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data1);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // set texture uniforms (the parameter 0 referes to GL_TEXTURE0)
    glUniform1i(glGetUniformLocation(shaderProgramId01, "texture1"), 0);



    // other stuff
    // -------------------------------------------------------------------------
    // create the transformations
    glm::mat4 model01 = glm::mat4(1.0f);

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // retrieve the matrix uniform locations
    unsigned int shaderProgram01modelLoc = glGetUniformLocation(shaderProgramId01, "model");
    unsigned int shaderProgram01viewLoc = glGetUniformLocation(shaderProgramId01, "view");
    unsigned int shaderProgram01projectionLoc = glGetUniformLocation(shaderProgramId01, "projection");



    // render loop
    // =========================================================================
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        float currentFrame = glfwGetTime();
        g_deltaTime = currentFrame - g_lastFrame;
        g_lastFrame = currentFrame;

        // input
        processInput(window);

        // render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        // spinning model
        model01 = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

        // camera view
        view = glm::lookAt(g_cameraPos, g_cameraPos + g_cameraFront, g_cameraUp);

        // perspective projection
        projection = glm::perspective(glm::radians(g_fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

        // orthographic projection
        /// view = glm::lookAt(glm::vec3(0, 0, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        /// projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 100.0f);

        // set unifroms
        glUniformMatrix4fv(shaderProgram01modelLoc, 1, GL_FALSE, glm::value_ptr(model01));
        glUniformMatrix4fv(shaderProgram01viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(shaderProgram01projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // set the shader program
        glUseProgram(shaderProgramId01);

        // bind the VAO and VBO being rendered with
        glBindVertexArray(vertexArrayIds[0]);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferIds[0]);

        // render
        glDrawArrays(GL_TRIANGLES, 0, sizeof(g_vertices01) / 8);

        // swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-allocate resources
    glDeleteVertexArrays(1, vertexArrayIds);
    glDeleteBuffers(1, vertexBufferIds);

    // terminate, clear all previously allocated glfw resources
    glfwTerminate();
    return 0;
}

// =============================================================================
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5 * g_deltaTime;
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        g_cameraPos += cameraSpeed * g_cameraFront;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        g_cameraPos -= cameraSpeed * g_cameraFront;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        g_cameraPos -= glm::normalize(glm::cross(g_cameraFront, g_cameraUp)) * cameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        g_cameraPos += glm::normalize(glm::cross(g_cameraFront, g_cameraUp)) * cameraSpeed;
    }
}

// glfw: whenever the mouse moves, this callback is called
// =============================================================================
void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (g_mouseControlledCamera)
    {
        float xoffset = xPos - g_lastX;
        float yoffset = g_lastY - yPos; // reversed since y-coordinates go from bottom to top
        g_lastX = xPos;
        g_lastY = yPos;

        float sensitivity = 0.1f; // variable
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        g_yaw += xoffset;
        g_pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (g_pitch > 89.0f)
        {
            g_pitch = 89.0f;
        }

        if (g_pitch < -89.0f)
        {
            g_pitch = -89.0f;
        }

        glm::vec3 front;
        front.x = cos(glm::radians(g_yaw)) * cos(glm::radians(g_pitch));
        front.y = sin(glm::radians(g_pitch));
        front.z = sin(glm::radians(g_yaw)) * cos(glm::radians(g_pitch));
        g_cameraFront = glm::normalize(front);
    }
    else
    {
        g_lastX = xPos;
        g_lastY = yPos;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// =============================================================================
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//
// =============================================================================
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        g_mouseControlledCamera = true;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        g_mouseControlledCamera = false;
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// =============================================================================
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
   if (g_fov >= 1.0f && g_fov <= 45.0f)
   {
      g_fov -= yOffset;
   }

   if (g_fov <= 1.0f)
   {
      g_fov = 1.0f;
   }

   if (g_fov >= 45.0f)
   {
      g_fov = 45.0f;
   }
}

//
// =============================================================================
GLuint createShader(GLenum shaderType, const GLchar** shaderSource)
{
    GLuint shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, shaderSource, NULL);
    glCompileShader(shaderId);
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        if (shaderType == GL_VERTEX_SHADER)
        {
            std::cout << "VERTEX";
        }
        else if (shaderType == GL_FRAGMENT_SHADER)
        {
            std::cout << "FRAG<MET";
        }
        else
        {
            std::cout << "UNKNOWN";
        }
        std::cout << "::COMPILATION_FAILED" << std::endl << infoLog << std::endl;
    }
    return shaderId;
}

//
// =============================================================================
GLuint createShaderProgram(GLenum vertexShaderId, GLenum fragmentShaderId)
{
    GLuint programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    glLinkProgram(programId);
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    return programId;
}