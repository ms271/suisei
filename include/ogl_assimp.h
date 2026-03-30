#pragma once

#include "ogl_presets.h"
#include "ogl_shader.h"

const inline glm::mat4 unitTransform = glm::mat4(1);

struct ass_vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    glm::vec4 Color = glm::vec4(1);
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

    float shininess;
    glm::mat4 Transform;
    bool aspSpec;

    ass_mesh
    (
        std::vector<ass_vertex>& VErtices,
        std::vector<unsigned int>& INdices, 
        std::vector<ass_texture>& TExtures,
        float SHininess
    );

    void draw(shader& ourShader, glm::mat4& worldTransform);

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
    glm::mat4 modelTransform = unitTransform;
    bool asp = true;

private:
    // model data
    std::vector<ass_mesh> meshes;
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene, glm::mat4& parentTransform);
    ass_mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<ass_texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName, const aiScene* scene);
};