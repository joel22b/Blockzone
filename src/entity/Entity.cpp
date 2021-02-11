#include "Entity.h"

Entity::Entity() {

}

Entity::Entity(World* world, glm::vec3 position, glm::vec3 dimentions, GLfloat yaw, GLfloat pitch, float speed) {
	this->world = world;
	this->position = position;
	this->dimentions = dimentions;
	this->worldUp = glm::vec3(0, 1, 0);
	this->yaw = yaw;
	this->pitch = pitch;
	this->speed = speed;
	this->potentialPos = position;

	updateVectors();
}

Entity::~Entity() {

}

void Entity::doUpdate() {
	/*Block* block = world->getBlock(position.x, position.y - 1, position.z);
	if (block != nullptr && block->getType() == AIR) {
		block->setType(DIRT);
		world->getChunk(position.x, position.z)->doUpdate(world->getChunk(position.x + 16, position.z), world->getChunk(position.x - 16, position.z),
			world->getChunk(position.x, position.z + 16), world->getChunk(position.x, position.z - 16));
	}*/
	for (int x = glm::floor(position.x) - 1; x < glm::floor(position.x) + 2; x++) {
		for (int z = glm::floor(position.z) - 1; z < glm::floor(position.z) + 2; z++) {
			//std::cout << "X: " << x << " Z: " << z << std::endl;
			glm::vec3 blockPos = glm::vec3(x, glm::floor(position.y) - 1, z);
			Block* block = world->getBlock(blockPos.x, blockPos.y, blockPos.z);
			if (block != nullptr) {
				if (block->getType() != AIR) {
					if (collisionDetection.cubeToRect(glm::vec2(potentialPos.x - (dimentions.x / 2), potentialPos.x + (dimentions.x / 2)), glm::vec2(potentialPos.z - (dimentions.z / 2), potentialPos.z + (dimentions.z / 2)),
						glm::vec2(potentialPos.y, potentialPos.y + dimentions.y), glm::vec2(blockPos.x, blockPos.x + 1), glm::vec2(blockPos.z, blockPos.z + 1), blockPos.y + 1)) {
						potentialPos.y = position.y;
						std::cout << "Collision x:" << blockPos.x << " y: " << blockPos.y << " z: " << blockPos.z << std::endl;
					}
				}
			}
		}
	}
	position = potentialPos;
	updateVectors();
	//return output.str();
}

glm::vec3 Entity::getPosition() {
	return position;
}

glm::vec3 Entity::getDirection() {
	return front;
}

void Entity::updateVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	front.y = sin(glm::radians(this->pitch));
	front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

	this->front = glm::normalize(front);
	this->right = glm::normalize(glm::cross(this->front, this->worldUp));
	this->up = glm::normalize(glm::cross(this->right, this->front));
}