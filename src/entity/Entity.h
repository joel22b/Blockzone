#pragma once

#include <ostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "../Collision_Detection.h"
#include "../world/World.h"
#include "../world/block/Block.h"

class Entity {
public:
	Entity();
	Entity(World* world, glm::vec3 position, glm::vec3 dimentions, GLfloat yaw = 0.0f, GLfloat pitch = 0.0f, float speed = 6.0f);
	~Entity();

	void doUpdate();

	glm::vec3 getPosition();
	glm::vec3 getDirection();
	
protected:
	World* world;

	glm::vec3 position, dimentions, front, worldUp, up, right;
	glm::vec3 potentialPos;
	GLfloat yaw, pitch;
	float speed;

	Collision_Detection collisionDetection;

	virtual void updateVectors();
};