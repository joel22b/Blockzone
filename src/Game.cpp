#include "game.h"

Game::Game() {
	//camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

	loadShaders();

	textureLoader = new Texture_Loader();
	loadTextures();

	world = World(textureLoader);

	blockShader.Use();
	glUniform1i(glGetUniformLocation(blockShader.getProgram(), "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(blockShader.getProgram(), "material.specular"), 1);

	int offsetArray[16] = { 0, 1, -1, 0, 1, 0, -1, 0, 1, -1, 0, 0, 0, 0, 0, 1};
	glm::mat4 offsetTransform = glm::make_mat4(offsetArray);
	GLint offsetTransformLoc = glGetUniformLocation(blockShader.getProgram(), "offsetTransform");
	glUniformMatrix4fv(offsetTransformLoc, 1, GL_FALSE, glm::value_ptr(offsetTransform));
	glUniform2f(glGetUniformLocation(blockShader.getProgram(), "texDim"), 3, 1);
}

Game::~Game() {
}

void Game::doInput() {

}

void Game::doUpdate(glm::vec3 viewPos, glm::mat4 projection, glm::mat4 view) {
	blockShader.Use();
	
	// Directional Light
	GLint lightDirLoc = glGetUniformLocation(blockShader.getProgram(), "dirLight.direction");
	glUniform3f(lightDirLoc, 1.0f, 10.0f, -3.0f);
	glUniform3f(glGetUniformLocation(blockShader.getProgram(), "dirLight.ambient"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(blockShader.getProgram(), "dirLight.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(blockShader.getProgram(), "dirLight.specular"), 1.0f, 1.0f, 1.0f);

	GLint modelLoc = glGetUniformLocation(blockShader.getProgram(), "model");
	GLint viewLoc = glGetUniformLocation(blockShader.getProgram(), "view");
	GLint projectionLoc = glGetUniformLocation(blockShader.getProgram(), "projection");

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glm::translate(glm::mat4(1), glm::vec3(-1, 0, -1))));

	GLint viewPosLoc = glGetUniformLocation(blockShader.getProgram(), "viewPos");
	glUniform3f(viewPosLoc, viewPos.x, viewPos.y, viewPos.z);
}

void Game::doRender() {
	//chunkMesh.doRender(blockShader);

	GLint modelLoc = glGetUniformLocation(blockShader.getProgram(), "model");
	world.doRender(blockShader, modelLoc);
}

void Game::loadShaders() {
	blockShader = Shader("Shaders\\block.vert", "Shaders\\block.frag", "Shaders\\block.geom");
}

void Game::loadTextures() {
	// Error texture
	textureLoader->loadTexture("error", "Textures/error.png");

	// Block Textures
	textureLoader->loadTexture("blocks_diffuse", "Textures/blocks_diffuse.png");
	textureLoader->loadTexture("blocks_specular", "Textures/blocks_specular.png");
}