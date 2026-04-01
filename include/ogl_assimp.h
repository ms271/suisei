#pragma once

#include "ogl_presets.h"
#include "ogl_shader.h"

unsigned int TextureFromMemory(const aiTexture* embeddedTexture);
unsigned int TextureFromFile(const char* path, const std::string& directory);

struct aTexture {
    unsigned int id;
    unsigned int type;//1 diffuse, 2 specular
    std::string path;
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

class aModel
{
public:
    aModel(std::string path);
    void Draw(Shader& shader);
private:
    // model data
    std::vector<aMesh> meshes;
    std::string directory;
    std::vector<aTexture> textures_loaded;
    std::unordered_map <std::string, aTexture> texLoaded;

    void loadModel(std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    aMesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<aTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const aiScene* scene);
};