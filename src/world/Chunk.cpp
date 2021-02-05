#include "Chunk.h"

Chunk::Chunk(glm::vec3 position) {
	blocks = std::vector<std::vector<std::vector<Block>>>(CHUNK_MAX_WIDTH, std::vector<std::vector<Block>>(CHUNK_MAX_HEIGHT, std::vector<Block>(CHUNK_MAX_WIDTH, Block())));
	this->position = position;
}

Chunk::~Chunk() {

}

void Chunk::doRender(Shader shader, GLuint modelLoc) {
	for (int i = 0; i < blocks.size(); i++) {
		for (int j = 0; j < blocks[i].size(); j++) {
			for (int k = 0; k < blocks[i][j].size(); k++) {
				if (blocks[i][j][k].getUpdate()) {
					blocks[i][j][k].doRender(shader, modelLoc, position + glm::vec3(i, j, k));
				}
			}
		}
	}
}


void Chunk::addBlock(glm::vec3 relPos, Block_Type type, Block_Consts* blockConsts) {
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