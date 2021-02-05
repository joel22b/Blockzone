#pragma once

#include <vector>
#include <iostream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "src/Mesh.h"
#include "Textures/Texture_Loader.h"
#include "src/world/block/Block_Consts.h"

class Block {
private:
	Block_Consts* blockConsts;
	Block_Type type;
	bool update = false;
	bool render[6];
	std::vector<Texture> texturesTop, texturesBottom, texturesSide;

public:
	Block();
	Block(Block_Consts* blockConsts, Block_Type type = AIR, bool update = true, bool render = false);
	~Block();

	void doRender(Shader shader, GLint modelLoc, glm::vec3 position);

	void setRender(Block_Side side, bool val);
	bool getRender(Block_Side);
	void setUpdate(bool update);
	bool getUpdate();
	Block_Type getType();
};

