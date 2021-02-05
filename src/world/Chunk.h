#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "block/Block_Consts.h"
#include "Chunk_Consts.h"
#include "block/Block.h"
#include "../../Textures/Texture_Loader.h"

class Chunk {
private:
	std::vector<std::vector<std::vector<Block>>> blocks;
	glm::vec2 position;
	Block_Consts* blockConsts;
	bool render;

public:
	Chunk();
	Chunk(glm::vec2 position, Block_Consts* blockConsts);
	~Chunk();

	void doRender(Shader shader, GLuint modelLoc);
	bool shouldRender();

	void addBlock(glm::vec3 relPos, Block_Type type);
};

