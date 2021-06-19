#include "World.h"

World::World() {}

World::World(Texture_Loader* textureLoader) {
	blockConsts = new Block_Consts(textureLoader->getDiffAndSpecTextures("blocks"));
	chunkXOffset = 0;
	chunkZOffset = 0;
	this->chunks = nullptr;
}

World::~World() {

}

void World::doUpdate() {
	/*for (int i = 0; i < threadsShiftChunks.size(); i++) {
		if (threadsShiftChunks.at(i).joinable()) {
			delete threadsShiftChunks.at(i);
		}
	}*/
}

void World::doRender(Shader shader, GLint modelLoc) {
	for (int x = 0; x < chunksLength; x++) {
		for (int z = 0; z < chunksLength; z++) {
			if (getChunkNoOffset(x, z) != nullptr && getChunkNoOffset(x, z)->shouldRender()) {
				getChunkNoOffset(x, z)->doRender(shader, modelLoc);
			}
			else {
				//std::cout << "Cannot render x=" << x << " z=" << z << std::endl;
			}
		}
	}
}

Chunk* World::getChunk(int xPos, int zPos) {
	int x = xPos + chunkXOffset;
	int z = zPos + chunkZOffset;

	if (x < 0 || x >= chunksLength || z < 0 || z >= chunksLength) {
		return nullptr;
	}
	return chunks[(x * chunksLength) + z];
}

Chunk* World::getChunkNoOffset(int xPos, int zPos) {
	if (xPos < 0 || xPos >= chunksLength || zPos < 0 || zPos >= chunksLength) {
		return nullptr;
	}

	return chunks[(xPos * chunksLength) + zPos];
}

Chunk* World::getChunkByCoords(int xPos, int zPos) {
	int x = (xPos >= 0) ? xPos / CHUNK_MAX_WIDTH : (xPos / CHUNK_MAX_WIDTH) - 1;
	int z = (zPos >= 0) ? zPos / CHUNK_MAX_WIDTH : (zPos / CHUNK_MAX_WIDTH) - 1;
	return getChunk(x, z);
}

glm::vec2 World::getChunkCoords(int xPos, int zPos) {
	int x = (xPos >= 0) ? xPos / CHUNK_MAX_WIDTH : (xPos / CHUNK_MAX_WIDTH) - 1;
	int z = (zPos >= 0) ? zPos / CHUNK_MAX_WIDTH : (zPos / CHUNK_MAX_WIDTH) - 1;
	return glm::vec2(x, z);
}

Block* World::getBlock(int xPos, int yPos, int zPos) {
	Chunk* chunk = getChunkByCoords(xPos, zPos);
	if (chunk == nullptr || yPos < 0 || yPos > CHUNK_MAX_HEIGHT) {
		return nullptr;
	}
	return chunk->getBlock((xPos >= 0 || xPos % CHUNK_MAX_WIDTH == 0) ? xPos % CHUNK_MAX_WIDTH : (xPos % CHUNK_MAX_WIDTH) + CHUNK_MAX_WIDTH,
		yPos, (zPos >= 0 || zPos % CHUNK_MAX_WIDTH == 0) ? zPos % CHUNK_MAX_WIDTH : (zPos % CHUNK_MAX_WIDTH) + CHUNK_MAX_WIDTH);
}

std::string World::getWorldFolder() {
	return "saves\\" + worldName + "\\";
}

void World::generateChunk(Chunk* chunk) {
	for (int x = 0; x < CHUNK_MAX_WIDTH; x++) {
		for (int z = 0; z < CHUNK_MAX_WIDTH; z++) {
			// Get base value (Between -1 and 1)
			float baseValue = glm::simplex(glm::vec2((chunk->getXPos() + x) / 50.0f, (chunk->getZPos() + z) / 50.0f));
			// Make base value between 0 and 10
			baseValue = (baseValue + 1) * 5;

			// Get height value (Between -1 and 1)
			float heightValue = glm::simplex(glm::vec2((chunk->getXPos() + x) / 24.0f, (chunk->getZPos() + z)/ 24.0f));
			// Make height value between 0 and 2
			heightValue = heightValue + 1;

			int y = roundf(heightValue * baseValue);

			chunk->addBlock(glm::vec3(x, y, z), GRASS);
			for (int i = 0; i < y; i++) {
				chunk->addBlock(glm::vec3(x, i, z), DIRT);
			}
		}
	}
}

