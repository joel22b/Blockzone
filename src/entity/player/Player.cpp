#include "Player.h"

Player::Player() : Entity() {
	camera = nullptr;
}

Player::Player(World* world, glm::vec3 position, glm::vec3 dimentions, GLfloat yaw, GLfloat pitch, float speed) : Entity(world, position, dimentions, yaw, pitch, speed) {
	camera = new Camera(position);
	camera->setCameraVectors(position + glm::vec3(0, 1, 0), front, right, up);
}

void Player::processKeyboardInput(Player_Movement movement, GLfloat deltaTime) {
	float velocity = this->speed * deltaTime;

	if (movement == FORWARD) {
		this->potentialPos += this->front * velocity;
	}
	if (movement == BACKWARD) {
		this->potentialPos -= this->front * velocity;
	}
	if (movement == LEFT) {
		this->potentialPos -= this->right * velocity;
	}
	if (movement == RIGHT) {
		this->potentialPos += this->right * velocity;
	}
	if (movement == JUMP) {
		position = potentialPos;
	}
}

void Player::processMouseInput(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch) {
	xOffset *= this->mouseSensitivity;
	yOffset *= this->mouseSensitivity;

	this->yaw += xOffset;
	this->pitch += yOffset;

	if (constrainPitch) {
		if (this->pitch > 89.0f) {
			this->pitch = 89.0f;
		}
		if (this->pitch < -89.f) {
			this->pitch = -89.0f;
		}
	}

	updateVectors();
}

glm::mat4 Player::getViewMatrix() {
	return camera->getViewMatrix();
}

glm::vec3 Player::getCameraPosition() {
	return camera->getPosition();
}

glm::vec3 Player::getCameraDirection() {
	return camera->getViewDirection();
}

void Player::updateVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	front.y = sin(glm::radians(this->pitch));
	front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

	this->front = glm::normalize(front);
	this->right = glm::normalize(glm::cross(this->front, this->worldUp));
	this->up = glm::normalize(glm::cross(this->right, this->front));

	camera->setCameraVectors(position + glm::vec3(0, 1, 0), front, right, up);
}