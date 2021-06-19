#pragma once

#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "../Shaders/Shader.h"
#include "world/World.h"
#include "../Textures/Texture_Loader.h"
#include "Text.h"

#include "entity/player/Player.h"

class Game {
private:
	//Camera camera;
	Shader blockShader;
	Texture_Loader* textureLoader;
	World* world;

	Player* player;

	bool keys[1024] = { false };
	GLfloat lastX;
	GLfloat lastY;
	bool firstMouse = true;

	void loadShaders(int screenWidth, int screenHeight);
	void loadTextures();

public:
	Game(int screenWidth, int screenHeight);
	~Game();

	void doInput(GLfloat deltaTime);
	void doUpdate();
	void doRender(Text* text);

	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
	void mouseCallback(GLFWwindow* window, double xPos, double yPos);

	void updateProjection(GLfloat fov, int screenWidth, int screenHeight);
};

