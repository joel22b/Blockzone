#include "Block.h"

Block::Block() {
	this->update = false;
	this->type = AIR;

	for (int i = 0; i < 6; i++) {
		this->render[i] = false;
	}
}

Block::Block(Block_Consts* blockConsts, Block_Type type, bool update, bool render) {
	this->type = type;
	this->update = update;
	this->blockConsts = blockConsts;

	for (int i = 0; i < 6; i++) {
		this->render[i] = render;
	}

	std::ostringstream prefix;
	prefix << type << "_top";
	texturesTop = blockConsts->loadTextures(prefix.str());
	prefix.str("");
	prefix << type << "_bottom";
	texturesBottom = blockConsts->loadTextures(prefix.str());
	prefix.str("");
	prefix << type << "_side";
	texturesSide = blockConsts->loadTextures(prefix.str());
}

Block::~Block() {
	this->type = AIR;
}

void Block::doRender(Shader shader, GLint modelLoc, glm::vec3 position) {
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glm::translate(glm::mat4(1), position)));
	for (int i = 0; i < 6; i++) {
		if (render[i]) {
			if (i == YPOS) {
				blockConsts->meshs[i].doRender(shader, texturesTop);
			}
			else if (i == YNEG) {
				blockConsts->meshs[i].doRender(shader, texturesBottom);
			}
			else {
				blockConsts->meshs[i].doRender(shader, texturesSide);
			}
		}
	}
}

void Block::setRender(Block_Side side, bool val) {
	render[side] = val;
}

bool Block::getRender(Block_Side side) {
	return render[side];
}

void Block::setUpdate(bool update) {
	this->update = update;
}
bool Block::getUpdate() {
	return this->update;
}

Block_Type Block::getType() {
	return this->type;
}