void World::loadChunk(Chunk* chunk) {
	// Check if chunk file exists
	const std::string chunkFilePath = getWorldFolder() + "chunks\\x" + std::to_string(chunk->getChunkXPos()) + "z" + std::to_string(chunk->getChunkZPos()) + ".chunk";
	if (fileExists(chunkFilePath)) {
		// Load in chunk from file
		std::string fileData;
		std::ifstream file;
		// ensure ifstream objects can throw exceptions:
		file.exceptions(std::ifstream::badbit);
		try {
			// Open files
			file.open(chunkFilePath);
			std::stringstream stream;
			// Read file's buffer contents into streams
			stream << file.rdbuf();
			// Close file handlers
			file.close();
			// Convert stream into string
			fileData = stream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::CHUNK::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
			std::cout << "\tfile: " << chunkFilePath << std::endl;
			return;
		}

		// Check that fileData is valid
		if (fileData.length() != CHUNK_MAX_WIDTH * CHUNK_MAX_WIDTH * CHUNK_MAX_HEIGHT) {
			std::cout << "ERROR::CHUNK::FILE_NOT_CORRECT_SIZE" << std::endl;
			std::cout << "\tfile: " << chunkFilePath << std::endl;
			std::cout << "\tsize: " << fileData.length() << std::endl;
			return;
		}

		// Load fileData into chunk
		int count = 0;
		for (int y = 0; y < CHUNK_MAX_HEIGHT; y++) {
			for (int z = 0; z < CHUNK_MAX_WIDTH; z++) {
				for (int x = 0; x < CHUNK_MAX_WIDTH; x++) {
					chunk->addBlock(x, y, z, Block_Type(int(fileData.at(count++)) - 33));
				}
			}
		}
	}
	else {
		// Generate new chunk
		generateChunk(chunk);
		saveChunk(chunk);
	}
}

void World::saveChunk(Chunk* chunk) {
	const std::string chunkFilePath = getWorldFolder() + "chunks\\x" + std::to_string(chunk->getChunkXPos()) + "z" + std::to_string(chunk->getChunkZPos()) + ".chunk";

	// Put all chunk data into a string
	std::string chunkData = "";
	for (int y = 0; y < CHUNK_MAX_HEIGHT; y++) {
		for (int z = 0; z < CHUNK_MAX_WIDTH; z++) {
			for (int x = 0; x < CHUNK_MAX_WIDTH; x++) {
				Block* block = chunk->getBlock(x, y, z);
				char blockData;
				if (block == nullptr) {
					blockData = 33;
				}
				else {
					blockData = block->getType() + 33;
				}
				chunkData.push_back(blockData);
			}
		}
	}

	//std::cout << chunkData << std::endl;

	try {
		std::ofstream file(chunkFilePath);
		file << chunkData;
		file.close();
	}
	catch (std::ofstream::failure e) {
		std::cout << "ERROR::CHUNK::FILE_NOT_SUCCESSFULLY_WRITTEN" << std::endl;
		std::cout << "\tfile: " << chunkFilePath << std::endl;
		return;
	}
}

inline bool World::fileExists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

void World::updateChunkNoOffset(int xPos, int zPos) {
	Chunk* chunk = getChunkNoOffset(xPos, zPos);

	if (chunk == nullptr) {
		std::cout << "Failed updating chunk x=" << xPos << " z=" << zPos << std::endl;
		return;
	}

	chunk->doUpdate((xPos < chunksLength - 1) ? getChunkNoOffset(xPos + 1, zPos) : nullptr,
		(xPos > 0) ? getChunkNoOffset(xPos - 1, zPos) : nullptr,
		(zPos < chunksLength - 1) ? getChunkNoOffset(xPos, zPos + 1) : nullptr,
		(zPos > 0) ? getChunkNoOffset(xPos, zPos - 1) : nullptr);
}

void World::updateChunkNoOffset(Chunk* chunk) {
	int xPos = chunk->getChunkXPos();
	int zPos = chunk->getChunkZPos();

	if (chunk == nullptr) {
		std::cout << "Failed updating chunk x=" << xPos << " z=" << zPos << std::endl;
		return;
	}

	chunk->doUpdate((xPos < chunksLength - 1) ? getChunkNoOffset(xPos + 1, zPos) : nullptr,
		(xPos > 0) ? getChunkNoOffset(xPos - 1, zPos) : nullptr,
		(zPos < chunksLength - 1) ? getChunkNoOffset(xPos, zPos + 1) : nullptr,
		(zPos > 0) ? getChunkNoOffset(xPos, zPos - 1) : nullptr);
}

