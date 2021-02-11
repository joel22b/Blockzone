#include <iostream>
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include "SOIL2/src/SOIL2/SOIL2.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shaders/Shader.h"
#include "src/Camera.h"
#include "src/Game.h"
#include "src/Text.h"

// Window dimensions
const GLint WIDTH = 1600, HEIGHT = 800;
int SCREEN_WIDTH, SCREEN_HEIGHT;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);

//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int nbFrames = 0;
double currentTime;
double lastTime = glfwGetTime();
std::string mspfText = "test", fpsText = "test";

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

Game* game;

Text arialText;

int main() {
    glfwInit();

    // Set OpenGL version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Creates the window (The two nullptrs are monitor and window respectively)
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Blockzone", nullptr, nullptr);

    // Adjusts for pixel density
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialise GLEW" << std::endl;

        return EXIT_FAILURE;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glEnable(GL_DEPTH_TEST);

    //glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    game = new Game(SCREEN_WIDTH, SCREEN_HEIGHT);

    arialText = Text("Freetype2/Fonts/arial.ttf", 20);

    // Main program loop
    while (!glfwWindowShouldClose(window)) {
        // Get time since last frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Get framerate and ms per frame
        currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= (double)1.0) {
            double mspf = 1000.0 / double(nbFrames);
            mspfText = "ms per frame: " + to_string(mspf);
            fpsText = "fps: " + to_string(1000.0 / mspf);
            nbFrames = 0;
            lastTime += 1.0;
        }

        glfwPollEvents();
        game->doInput(deltaTime);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        game->doUpdate();

        game->doRender(&arialText);

        arialText.RenderText(mspfText, 25.0f, 50.0f, 1.0f, glm::vec3(1, 1, 1));
        arialText.RenderText(fpsText, 25.0f, 25.0f, 1.0f, glm::vec3(1, 1, 1));
        // Drawing done above

        glfwSwapBuffers(window);
    }

    // If here, program is exiting
    delete game;

    glfwTerminate();

    return EXIT_SUCCESS;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    game->keyCallback(window, key, scancode, action, mode);
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos) {
    game->mouseCallback(window, xPos, yPos);
}