#pragma once

#include <string>
#include <map>
#include <vector>

#include <GL/glew.h>

#include "../SOIL2/src/SOIL2/SOIL2.h";

struct Texture {
    GLuint id;
    std::string type;
};

class Texture_Loader {
private:
    std::map<std::string, GLuint> loadedTextures;

public:
	Texture_Loader() {}

	void loadTexture(std::string textureName, const char* path) {
        GLuint textureMap;
        glGenTextures(1, &textureMap);

        int textureWidth, textureHeight;
        unsigned char* image;

        image = SOIL_load_image(path, &textureWidth, &textureHeight, 0, SOIL_LOAD_RGBA);
        glBindTexture(GL_TEXTURE_2D, textureMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        this->loadedTextures.insert(make_pair(textureName, textureMap));
	}

    GLuint getTextureId(std::string textureName) {
        if(this->loadedTextures.count(textureName) > 0) {
            return this->loadedTextures[textureName];
        }
        else {
            std::cout << "ERROR::TEXTURE_LOADER::UNKNOWN_TEXTURE_NAME" << std::endl;
            std::cout << "\t" << textureName << std::endl;
            return this->loadedTextures["error"];
        }
    }

    std::vector<Texture> getDiffAndSpecTextures(std::string prefix) {
        std::vector<Texture> textures;
        Texture texture;
        
        texture.id = getTextureId(prefix + "_diffuse");
        texture.type = "texture_diffuse";
        textures.push_back(texture);

        texture.id = getTextureId(prefix + "_specular");
        texture.type = "texture_specular";
        textures.push_back(texture);

        return textures;
    }
};