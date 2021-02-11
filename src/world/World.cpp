#include "World.h"

World::World() {}

World::World(Texture_Loader* textureLoader) {
	blockConsts = new Block_Consts(textureLoader->getDiffAndSpecTextures("blocks"));

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			addChunk(i, j);
		}
	}

	for (int i = 0; i < chunks.size(); i++) {
		for (int j = 0; j < chunks[i].size(); j++) {
			generateChunk(chunks[i][j]);
		}
	}

	for (int i = 0; i < chunks.size(); i++) {
		for (int j = 0; j < chunks[i].size(); j++) {
			chunks[i][j].doUpdate((i != chunks.size() - 1) ? &chunks[i + 1][j] : nullptr,
				(i != 0) ? &chunks[i - 1][j] : nullptr, (j != chunks[i].size() - 1) ? &chunks[i][j + 1] : nullptr,
				(j != 0) ? &chunks[i][j - 1] : nullptr);
		}
	}
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

Chunk* World::getChunk(int xPos, int zPos) {
	int x = (xPos >= 0) ? xPos / CHUNK_MAX_WIDTH : (xPos / CHUNK_MAX_WIDTH) - 1;
	int z = (zPos >= 0) ? zPos / CHUNK_MAX_WIDTH : (zPos / CHUNK_MAX_WIDTH) - 1;

	if (x >= 0 && z >= 0 && chunks.size() > x && chunks[x].size() > z) {
		return &chunks[x][z];
	}
	return nullptr;
}

Block* World::getBlock(int xPos, int yPos, int zPos) {
	Chunk* chunk = getChunk(xPos, zPos);
	if (chunk == nullptr || yPos < 0 || yPos > CHUNK_MAX_HEIGHT) {
		return nullptr;
	}
	return chunk->getBlock(xPos % CHUNK_MAX_WIDTH, yPos, zPos % CHUNK_MAX_WIDTH);
}


void World::addChunk(int xPos, int zPos) {
	if (xPos > chunks.size()) {
		std::cout << "ERROR::WORLD::CHUNK_OUT_OF_RANGE::X_POS" << std::endl;
		std::cout << "\txPos: " << xPos << " chunks size: " << chunks.size() << std::endl;
	}
	else if (xPos == chunks.size()) {
		chunks.push_back(std::vector<Chunk>(zPos));
		chunks[xPos].push_back(Chunk(blockConsts, xPos * CHUNK_MAX_WIDTH, zPos * CHUNK_MAX_WIDTH));
	}
	else if (zPos > chunks[xPos].size()) {
		std::cout << "ERROR::WORLD::CHUNK_OUT_OF_RANGE::Z_POS" << std::endl;
		std::cout << "\tzPos: " << zPos << " chunks size: " << chunks[xPos].size() << std::endl;
	}
	else if (zPos == chunks[xPos].size()) {
		chunks[xPos].push_back(Chunk(blockConsts, xPos * CHUNK_MAX_WIDTH, zPos * CHUNK_MAX_WIDTH));
	}
	else {
		std::cout << "ERROR::WORLD::CHUNK_ALREADY_EXISTS" << std::endl;
		std::cout << "\txPos: " << xPos << " zPos: " << zPos << std::endl;
	}
}

void World::generateChunk(Chunk &chunk) {
	for (int x = 0; x < CHUNK_MAX_WIDTH; x++) {
		for (int z = 0; z < CHUNK_MAX_WIDTH; z++) {
			// Get noise value (Between -1 and 1)
			float heightValue = glm::simplex(glm::vec2((chunk.getXPos() + x) / 24.0f, (chunk.getZPos() + z)/ 24.0f));

			// Make noise value between 0 and 2
			heightValue = (heightValue + 1) * 5;
			int y = roundf(heightValue);

			chunk.addBlock(glm::vec3(x, y, z), GRASS);
			for (int i = 0; i < y; i++) {
				chunk.addBlock(glm::vec3(x, i, z), DIRT);
			}
		}
	}
}