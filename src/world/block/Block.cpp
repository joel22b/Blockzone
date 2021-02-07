#include "Block.h"

Block::Block(Block_Type type) {
	this->type = type;
}

Block::~Block() {
	this->type = AIR;
}

Block_Type Block::getType() {
	return this->type;
}
