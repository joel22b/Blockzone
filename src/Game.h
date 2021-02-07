#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "../Shaders/Shader.h"
#include "world/World.h"
#include "../Textures/Texture_Loader.h"

#include "Chunk_Mesh.h"

class Game {
private:
	//Camera camera;
	Shader blockShader;
	Texture_Loader* textureLoader;
	World world;

	Chunk_Mesh chunkMesh;

	void loadShaders();
	void loadTextures();

public:
	Game();
	~Game();

	void doInput();
	void doUpdate(glm::vec3 viewPos, glm::mat4 projection, glm::mat4 view);
	void doRender();
};

