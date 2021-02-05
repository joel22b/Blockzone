#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "world/World.h"
#include "../Textures/Texture_Loader.h"

class Game {
private:
	//Camera camera;
	Texture_Loader* textureLoader;
	World world;

	void loadTextures();

public:
	Game();
	~Game();

	void doInput();
	void doUpdate();
	void doRender(Shader shader, GLint modelLoc);
};