void World::shiftChunksThread(int xPos, int zPos) {
	int prevXPos = (renderDistance + bufferDistance) - chunkXOffset;
	int prevZPos = (renderDistance + bufferDistance) - chunkZOffset;

	// Copy chunks array
	Chunk** chunksTemp = new Chunk * [chunksLength * chunksLength];
	for (int i = 0; i < chunksLength * chunksLength; i++) {
		chunksTemp[i] = chunks[i];
	}

	// Check X
	if (xPos != prevXPos) {
		int shiftDist = (prevXPos - xPos) * chunksLength;
		chunkXOffset += prevXPos - xPos;

		if (xPos > prevXPos) {
			// Shift and clean chunks array
			for (int i = 0; i < chunksLength * chunksLength; i++) {
				Chunk* chunk = chunksTemp[i];
				chunksTemp[i] = nullptr;

				if (i + shiftDist < 0) {
					saveChunk(chunk);
					chunk->setDelete();
				}
				else {
					chunksTemp[i + shiftDist] = chunk;
				}
			}

			// Load new data
			for (int i = (chunksLength * chunksLength) + shiftDist; i < chunksLength * chunksLength; i++) {
				if (chunksTemp[i] != nullptr) {
					std::cout << "i=" << i << " shiftDist=" << shiftDist << " chunksLength=" << chunksLength << std::endl;
				}
				else {
					int chunkX = (int)(i / chunksLength) - chunkXOffset;
					int chunkZ = i % chunksLength - chunkZOffset;
					chunksTemp[i] = new Chunk(blockConsts, chunkX, chunkZ);
					loadChunk(chunksTemp[i]);
				}
			}
		}
		else {
			// Shift and clean chunks array
			for (int i = (chunksLength * chunksLength) - 1; i >= 0; i--) {
				Chunk* chunk = chunksTemp[i];
				chunksTemp[i] = nullptr;

				if (i + shiftDist >= chunksLength * chunksLength) {
					saveChunk(chunk);
					chunk->setDelete();
				}
				else {
					chunksTemp[i + shiftDist] = chunk;
				}
			}

			// Load new data
			for (int i = 0; i < shiftDist; i++) {
				if (chunksTemp[i] != nullptr) {
					std::cout << "i=" << i << " shiftDist=" << shiftDist << " chunksLength=" << chunksLength << std::endl;
				}
				else {
					int chunkX = (int)(i / chunksLength) - chunkXOffset;
					int chunkZ = i % chunksLength - chunkZOffset;
					chunksTemp[i] = new Chunk(blockConsts, chunkX, chunkZ);
					loadChunk(chunksTemp[i]);
				}
			}
		}
	}

	// Check Z
	if (zPos != prevZPos) {
		int shiftDist = prevZPos - zPos;
		chunkZOffset += shiftDist;

		if (zPos > prevZPos) {
			// Shift and clean chunks array
			for (int i = 0; i < chunksLength * chunksLength; i++) {
				Chunk* chunk = chunksTemp[i];
				chunksTemp[i] = nullptr;

				if ((int)((i + shiftDist) / chunksLength) != (int)(i / chunksLength) || i + shiftDist < 0) {
					saveChunk(chunk);
					chunk->setDelete();
				}
				else {
					chunksTemp[i + shiftDist] = chunk;
				}
			}
			
			// Load new chunks
			for (int i = 0; i < chunksLength * chunksLength; i++) {
				if (i % chunksLength >= chunksLength + shiftDist) {
					if (chunksTemp[i] != nullptr) {
						std::cout << "i=" << i << " shiftDist=" << shiftDist << " chunksLength=" << chunksLength << std::endl;
					}
					int chunkX = (int)(i / chunksLength) - chunkXOffset;
					int chunkZ = i % chunksLength - chunkZOffset;
					Chunk* chunk = new Chunk(blockConsts, chunkX, chunkZ);
					loadChunk(chunk);
					chunksTemp[i] = chunk;
				}
			}
		}
		else {
			// Shift and clean chunks array
			for (int i = (chunksLength * chunksLength) - 1; i >= 0 ; i--) {
				Chunk* chunk = chunksTemp[i];
				chunksTemp[i] = nullptr;

				if ((int)((i + shiftDist) / chunksLength) != (int)(i / chunksLength) || i + shiftDist >= chunksLength * chunksLength) {
					saveChunk(chunk);
					chunk->setDelete();
				}
				else {
					chunksTemp[i + shiftDist] = chunk;
				}
			}

			// Load new chunks
			for (int i = 0; i < chunksLength * chunksLength; i++) {
				if (i % chunksLength < shiftDist) {
					if (chunksTemp[i] != nullptr) {
						std::cout << "i=" << i << " shiftDist=" << shiftDist << " chunksLength=" << chunksLength << std::endl;
					}
					int chunkX = (int)(i / chunksLength) - chunkXOffset;
					int chunkZ = i % chunksLength - chunkZOffset;
					Chunk* chunk = new Chunk(blockConsts, chunkX, chunkZ);
					loadChunk(chunk);
					chunksTemp[i] = chunk;
				}
			}
		}
	}

	// Update the chunks
	for (int i = bufferDistance; i <= (renderDistance * 2) + bufferDistance; i++) {
		for (int j = bufferDistance; j <= (renderDistance * 2) + bufferDistance; j++) {
			Chunk* chunk;
			if ((i * chunksLength) + j >= chunksLength * chunksLength || (i * chunksLength) + j < 0) {
				chunk = nullptr;
				continue;
			}
			else {
				chunk = chunksTemp[(i * chunksLength) + j];
			}

			chunk->doUpdate((i < chunksLength - 1) ? chunksTemp[((i + 1) * chunksLength) + j] : nullptr,
				(i > 0) ? chunksTemp[((i - 1) * chunksLength) + j] : nullptr,
				(j < chunksLength - 1) ? chunksTemp[(i * chunksLength) + j + 1] : nullptr,
				(j > 0) ? chunksTemp[(i * chunksLength) + j - 1] : nullptr);

			chunk->setRender(true);
		}
	}

	// Switch to new chunks array
	Chunk** chunksOld = chunks;
	chunks = chunksTemp;

	// Clean up old chunks array
	for (int i = 0; i < chunksLength * chunksLength; i++) {
		if (chunksOld[i]->shouldDelete()) {
			delete chunksOld[i];
		}
	}
	delete[] chunksOld;

	std::cout << "---==== Printing out chunks ====---" << std::endl;

	for (int i = 0; i < chunksLength * chunksLength; i++) {
		std::cout << "i=" << i << " x=" << chunksTemp[i]->getChunkXPos() << " z=" << chunksTemp[i]->getChunkZPos() << " show=" << chunksTemp[i]->shouldRender() << std::endl;
	}

	std::cout << "---==== Shifted chunks ====---" << std::endl;
}

