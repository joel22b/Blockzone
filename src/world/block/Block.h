#pragma once

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

