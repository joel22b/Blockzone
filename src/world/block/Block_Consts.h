#pragma once

#include <iostream>
#include <vector>

#include "glm/glm.hpp"

#include "../../Mesh.h"
#include "../../../Textures/Texture_Loader.h"

enum Block_Type {
	AIR = 0,
	GRASS = 1,
	DIRT = 2
};

enum Block_Side {
	XPOS = 0,
	XNEG = 1,
	YPOS = 2,
	YNEG = 3,
	ZPOS = 4,
	ZNEG = 5
};

class Block_Consts {
public:
	Mesh meshs[6];

	Block_Consts() {}

	Block_Consts(Texture_Loader* textureLoader) {
		this->textureLoader = textureLoader;

		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;

		Vertex vertex;
		// XPOS
		vertex.Position = glm::vec3(1.0f, 0.0f, 0.0f);
		vertex.Normal = glm::vec3(1.0f, 0.0f, 0.0f);
		vertex.TexCoords = glm::vec2(0.0f, 1.0f);
		vertices.push_back(vertex);

		vertex.Position = glm::vec3(1.0f, 0.0f, 1.0f);
		vertex.TexCoords = glm::vec2(1.0f, 1.0f);
		vertices.push_back(vertex);

		vertex.Position = glm::vec3(1.0f, 1.0f, 0.0f);
		vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);

		vertex.Position = glm::vec3(1.0f, 1.0f, 1.0f);
		vertex.TexCoords = glm::vec2(1.0f, 0.0f);
		vertices.push_back(vertex);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(3);

		this->meshs[0] = Mesh(vertices, indices);
		vertices.clear();
		indices.clear();

		// XNEG
		vertex.Position = glm::vec3(0.0f, 0.0f, 0.0f);
		vertex.Normal = glm::vec3(-1.0f, 0.0f, 0.0f);
		vertex.TexCoords = glm::vec2(0.0f, 1.0f);
		vertices.push_back(vertex);

		vertex.Position = glm::vec3(0.0f, 0.0f, 1.0f);
		vertex.TexCoords = glm::vec2(1.0f, 1.0f);
		vertices.push_back(vertex);

		vertex.Position = glm::vec3(0.0f, 1.0f, 0.0f);
		vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);

		vertex.Position = glm::vec3(0.0f, 1.0f, 1.0f);
		vertex.TexCoords = glm::vec2(1.0f, 0.0f);
		vertices.push_back(vertex);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(3);

		this->meshs[1] = Mesh(vertices, indices);
		vertices.clear();
		indices.clear();

		// YPOS
		vertex.Position = glm::vec3(0.0f, 1.0f, 0.0f);
		vertex.Normal = glm::vec3(0.0f, 0.0f, 1.0f);
		vertex.TexCoords = glm::vec2(0.0f, 1.0f);
		vertices.push_back(vertex);

		vertex.Position = glm::vec3(1.0f, 1.0f, 0.0f);
		vertex.TexCoords = glm::vec2(1.0f, 1.0f);
		vertices.push_back(vertex);

		vertex.Position = glm::vec3(0.0f, 1.0f, 1.0f);
		vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);

		vertex.Position = glm::vec3(1.0f, 1.0f, 1.0f);
		vertex.TexCoords = glm::vec2(1.0f, 0.0f);
		vertices.push_back(vertex);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(3);

		this->meshs[2] = Mesh(vertices, indices);
		vertices.clear();
		indices.clear();

		// YNEG
		vertex.Position = glm::vec3(0.0f, 0.0f, 0.0f);
		vertex.Normal = glm::vec3(0.0f, -1.0f, 0.0f);
		vertex.TexCoords = glm::vec2(0.0f, 1.0f);
		vertices.push_back(vertex);

		vertex.Position = glm::vec3(1.0f, 0.0f, 0.0f);
		vertex.TexCoords = glm::vec2(1.0f, 1.0f);
		vertices.push_back(vertex);

		vertex.Position = glm::vec3(0.0f, 0.0f, 1.0f);
		vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);

		vertex.Position = glm::vec3(1.0f, 0.0f, 1.0f);
		vertex.TexCoords = glm::vec2(1.0f, 0.0f);
		vertices.push_back(vertex);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(3);

		this->meshs[3] = Mesh(vertices, indices);
		vertices.clear();
		indices.clear();

		// ZPOS
		vertex.Position = glm::vec3(0.0f, 0.0f, 1.0f);
		vertex.Normal = glm::vec3(0.0f, 0.0f, 1.0f);
		vertex.TexCoords = glm::vec2(0.0f, 1.0f);
		vertices.push_back(vertex);

		vertex.Position = glm::vec3(1.0f, 0.0f, 1.0f);
		vertex.TexCoords = glm::vec2(1.0f, 1.0f);
		vertices.push_back(vertex);

		vertex.Position = glm::vec3(0.0f, 1.0f, 1.0f);
		vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);

		vertex.Position = glm::vec3(1.0f, 1.0f, 1.0f);
		vertex.TexCoords = glm::vec2(1.0f, 0.0f);
		vertices.push_back(vertex);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(3);

		this->meshs[4] = Mesh(vertices, indices);
		vertices.clear();
		indices.clear();

		// ZNEG
		vertex.Position = glm::vec3(0.0f, 0.0f, 0.0f);
		vertex.Normal = glm::vec3(0.0f, 0.0f, -1.0f);
		vertex.TexCoords = glm::vec2(0.0f, 1.0f);
		vertices.push_back(vertex);

		vertex.Position = glm::vec3(1.0f, 0.0f, 0.0f);
		vertex.TexCoords = glm::vec2(1.0f, 1.0f);
		vertices.push_back(vertex);

		vertex.Position = glm::vec3(0.0f, 1.0f, 0.0f);
		vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);

		vertex.Position = glm::vec3(1.0f, 1.0f, 0.0f);
		vertex.TexCoords = glm::vec2(1.0f, 0.0f);
		vertices.push_back(vertex);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(3);

		this->meshs[5] = Mesh(vertices, indices);
		vertices.clear();
		indices.clear();
	}

	std::vector<Texture> loadTextures(std::string prefix) {
		return textureLoader->getDiffAndSpecTextures(prefix);
	}

private:
	Texture_Loader* textureLoader;
};
