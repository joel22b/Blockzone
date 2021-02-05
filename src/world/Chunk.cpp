#include "Chunk.h"

Chunk::Chunk() {
	render = false;
}

Chunk::Chunk(glm::vec2 position, Block_Consts* blockConsts) {
	blocks = std::vector<std::vector<std::vector<Block>>>(CHUNK_MAX_WIDTH, std::vector<std::vector<Block>>(CHUNK_MAX_HEIGHT, std::vector<Block>(CHUNK_MAX_WIDTH, Block())));
	this->position = position;
	this->blockConsts = blockConsts;
	render = true;

	addBlock(glm::vec3(0, 0, 0), GRASS);
	addBlock(glm::vec3(1, 0, 0), GRASS);
	addBlock(glm::vec3(0, 0, 1), GRASS);
	addBlock(glm::vec3(1, 0, 1), DIRT);
	addBlock(glm::vec3(2, 0, 1), GRASS);
	addBlock(glm::vec3(1, 0, 2), GRASS);
	addBlock(glm::vec3(1, 1, 1), DIRT);
	addBlock(glm::vec3(1, 2, 1), GRASS);

	addBlock(glm::vec3(7, 0, 0), GRASS);
	addBlock(glm::vec3(8, 0, 0), GRASS);

	addBlock(glm::vec3(15, 0, 0), GRASS);
}

Chunk::~Chunk() {

}

void Chunk::doRender(Shader shader, GLuint modelLoc) {
	for (int i = 0; i < blocks.size(); i++) {
		for (int j = 0; j < blocks[i].size(); j++) {
			for (int k = 0; k < blocks[i][j].size(); k++) {
				if (blocks[i][j][k].getUpdate()) {
					blocks[i][j][k].doRender(shader, modelLoc, glm::vec3(position.x, 0, position.y) + glm::vec3(i, j, k));
				}
			}
		}
	}
}

bool Chunk::shouldRender() {
	return this->render;
}


void Chunk::addBlock(glm::vec3 relPos, Block_Type type) {
	blocks[(int)relPos.x][(int)relPos.y][(int)relPos.z] = Block(blockConsts, type, type != AIR, false);

	// Update render for itself and surrounding blocks
	// XPOS
	if ((int)relPos.x == CHUNK_MAX_WIDTH - 1) {
		// Check other chunk
		blocks[(int)relPos.x][(int)relPos.y][(int)relPos.z].setRender(XPOS, true);
	}
	else if (blocks[(int)relPos.x + 1][(int)relPos.y][(int)relPos.z].getType() == AIR) {
		blocks[(int)relPos.x][(int)relPos.y][(int)relPos.z].setRender(XPOS, true);
	}
	else {
		blocks[(int)relPos.x + 1][(int)relPos.y][(int)relPos.z].setRender(XNEG, false);
	}
	// XNEG
	if ((int)relPos.x == 0) {
		// Check other chunk
		blocks[(int)relPos.x][(int)relPos.y][(int)relPos.z].setRender(XNEG, true);
	}
	else if (blocks[(int)relPos.x - 1][(int)relPos.y][(int)relPos.z].getType() == AIR) {
		blocks[(int)relPos.x][(int)relPos.y][(int)relPos.z].setRender(XNEG, true);
	}
	else {
		blocks[(int)relPos.x - 1][(int)relPos.y][(int)relPos.z].setRender(XPOS, false);
	}
	// YPOS
	if ((int)relPos.y == CHUNK_MAX_HEIGHT - 1 || blocks[(int)relPos.x][(int)relPos.y + 1][(int)relPos.z].getType() == AIR) {
		blocks[(int)relPos.x][(int)relPos.y][(int)relPos.z].setRender(YPOS, true);
	}
	else {
		blocks[(int)relPos.x][(int)relPos.y + 1][(int)relPos.z].setRender(YNEG, false);
	}
	// YNEG
	if ((int)relPos.y == 0 || blocks[(int)relPos.x][(int)relPos.y - 1][(int)relPos.z].getType() == AIR) {
		blocks[(int)relPos.x][(int)relPos.y][(int)relPos.z].setRender(YNEG, true);
	}
	else {
		blocks[(int)relPos.x ][(int)relPos.y - 1][(int)relPos.z].setRender(YPOS, false);
	}
	// ZPOS
	if ((int)relPos.z == CHUNK_MAX_WIDTH - 1) {
		// Check other chunk
		blocks[(int)relPos.x][(int)relPos.y][(int)relPos.z].setRender(ZPOS, true);
	}
	else if (blocks[(int)relPos.x][(int)relPos.y][(int)relPos.z + 1].getType() == AIR) {
		blocks[(int)relPos.x][(int)relPos.y][(int)relPos.z].setRender(ZPOS, true);
	}
	else {
		blocks[(int)relPos.x][(int)relPos.y][(int)relPos.z + 1].setRender(ZNEG, false);
	}
	// ZNEG
	if ((int)relPos.z == 0) {
		// Check other chunk
		blocks[(int)relPos.x][(int)relPos.y][(int)relPos.z].setRender(ZNEG, true);
	}
	else if (blocks[(int)relPos.x][(int)relPos.y][(int)relPos.z - 1].getType() == AIR) {
		blocks[(int)relPos.x][(int)relPos.y][(int)relPos.z].setRender(ZNEG, true);
	}
	else {
		blocks[(int)relPos.x][(int)relPos.y][(int)relPos.z - 1].setRender(ZPOS, false);
	}
}