#pragma once

#include "ogl_presets.h"
#include "ogl_texture.h"
#include "ogl_object.h"

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