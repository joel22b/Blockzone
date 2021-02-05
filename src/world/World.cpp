#include "World.h"

World::World() {}

World::World(Texture_Loader* textureLoader) {
	blockConsts = new Block_Consts(textureLoader);

	addChunk(0, 0);
	addChunk(1, 0);
}

World::~World() {

}

void World::doRender(Shader shader, GLint modelLoc) {
	for (int i = 0; i < chunks.size(); i++) {
		for (int j = 0; j < chunks[i].size(); j++) {
			if (chunks[i][j].shouldRender()) {
				chunks[i][j].doRender(shader, modelLoc);
			}
		}
	}
}

void World::addChunk(int xPos, int zPos) {
	if (xPos > chunks.size()) {
		std::cout << "ERROR::WORLD::CHUNK_OUT_OF_RANGE::X_POS" << std::endl;
		std::cout << "\txPos: " << xPos << " chunks size: " << chunks.size() << std::endl;
	}
	else if (xPos == chunks.size()) {
		chunks.push_back(std::vector<Chunk>(zPos));
		chunks[xPos].push_back(Chunk(glm::vec2(xPos * CHUNK_MAX_WIDTH, zPos * CHUNK_MAX_WIDTH), blockConsts));
	}
	else {
		std::cout << "ERROR::WORLD::CHUNK_ALREADY_EXISTS" << std::endl;
		std::cout << "\txPos: " << xPos << " zPos: " << zPos << std::endl;
	}
}