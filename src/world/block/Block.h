#pragma once

#include <vector>
#include <iostream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../Mesh.h"
#include "../../utils/Texture_Loader.h"
#include "Block_Consts.h"

class Block {
private:
	Block_Type type;

public:
	Block(Block_Type type = AIR);
	~Block();

	Block_Type getType();
	void setType(Block_Type type);
};

