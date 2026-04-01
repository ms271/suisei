#pragma once

#include "ogl_presets.h"
#include "ogl_shader.h"

unsigned int TextureFromMemory(const aiTexture* embeddedTexture);
unsigned int TextureFromFile(const char* path, const std::string& directory);
glm::mat4 aiToGlm(aiMatrix4x4& from);

struct aTexture {
    unsigned int id;
    unsigned int type;//1 diffuse, 2 specular
    std::string path;
    float blend;
    float blendOp;
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
    glm::vec3 baseColor;
    glm::vec3 ambientColor;
    glm::vec3 specularColor;

    float specularStrength;
    float specularExponent;
};

class aMesh {
public:
    // mesh data
    std::vector<aVertex>      vertices;
    std::vector<unsigned int> indices;
    std::vector<aTexture>     textures;
    aMaterial material;
    glm::mat4 Transform;

    aMesh(std::vector<aVertex> vertices, std::vector<unsigned int> indices, std::vector<aTexture> textures, aMaterial MAterial);
    void Draw(Shader& shader, glm::mat4& worldTransform);
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
    glm::mat4 modelTransform = glm::mat4(1.0f);
private:
    // model data
    std::vector<aMesh> meshes;
    std::string directory;
    std::unordered_map <std::string, aTexture> texLoaded;
    
    void loadModel(std::string& path);
    void processNode(aiNode* node, const aiScene* scene, glm::mat4 parentTransform);
    aMesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<aTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const aiScene* scene);
};