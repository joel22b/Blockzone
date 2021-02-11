#pragma once

#include <vector>
#include <math.h>

#include "glm/glm.hpp"
#include "glm/gtc/noise.hpp"

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

	Chunk* getChunk(int xPos, int zPos);
	Block* getBlock(int xPos, int yPos, int zPos);

private:
	std::vector<std::vector<Chunk>> chunks;
	Block_Consts* blockConsts;

	void addChunk(int xPos, int zPos);

	void generateChunk(Chunk &chunk);
};