#include "Chunk.h"

Chunk::Chunk() {
	render = false;
}

Chunk::Chunk(int xPos, int zPos) {
	blocks = std::vector<std::vector<std::vector<Block>>>(CHUNK_MAX_WIDTH, std::vector<std::vector<Block>>(CHUNK_MAX_HEIGHT, std::vector<Block>(CHUNK_MAX_WIDTH, Block())));
	this->xPos = xPos;
	this->zPos = zPos;
	render = true;

	/*for (int i = 0; i < CHUNK_MAX_WIDTH; i++) {
		for (int j = 0; j < CHUNK_MAX_WIDTH; j++) {
			addBlock(glm::vec3(i, 0, j), DIRT);
		}
	}
	for (int i = 0; i < CHUNK_MAX_WIDTH; i++) {
		for (int j = 0; j < CHUNK_MAX_WIDTH; j++) {
			addBlock(glm::vec3(i, 1, j), GRASS);
		}
	}*/
}

Chunk::~Chunk() {

}

void Chunk::doUpdate(std::vector<Texture> blockTextures, Block_Consts* blockConsts, Chunk* chunkXPOS, Chunk* chunkXNEG, Chunk* chunkZPOS, Chunk* chunkZNEG) {
	std::vector<Block_Face> blockFaces;
	Block_Face blockFace;

	for (int x = 0; x < CHUNK_MAX_WIDTH; x++) {
		for (int y = 0; y < CHUNK_MAX_HEIGHT - 40; y++) {
			for (int z = 0; z < CHUNK_MAX_WIDTH; z++) {
				if (blocks[x][y][z].getType() == AIR) {
					// Add surrounding faces

					// XPOS
					if (x != 0) {
						if (blocks[x - 1][y][z].getType() != AIR) {
							blockFace.Position = glm::vec3(x, y + 0.5f, z + 0.5f);
							blockFace.Normal = glm::vec3(1, 0, 0);
							blockFace.TexCoords = blockConsts->getBlockTexCoords(blocks[x - 1][y][z].getType(), XPOS);
							blockFaces.push_back(blockFace);
						}
					}
					else if (chunkXNEG != nullptr) {
						if (chunkXNEG->getBlock(CHUNK_MAX_WIDTH - 1, y, z).getType() != AIR) {
							blockFace.Position = glm::vec3(x, y + 0.5f, z + 0.5f);
							blockFace.Normal = glm::vec3(1, 0, 0);
							blockFace.TexCoords = blockConsts->getBlockTexCoords(chunkXNEG->getBlock(CHUNK_MAX_WIDTH - 1, y, z).getType(), XPOS);
							blockFaces.push_back(blockFace);
						}
					}
					// XNEG
					if (x != CHUNK_MAX_WIDTH - 1) {
						if (blocks[x + 1][y][z].getType() != AIR) {
							blockFace.Position = glm::vec3(x + 1, y + 0.5f, z + 0.5f);
							blockFace.Normal = glm::vec3(-1, 0, 0);
							blockFace.TexCoords = blockConsts->getBlockTexCoords(blocks[x + 1][y][z].getType(), XNEG);
							blockFaces.push_back(blockFace);
						}
					}
					else if (chunkXPOS != nullptr) {
						if (chunkXPOS->getBlock(0, y, z).getType() != AIR) {
							blockFace.Position = glm::vec3(x + 1, y + 0.5f, z + 0.5f);
							blockFace.Normal = glm::vec3(-1, 0, 0);
							blockFace.TexCoords = blockConsts->getBlockTexCoords(chunkXPOS->getBlock(0, y, z).getType(), XNEG);
							blockFaces.push_back(blockFace);
						}
					}
					// YPOS
					if (y != 0) {
						if (blocks[x][y - 1][z].getType() != AIR) {
							blockFace.Position = glm::vec3(x + 0.5f, y, z + 0.5f);
							blockFace.Normal = glm::vec3(0, 1, 0);
							blockFace.TexCoords = blockConsts->getBlockTexCoords(blocks[x][y - 1][z].getType(), YPOS);
							blockFaces.push_back(blockFace);
						}
					}
					// YNEG
					if (y != CHUNK_MAX_HEIGHT - 1) {
						if (blocks[x][y + 1][z].getType() != AIR) {
							blockFace.Position = glm::vec3(x + 0.5f, y + 1, z + 0.5f);
							blockFace.Normal = glm::vec3(0, -1, 0);
							blockFace.TexCoords = blockConsts->getBlockTexCoords(blocks[x][y + 1][z].getType(), YNEG);
							blockFaces.push_back(blockFace);
						}
					}
					// ZPOS
					if (z != 0) {
						if (blocks[x][y][z - 1].getType() != AIR) {
							blockFace.Position = glm::vec3(x + 0.5f, y + 0.5f, z);
							blockFace.Normal = glm::vec3(0, 0, 1);
							blockFace.TexCoords = blockConsts->getBlockTexCoords(blocks[x][y][z - 1].getType(), ZPOS);
							blockFaces.push_back(blockFace);
						}
					}
					else if (chunkZNEG != nullptr) {
						if (chunkZNEG->getBlock(x, y, CHUNK_MAX_WIDTH - 1).getType() != AIR) {
							blockFace.Position = glm::vec3(x + 0.5f, y + 0.5f, z);
							blockFace.Normal = glm::vec3(0, 0, 1);
							blockFace.TexCoords = blockConsts->getBlockTexCoords(chunkZNEG->getBlock(x, y, CHUNK_MAX_WIDTH - 1).getType(), ZPOS);
							blockFaces.push_back(blockFace);
						}
					}
					// ZNEG
					if (z != CHUNK_MAX_WIDTH - 1) {
						if (blocks[x][y][z + 1].getType() != AIR) {
							blockFace.Position = glm::vec3(x + 0.5f, y + 0.5f, z + 1);
							blockFace.Normal = glm::vec3(0, 0, -1);
							blockFace.TexCoords = blockConsts->getBlockTexCoords(blocks[x][y][z + 1].getType(), ZNEG);
							blockFaces.push_back(blockFace);
						}
					}
					else if (chunkZPOS != nullptr) {
						if (chunkZPOS->getBlock(x, y, 0).getType() != AIR) {
							blockFace.Position = glm::vec3(x + 0.5f, y + 0.5f, z + 1);
							blockFace.Normal = glm::vec3(0, 0, -1);
							blockFace.TexCoords = blockConsts->getBlockTexCoords(chunkZPOS->getBlock(x, y, 0).getType(), ZNEG);
							blockFaces.push_back(blockFace);
						}
					}
				}
			}
		}
	}

	chunkMesh = Chunk_Mesh(blockFaces, blockTextures);
}

void Chunk::doRender(Shader shader, GLuint modelLoc) {
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glm::translate(glm::mat4(1), glm::vec3(xPos, 0, zPos))));
	chunkMesh.doRender(shader);
}

bool Chunk::shouldRender() {
	return this->render;
}

void Chunk::addBlock(glm::vec3 relPos, Block_Type type) {
	blocks[(int)relPos.x][(int)relPos.y][(int)relPos.z] = Block(type);
}

Block Chunk::getBlock(int x, int y, int z) {
	return blocks[x][y][z];
}

int Chunk::getXPos() {
	return xPos;
}

int Chunk::getZPos() {
	return zPos;
}