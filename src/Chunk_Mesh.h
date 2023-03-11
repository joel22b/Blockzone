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

#include "../Shaders/Shader.h"
#include "utils/Texture_Loader.h"

#include "utils/Logger.h"
#define LOG(severity, msg) Logger::log("Chunk_Mesh.h", severity, msg)

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
    Chunk_Mesh(vector<Block_Face> vertices, vector<Texture> textures) {
        updateVertices(vertices);
        
        texturesMutex.lock();
        this->textures = textures;
        texturesMutex.unlock();
    }

    void doRender(Shader shader) {
        if (meshMutex.try_lock()) {
            if (texturesMutex.try_lock()) {
                Draw(shader);
                texturesMutex.unlock();
            }
            else {
                LOG(DEBUG, "Texture mutex locked");
            }
            meshMutex.unlock();
        }
        else {
            LOG(DEBUG, "Mesh mutex locked");
        }
    }

    void updateVertices(vector<Block_Face> vertices) {
        meshMutex.lock();
        this->vertices = vertices;

        setupMesh();
        meshMutex.unlock();
    }

private:
    /*  Mesh Data  */
    vector<Block_Face> vertices;
    vector<Texture> textures;
    std::mutex meshMutex, texturesMutex;

    /*  Render data  */
    GLuint VAO, VBO;

    /*  Functions    */
    // Initializes all the buffer objects/arrays
    void setupMesh()
    {
        // Create buffers/arrays
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);

        glBindVertexArray(this->VAO);
        // Load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Block_Face), &this->vertices[0], GL_STATIC_DRAW);

        // Set the vertex attribute pointers
        // Vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Block_Face), (GLvoid*)0);
        // Vertex Normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Block_Face), (GLvoid*)offsetof(Block_Face, Normal));
        // Vertex Texture Coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Block_Face), (GLvoid*)offsetof(Block_Face, TexCoords));

        glBindVertexArray(0);
    }

    // Render the mesh
    void Draw(Shader shader)
    {
        shader.Use();

        // Bind appropriate textures
        GLuint diffuseNr = 1;
        GLuint specularNr = 1;

        for (GLuint i = 0; i < this->textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
            // Retrieve texture number (the N in diffuse_textureN)
            stringstream ss;
            string number;
            string name = this->textures[i].type;

            if (name == "texture_diffuse")
            {
                ss << diffuseNr++; // Transfer GLuint to stream
            }
            else if (name == "texture_specular")
            {
                ss << specularNr++; // Transfer GLuint to stream
            }

            number = ss.str();
            // Now set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(shader.getProgram(), (name + number).c_str()), i);
            // And finally bind the texture
            glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
        }

        // Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
        glUniform1f(glGetUniformLocation(shader.getProgram(), "material.shininess"), 32.0f);

        // Draw mesh
        glBindVertexArray(this->VAO);
        glDrawArrays(GL_POINTS, 0, vertices.size());
        glBindVertexArray(0);

        // Always good practice to set everything back to defaults once configured.
        for (GLuint i = 0; i < this->textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
};