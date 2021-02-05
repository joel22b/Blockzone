#include "game.h"

Game::Game() {
	//camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	textureLoader = new Texture_Loader();
	loadTextures();

	world = World(textureLoader);
}

Game::~Game() {
}

void Game::doInput() {

}

void Game::doUpdate() {

}

void Game::doRender(Shader shader, GLint modelLoc) {
	world.doRender(shader, modelLoc);
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