#include "game.h"

Game::Game() {
	//camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	textureLoader = new Texture_Loader();
	blockConsts = new Block_Consts(textureLoader);

	loadTextures();

	chunk.addBlock(glm::vec3(0, 0, 0), GRASS, blockConsts);
	chunk.addBlock(glm::vec3(1, 0, 0), GRASS, blockConsts);
	chunk.addBlock(glm::vec3(0, 0, 1), GRASS, blockConsts);
	chunk.addBlock(glm::vec3(1, 0, 1), DIRT, blockConsts);
	chunk.addBlock(glm::vec3(2, 0, 1), GRASS, blockConsts);
	chunk.addBlock(glm::vec3(1, 0, 2), GRASS, blockConsts);
	chunk.addBlock(glm::vec3(1, 1, 1), DIRT, blockConsts);
	chunk.addBlock(glm::vec3(1, 2, 1), GRASS, blockConsts);
}

Game::~Game() {
}

void Game::doInput() {

}

void Game::doUpdate() {

}

void Game::doRender(Shader shader, GLint modelLoc) {
	chunk.doRender(shader, modelLoc);
	/*for (int i = 0; i < chunks.size(); i++) {
		for (int j = 0; j < chunks[i].size(); j++) {
			chunks[i][j].doRender(shader, modelLoc);
		}
	}*/
}

void Game::loadTextures() {
	// Error texture
	textureLoader->loadTexture("error", "Textures/error.png");

	// Grass
	textureLoader->loadTexture("1_side_diffuse", "Textures/block_grass_side_diffuse.png");
	textureLoader->loadTexture("1_side_specular", "Textures/block_grass_side_specular.png");
	textureLoader->loadTexture("1_bottom_diffuse", "Textures/block_grass_bottom_diffuse.png");
	textureLoader->loadTexture("1_bottom_specular", "Textures/block_grass_bottom_specular.png");
	textureLoader->loadTexture("1_top_diffuse", "Textures/block_grass_top_diffuse.png");
	textureLoader->loadTexture("1_top_specular", "Textures/block_grass_top_specular.png");

	// Dirt
	textureLoader->loadTexture("2_side_diffuse", "Textures/block_dirt_side_diffuse.png");
	textureLoader->loadTexture("2_side_specular", "Textures/block_dirt_side_specular.png");
	textureLoader->loadTexture("2_bottom_diffuse", "Textures/block_dirt_bottom_diffuse.png");
	textureLoader->loadTexture("2_bottom_specular", "Textures/block_dirt_bottom_specular.png");
	textureLoader->loadTexture("2_top_diffuse", "Textures/block_grass_dirt_diffuse.png");
	textureLoader->loadTexture("2_top_specular", "Textures/block_grass_dirt_specular.png");
}