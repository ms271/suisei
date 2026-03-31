#pragma once

#include "ogl_presets.h"
#include "ogl_shader.h"

struct aTexture {
    unsigned int id;
    std::string type;
};

struct aVertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Color;
};

struct aMaterial
{
    glm::vec3 diffuseColor;

};

class aMesh {
public:
    // mesh data
    std::vector<aVertex>      vertices;
    std::vector<unsigned int> indices;
    std::vector<aTexture>     textures;

    aMesh(std::vector<aVertex> vertices, std::vector<unsigned int> indices, std::vector<aTexture> textures);
    void Draw(Shader& shader);
private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void SetupMesh();
};