#pragma once

#include <vector>
#include <map>

#include "glm/glm.hpp"

#include "block/Block_Consts.h"
#include "Chunk_Consts.h"
#include "../Chunk_Mesh.h"
#include "block/Block.h"
#include "../../Textures/Texture_Loader.h"

class Chunk {
private:
	std::vector<std::vector<std::vector<Block>>> blocks;
	Chunk_Mesh chunkMesh;
	Block_Consts* blockConsts;
	int xPos, zPos;
	bool render;

public:
	Chunk();
	Chunk(Block_Consts* blockConsts, int xPos, int zPos);
	~Chunk();

	void doUpdate(Chunk* chunkXPOS, Chunk* chunkXNEG, Chunk* chunkZPOS, Chunk* chunkZNEG);
	void doRender(Shader shader, GLuint modelLoc);
	bool shouldRender();

	void addBlock(glm::vec3 relPos, Block_Type type);
	Block* getBlock(int x, int y, int z);

	int getXPos();
	int getZPos();
};
