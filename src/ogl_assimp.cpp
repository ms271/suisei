#include "../include/ogl_assimp.h"

std::vector<ass_texture> textures_loaded;

unsigned int TextureFromFile(const char* path, const std::string& directory)
{
    std::string filename = (std::string)path;
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

ass_mesh::ass_mesh
(
    std::vector<ass_vertex> VErtices,
    std::vector<unsigned int> INdices,
    std::vector<ass_texture> TExtures
)
{
    vertices = VErtices;
    indices = INdices;
    textures = TExtures;

    setupMesh();
}

void ass_mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    //make vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ass_vertex), &vertices[0], GL_STATIC_DRAW);

    //make indices buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    //make VAO data
    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ass_vertex), (void*)0);
    // vertex texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ass_vertex), (void*)offsetof(ass_vertex, TexCoords));
    // vertex normals
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(ass_vertex), (void*)offsetof(ass_vertex, Normal));
    
    glBindVertexArray(0);
}

void ass_mesh::draw(shader& ourShader)
{
    ourShader.setBool("assimp", 1);
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);

        ourShader.setInt("aspMaterial." + name + number, i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    ourShader.setBool("assimp", 0);
}

ass_model::ass_model(std::string path)
{
    loadModel(path);
}

void ass_model::draw(shader& ourShader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].draw(ourShader);
}

void ass_model::loadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void ass_model::processNode(aiNode* node, const aiScene* scene)
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

ass_mesh ass_model::processMesh(aiMesh* mesh, const aiScene* scene)
{

    std::vector<ass_vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<ass_texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        ass_vertex vertex;

        glm::vec3 vec;
        vec.x = mesh->mVertices[i].x;
        vec.y = mesh->mVertices[i].y;
        vec.z = mesh->mVertices[i].z;
        vertex.Position = vec;

        vec.x = mesh->mNormals[i].x;
        vec.y = mesh->mNormals[i].y;
        vec.z = mesh->mNormals[i].z;
        vertex.Normal = vec;

        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec2;
            vec2.x = mesh->mTextureCoords[0][i].x;
            vec2.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec2;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // process material
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<ass_texture> diffuseMaps = 
            loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<ass_texture> specularMaps = 
            loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return ass_mesh(vertices, indices, textures);
}

std::vector<ass_texture> ass_model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<ass_texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            ass_texture texture;
            texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); // add to loaded textures
        }
    }
    return textures;
}