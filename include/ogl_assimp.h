#pragma once

#include "ogl_presets.h"
#include "ogl_shader.h"

struct ass_vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct ass_texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

class ass_mesh {
public:
    // mesh data
    std::vector<ass_vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<ass_texture> textures;

    ass_mesh
    (
        std::vector<ass_vertex> vertices, 
        std::vector<unsigned int> indices, 
        std::vector<ass_texture> textures
    );

    void draw(shader& ourShader);

private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};

class ass_model
{
public:
    ass_model(std::string path);
    void draw(shader& ourShader);

private:
    // model data
    std::vector<ass_mesh> meshes;
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    ass_mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<ass_texture> loadMaterialTextures
    (
        aiMaterial* mat, 
        aiTextureType type,
        std::string typeName
    );
};