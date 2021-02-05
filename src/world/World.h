#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "Chunk.h"
#include "Chunk_Consts.h"
#include "block/Block_Consts.h"
#include "../../Textures/Texture_Loader.h"

class World {
public:
	World();
	World(Texture_Loader* textureLoader);
	~World();

	void doRender(Shader shader, GLint modelLoc);

private:
	std::vector<std::vector<Chunk>> chunks;
	Block_Consts* blockConsts;

	void addChunk(int xPos, int zPos);
};