#pragma once

#include <vector>
#include <map>
#include <string>
#include <fstream>

#include <sys/stat.h>

#include "glm/glm.hpp"
#include "glm/gtc/noise.hpp"

#include "block/Block_Consts.h"
#include "Chunk_Consts.h"
#include "../Chunk_Mesh.h"
#include "block/Block.h"
#include "../../Textures/Texture_Loader.h"

class Chunk {
private:
	std::vector<std::vector<std::vector<Block>>> blocks;
	Chunk_Mesh* chunkMesh = nullptr;
	Block_Consts* blockConsts;
	int xPos, zPos;
	bool render, toDelete;

	std::vector<Block_Face> calculateMesh(Chunk* chunkXPOS, Chunk* chunkXNEG, Chunk* chunkZPOS, Chunk* chunkZNEG);

public:
	Chunk();
	Chunk(Block_Consts* blockConsts, int xPos, int zPos);
	~Chunk();

	Chunk_Mesh* getChunkMesh();

	void doUpdate(Chunk* chunkXPOS, Chunk* chunkXNEG, Chunk* chunkZPOS, Chunk* chunkZNEG);
	void doPartialUpdate(Chunk* chunkXPOS, Chunk* chunkXNEG, Chunk* chunkZPOS, Chunk* chunkZNEG);

	void doRender(Shader shader, GLuint modelLoc);
	bool shouldRender();
	void setRender(bool render);

	void addBlock(glm::vec3 relPos, Block_Type type);
	void addBlock(int x, int y, int z, Block_Type type);
	Block* getBlock(int x, int y, int z);

	int getXPos();
	int getZPos();

	int getChunkXPos();
	int getChunkZPos();

	bool shouldDelete();
	void setDelete();
};

