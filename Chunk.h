#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "src/world/block/Block_Consts.h"
#include "src/world/Chunk_Consts.h"
#include "Block.h"
#include "Textures/Texture_Loader.h"

class Chunk {
private:
	std::vector<std::vector<std::vector<Block>>> blocks;
	glm::vec3 position;


public:
	Chunk(glm::vec3 position = glm::vec3(0, 0, 0));
	~Chunk();

	void doRender(Shader shader, GLuint modelLoc);

	void addBlock(glm::vec3 relPos, Block_Type type, Block_Consts* blockConsts);
};

