#include "Entity.h"

Entity::Entity() {

}

Entity::Entity(World* world, glm::vec3 position, glm::vec3 dimentions, GLfloat yaw, GLfloat pitch, bool flying, float speed, float jumpSpeed) {
	this->world = world;
	this->position = position;
	this->dimentions = dimentions;
	this->worldUp = glm::vec3(0, 1, 0);
	this->yaw = yaw;
	this->pitch = pitch;
	this->speed = speed;
	this->jumpSpeed = jumpSpeed;
	this->potentialPos = position;
	this->flying = flying;

	velocity = glm::vec3(0.0f, 0.0f, 0.0f);

	updateVectors();
}

Entity::~Entity() {

}

void Entity::doUpdate() {
	// Update velocity
	if (!flying) {
		this->velocity -= this->worldUp * 0.0005f;	// Gravity

		this->potentialPos += this->velocity;

		velocity.x = velocity.x * 0.9f;
		velocity.z = velocity.z * 0.9f;
	}
	else {
		this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	// Collision Detection
	glm::vec2 entityPos[] = { glm::vec2(potentialPos.x - (dimentions.x / 2), potentialPos.x + (dimentions.x / 2)),
		glm::vec2(potentialPos.y, potentialPos.y + dimentions.y), glm::vec2(potentialPos.z - (dimentions.z / 2), potentialPos.z + (dimentions.z / 2)) };
	bool collided = false;
	glm::vec3 posInt = glm::vec3(glm::floor(position.x), glm::floor(position.y), glm::floor(position.z));

	glm::vec4 collisionPosFaces[] = {
		glm::vec4(-1, 0, 0, XPOS), glm::vec4(-1, 1, 0, XPOS),

		glm::vec4(1, 0, 0, XNEG), glm::vec4(1, 1, 0, XNEG),

		glm::vec4(0, -1, 0, YPOS), glm::vec4(1, -1, 0, YPOS),

		glm::vec4(-1, -1, 0, YPOS), glm::vec4(0, -1, 1, YPOS),

		glm::vec4(0, -1, -1, YPOS),

		glm::vec4(0, 2, 0, YNEG),

		glm::vec4(0, 0, -1, ZPOS), glm::vec4(0, 1, -1, ZPOS),

		glm::vec4(0, 0, 1, ZNEG), glm::vec4(0, 1, 1, ZNEG)
	};

	for (int i = 0; i < 14; i++) {
		Block* block = world->getBlock(posInt.x + collisionPosFaces[i].x, posInt.y + collisionPosFaces[i].y, posInt.z + collisionPosFaces[i].z);

		if (block != nullptr && block->getType() != AIR) {
			glm::vec2 blockPos[] = {
				glm::vec2(posInt.x + collisionPosFaces[i].x, posInt.x + collisionPosFaces[i].x + 1),
				glm::vec2(posInt.y + collisionPosFaces[i].y, posInt.y + collisionPosFaces[i].y + 1),
				glm::vec2(posInt.z + collisionPosFaces[i].z, posInt.z + collisionPosFaces[i].z + 1)
			};

			int blockPoint;
			int side = collisionPosFaces[i].w / 2;
			if ((int)collisionPosFaces[i].w % 2 == 0) {
				blockPoint = blockPos[side].y;
			}
			else {
				blockPoint = blockPos[side].x;
			}

			if (collisionDetection.cubeToRect(entityPos[(side + 1) % 3], entityPos[(side + 2) % 3], entityPos[side % 3],
				blockPos[(side + 1) % 3], blockPos[(side + 2) % 3], blockPoint)) {
				collided = true;
				if (side == 0) {
					potentialPos.x = position.x;
					velocity.x = 0.0f;
				}
				else if (side == 1) {
					potentialPos.y = position.y;
					velocity.y = 0.0f;
				}
				else {
					potentialPos.z = position.z;
					velocity.z = 0.0f;
				}
			}
		}
	}

	position = potentialPos;
	updateVectors();
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