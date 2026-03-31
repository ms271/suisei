#include"../include/ogl_assimp.h"

aMesh::aMesh(std::vector<aVertex> VErtices, std::vector<unsigned int> INdices, std::vector<aTexture> TExtures)
{
    this->vertices = VErtices;
    this->indices = INdices;
    this->textures = TExtures;
    
    SetupMesh();
}

void aMesh::SetupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //type, total size stream, home or start pointer, type of draw
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(aVertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //type, total size, home or start pointer, type of draw
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    //assume a stream made up of individual blocks, each block of same size and every corresponding element of two blocks represent the same property

    // vertex positions
    glEnableVertexAttribArray(0);
    //channel, no of entries in block, type, normalization, size of block, offset from start of block
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(aVertex), (void*)0);
    // vertex texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(aVertex), (void*)offsetof(aVertex, TexCoords));
    // vertex normals
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(aVertex), (void*)offsetof(aVertex, Normal));
    // vertex Color
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(aVertex), (void*)offsetof(aVertex, Color));

    glBindVertexArray(0);
}

void aMesh::Draw(Shader& shader)
{
//textures have been preloaded into their respective ids, what we have to do is bind them at their respective postions
    
    for (int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        shader.setInt("asiTexture[" + std::to_string(i) + "]", textures[i].id);
        shader.setInt("asiTexType[" + std::to_string(i) + "]", textures[i].type);//0 means diffuse
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

aModel::aModel(std::string path)
{
    loadModel(path);
}

void aModel::loadModel(std::string& path)
{
//make and add import to a scene
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
//error message for scene
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
//save directory
    directory = path.substr(0, path.find_last_of('/'));
//recursive processing
    processNode(scene->mRootNode, scene);
}

void aModel::processNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

aMesh aModel::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<aVertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<aTexture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        aVertex vertex;
        // process vertex positions, normals and texture coordinates
        vertices.push_back(vertex);
    }
    // process indices
    
    // process material
    if (mesh->mMaterialIndex >= 0)
    {
    
    }

    return aMesh(vertices, indices, textures);
}