void World::shiftChunks(int xPos, int zPos) {
	std::cout << "---==== Shifting chunks ====---" << std::endl;
	//std::thread threadShiftChunks(&World::shiftChunksThread, this, xPos, zPos);
	//threadShiftChunks.detach();
	shiftChunksThread(xPos, zPos);
}

void World::updateChunkRenderDistance(int renderDistance, int bufferDistance, int xPos, int zPos) {
	this->renderDistance = renderDistance;
	this->bufferDistance = bufferDistance;

	// Clear chunk arrays
	if (chunks != nullptr) {
		delete[] chunks;
	}

	// Create new arrays
	chunksLength = 1 + (renderDistance * 2) + (bufferDistance * 2);
	chunks = new Chunk * [chunksLength * chunksLength];
	
	// Set offsets
	int chunkX = (xPos >= 0) ? xPos / CHUNK_MAX_WIDTH : (xPos / CHUNK_MAX_WIDTH) - 1;
	int chunkZ = (zPos >= 0) ? zPos / CHUNK_MAX_WIDTH : (zPos / CHUNK_MAX_WIDTH) - 1;

	chunkXOffset = chunkX + renderDistance + bufferDistance;
	chunkZOffset = chunkZ + renderDistance + bufferDistance;

	// Load chunks
	for (int x = -(renderDistance + bufferDistance); x <= renderDistance + bufferDistance; x++) {
		for (int z = -(renderDistance + bufferDistance); z <= renderDistance + bufferDistance; z++) {
			int offsetX = x + chunkXOffset;
			int offsetZ = z + chunkZOffset;
			chunks[(offsetX * chunksLength) + offsetZ] = new Chunk(blockConsts, x, z);
			loadChunk(chunks[(offsetX * chunksLength) + offsetZ]);
		}
	}

	// Update the chunks
	for (int i = bufferDistance; i <= (renderDistance * 2) + bufferDistance; i++) {
		for (int j = bufferDistance; j <= (renderDistance * 2) + bufferDistance; j++) {
			updateChunkNoOffset(i, j);
			getChunkNoOffset(i, j)->setRender(true);
		}
	}
}