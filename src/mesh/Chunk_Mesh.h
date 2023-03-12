#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <mutex>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../Shaders/Shader.h"
#include "../utils/Texture_Loader.h"

using namespace std;

struct Block_Face
{
    // Position
    glm::vec3 Position;
    // Normal
    glm::vec3 Normal;
    // TexCoords
    glm::vec2 TexCoords;
};

class Chunk_Mesh
{
public:
    /*  Functions  */
    // Constructor
    Chunk_Mesh(vector<Block_Face> vertices, vector<Texture> textures);

    void doRender(Shader shader);

    void updateVertices(vector<Block_Face> vertices);

private:
    /*  Mesh Data  */
    vector<Block_Face> vertices;
    vector<Texture> textures;
    std::mutex meshMutex, texturesMutex;

    /*  Render data  */
    GLuint VAO, VBO;

    /*  Functions    */
    // Initializes all the buffer objects/arrays
    void setupMesh();

    // Render the mesh
    void Draw(Shader shader);
};