///
////////////////////////////////////////////////////////////////////////////////

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/matrix_decompose.hpp"

#include <iostream>

#include "Shaders.h"
#include "Vertices.h"



int learnOpenGLMain(int argc, char** argv);

void processInput(GLFWwindow* window);

void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

GLuint createShader(GLenum shaderType, const GLchar** shaderSource);
GLuint createShaderProgram(GLenum vertexShaderId, GLenum fragmentShaderId);

GLuint loadTexture(const GLchar* texturePath);



// window dimensions
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

// camera
glm::vec3 g_cameraPos = glm::vec3(0.0f, 0.0f, 8.0f);
glm::vec3 g_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 g_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool g_mouseControlledCamera = false;
float g_yaw = -90.0f;   // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float g_pitch = 0.0f;
float g_lastX = 800.0f / 2.0;
float g_lastY = 600.0 / 2.0;
float g_fov = 45.0f;

// timing
float g_deltaTime = 0.0f; // time between current frame and last frame
float g_lastFrame = 0.0f;


// =============================================================================
int learnOpenGLMain(int argc, char** argv)
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

    GLuint vertexShaderId02 = createShader(GL_VERTEX_SHADER, &g_vertexShaderSource02);
    GLuint fragmentShaderId02 = createShader(GL_FRAGMENT_SHADER, &g_fragmentShaderSource02);

    GLuint vertexShaderId03 = createShader(GL_VERTEX_SHADER, &g_vertexShaderSource03);
    GLuint fragmentShaderId03 = createShader(GL_FRAGMENT_SHADER, &g_fragmentShaderSource03);

    GLuint vertexShaderId04 = createShader(GL_VERTEX_SHADER, &g_vertexShaderSource04);
    GLuint fragmentShaderId04 = createShader(GL_FRAGMENT_SHADER, &g_fragmentShaderSource04);

    // create programs to attach and link the shaders
    GLuint shaderProgramId01 = createShaderProgram(vertexShaderId01, fragmentShaderId01);
    GLuint shaderProgramId02 = createShaderProgram(vertexShaderId02, fragmentShaderId02);
    GLuint shaderProgramId03 = createShaderProgram(vertexShaderId03, fragmentShaderId03);
    GLuint shaderProgramId04 = createShaderProgram(vertexShaderId04, fragmentShaderId04);

    // flag the shaders for deletion
    glDeleteShader(vertexShaderId01);
    glDeleteShader(fragmentShaderId01);

    glDeleteShader(vertexShaderId02);
    glDeleteShader(fragmentShaderId02);

    glDeleteShader(vertexShaderId03);
    glDeleteShader(fragmentShaderId03);

    glDeleteShader(vertexShaderId04);
    glDeleteShader(fragmentShaderId04);



    // create the vertex buffers and vertex array objects
    // -------------------------------------------------------------------------
    GLuint vertexArrayIds[4];
    glGenVertexArrays(4, vertexArrayIds);

    GLuint vertexBufferIds[4];
    glGenBuffers(4, vertexBufferIds);

    // bind an array buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferIds[0]);

    // bind the vertex array object
    glBindVertexArray(vertexArrayIds[0]);

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

    // similar steps for other buffer ------------------------------------------
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferIds[1]);
    glBindVertexArray(vertexArrayIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertices02), g_vertices02, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // and another one ---------------------------------------------------------
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferIds[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertices03), g_vertices03, GL_STATIC_DRAW);
    glBindVertexArray(vertexArrayIds[2]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 4th buffer --------------------------------------------------------------
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferIds[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertices04), g_vertices04, GL_STATIC_DRAW);
    glBindVertexArray(vertexArrayIds[3]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // unbind the array buffer and the vertex array
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);



    // load and create textures
    // -------------------------------------------------------------------------
    // tell stb_image.h to flip loaded texture's on the y-axis.
    stbi_set_flip_vertically_on_load(true);

    GLuint texture1 = loadTexture("../Textures/thonkCube.png");

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLuint diffuseMap = loadTexture("../Textures/container2.png");

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLuint specularMap = loadTexture("../Textures/container2_specular.png");

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    // other stuff
    // -------------------------------------------------------------------------
    // create the transformations
    glm::mat4 model01 = glm::mat4(1.0f);
    glm::mat4 model02 = glm::mat4(1.0f);
    glm::mat4 model03 = glm::mat4(1.0f);
    glm::mat4 model04 = glm::mat4(1.0f);

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // retrieve the matrix uniform locations
    GLint shaderProgram01modelLoc = glGetUniformLocation(shaderProgramId01, "model");
    GLint shaderProgram01viewLoc = glGetUniformLocation(shaderProgramId01, "view");
    GLint shaderProgram01projectionLoc = glGetUniformLocation(shaderProgramId01, "projection");

    GLint shaderProgram02modelLoc = glGetUniformLocation(shaderProgramId02, "model");
    GLint shaderProgram02viewLoc = glGetUniformLocation(shaderProgramId02, "view");
    GLint shaderProgram02projectionLoc = glGetUniformLocation(shaderProgramId02, "projection");

    GLint shaderProgram02viewPosLoc = glGetUniformLocation(shaderProgramId02, "viewPos");

    GLint shaderProgram02materialAmbientLoc = glGetUniformLocation(shaderProgramId02, "material.ambient");
    GLint shaderProgram02materialDiffuseLoc = glGetUniformLocation(shaderProgramId02, "material.diffuse");
    GLint shaderProgram02materialSpecularLoc = glGetUniformLocation(shaderProgramId02, "material.specular");
    GLint shaderProgram02materialShininessLoc = glGetUniformLocation(shaderProgramId02, "material.shininess");

    GLint shaderProgram02lightPositionLoc = glGetUniformLocation(shaderProgramId02, "light.position");
    GLint shaderProgram02lightAmbientLoc = glGetUniformLocation(shaderProgramId02, "light.ambient");
    GLint shaderProgram02lightDiffuseLoc = glGetUniformLocation(shaderProgramId02, "light.diffuse");
    GLint shaderProgram02lightSpecularLoc = glGetUniformLocation(shaderProgramId02, "light.specular");

    GLint shaderProgram03modelLoc = glGetUniformLocation(shaderProgramId03, "model");
    GLint shaderProgram03viewLoc = glGetUniformLocation(shaderProgramId03, "view");
    GLint shaderProgram03projectionLoc = glGetUniformLocation(shaderProgramId03, "projection");

    GLint shaderProgram04modelLoc = glGetUniformLocation(shaderProgramId04, "model");
    GLint shaderProgram04viewLoc = glGetUniformLocation(shaderProgramId04, "view");
    GLint shaderProgram04projectionLoc = glGetUniformLocation(shaderProgramId04, "projection");

    GLint shaderProgram04viewPosLoc = glGetUniformLocation(shaderProgramId04, "viewPos");

    GLint shaderProgram04materialDiffuseLoc = glGetUniformLocation(shaderProgramId04, "material.diffuse");
    GLint shaderProgram04materialSpecularLoc = glGetUniformLocation(shaderProgramId04, "material.specular");
    GLint shaderProgram04materialShininessLoc = glGetUniformLocation(shaderProgramId04, "material.shininess");

    GLint shaderProgram04lightPositionLoc = glGetUniformLocation(shaderProgramId04, "light.position");
    GLint shaderProgram04lightAmbientLoc = glGetUniformLocation(shaderProgramId04, "light.ambient");
    GLint shaderProgram04lightDiffuseLoc = glGetUniformLocation(shaderProgramId04, "light.diffuse");
    GLint shaderProgram04lightSpecularLoc = glGetUniformLocation(shaderProgramId04, "light.specular");

    glUseProgram(shaderProgramId01);
    // set texture uniforms (the parameter 0 referes to GL_TEXTURE0)
    glUniform1i(glGetUniformLocation(shaderProgramId01, "texture1"), 0);

    glUseProgram(shaderProgramId04);
    glUniform1i(glGetUniformLocation(shaderProgramId04, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(shaderProgramId04, "material.specular"), 1);

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

        // camera view
        view = glm::lookAt(g_cameraPos, g_cameraPos + g_cameraFront, g_cameraUp);

        // perspective projection
        projection = glm::perspective(glm::radians(g_fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ---------------------------------------------------------------------------
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        model01 = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 0.0f, -5.0f));
        model01 = glm::rotate(model01, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

        glUseProgram(shaderProgramId01);

        glUniformMatrix4fv(shaderProgram01modelLoc, 1, GL_FALSE, glm::value_ptr(model01));
        glUniformMatrix4fv(shaderProgram01viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(shaderProgram01projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(vertexArrayIds[0]);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(g_vertices01) / 8);

        // ---------------------------------------------------------------------------
        model02 = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -1.0f, -1.0f));

        model03 = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        model03 = glm::translate(model03, glm::vec3(1.0f, 1.0f, 3.0f));
        model03 = glm::scale(model03, glm::vec3(0.2f));

        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 translation;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(model03, scale, rotation, translation, skew, perspective);

        glUseProgram(shaderProgramId02);

        glUniform3f(shaderProgram02lightPositionLoc, translation.x, translation.y, translation.z);
        glUniform3f(shaderProgram02viewPosLoc, g_cameraPos.x, g_cameraPos.y, g_cameraPos.z);

        glUniform3f(shaderProgram02lightAmbientLoc, 0.1f, 0.1f, 0.1f);
        glUniform3f(shaderProgram02lightDiffuseLoc, 0.6f, 0.6f, 0.6f);
        glUniform3f(shaderProgram02lightSpecularLoc, 1.0f, 1.0f, 1.0f);
        
        glUniform3f(shaderProgram02materialAmbientLoc, 0.25725, 0.1995, 0.0745);
        glUniform3f(shaderProgram02materialDiffuseLoc, 0.76164, 0.60648, 0.22648);
        glUniform3f(shaderProgram02materialSpecularLoc, 0.638281, 0.555802, 0.366065);
        glUniform1f(shaderProgram02materialShininessLoc, 32.0f);

        glUniformMatrix4fv(shaderProgram02modelLoc, 1, GL_FALSE, glm::value_ptr(model02));
        glUniformMatrix4fv(shaderProgram02viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(shaderProgram02projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(vertexArrayIds[1]);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(g_vertices02) / 6);

        glUseProgram(shaderProgramId03);

        glUniformMatrix4fv(shaderProgram03modelLoc, 1, GL_FALSE, glm::value_ptr(model03));
        glUniformMatrix4fv(shaderProgram03viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(shaderProgram03projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(vertexArrayIds[2]);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(g_vertices03) / 3);

        // ---------------------------------------------------------------------------
        model04 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, -1.5f));

        glUseProgram(shaderProgramId04);
        glUniform3f(shaderProgram04lightPositionLoc, translation.x, translation.y, translation.z);
        glUniform3f(shaderProgram04viewPosLoc, g_cameraPos.x, g_cameraPos.y, g_cameraPos.z);

        glUniform3f(shaderProgram04lightAmbientLoc, 0.2f, 0.2f, 0.2f);
        glUniform3f(shaderProgram04lightDiffuseLoc, 0.5f, 0.5f, 0.5f);
        glUniform3f(shaderProgram04lightSpecularLoc, 1.0f, 1.0f, 1.0f);

        glUniform1f(shaderProgram04materialShininessLoc, 64.0f);

        glUniformMatrix4fv(shaderProgram04modelLoc, 1, GL_FALSE, glm::value_ptr(model04));
        glUniformMatrix4fv(shaderProgram04viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(shaderProgram04projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // render the cube
        glBindVertexArray(vertexArrayIds[3]);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(g_vertices04) / 8);

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

//
// =============================================================================
GLuint loadTexture(const GLchar* texturePath)
{
    GLuint textureId;
    glGenTextures(1, &textureId);

    int width;
    int height;
    int nrComponents;
    unsigned char* data = stbi_load(texturePath, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << texturePath << std::endl;
        stbi_image_free(data);
    }

    return textureId;
}