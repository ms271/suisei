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
        shader.setInt("asiTexType[" + std::to_string(i) + "]", textures[i].type);//1 is diffuse, 2 is specular
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
    bool hasTextures = true;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        aVertex vertex;
        // process vertex positions, normals and texture coordinates
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;

        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
        {
            hasTextures = false;
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

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
        
        for(int i = 1; i < 22; i++)
        {
            if (textures.size() == 16)
            {
                std::cout << "texture limit reached";
                break;
            }

            std::vector<aTexture> Maps = loadMaterialTextures(material, (aiTextureType)i, scene);
            textures.insert(textures.end(), Maps.begin(), Maps.end());
        }
    }

    return aMesh(vertices, indices, textures);
}

std::vector<aTexture> aModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const aiScene* scene)
{
    std::vector<aTexture> textures;
    int typeName = -1;

    if (type != (aiTextureType)1 && type != (aiTextureType)2)
    {
        if (mat->GetTextureCount(type) > 0)
        {
            std::cout<<"counts of non supported texture - "<< mat->GetTextureCount(type)<<'\n';
            return textures;
        }
    }
    else
    {
        typeName = (int)type;
    }
    
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;

        std::string name = str.C_Str();

        if (texLoaded.find(name) == texLoaded.end())
        {
            aTexture texture;
            const aiTexture* embedded = scene->GetEmbeddedTexture(str.C_Str());

            if (embedded) {
                // Goal #1: Handle Embedded (like .glb or .fbx)
                texture.id = TextureFromMemory(embedded);
            }
            else {
                // Goal #1: Handle External (standard images)
                texture.id = TextureFromFile(str.C_Str(), directory);
            }

            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            texLoaded[name] = texture;
        }
        else
        {
            textures.push_back(texLoaded[name]);
        }
    }
    return textures;
}

unsigned int TextureFromFile(const char* path, const std::string& directory)
{
    std::string filename = std::string(path);
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
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
    }

    stbi_image_free(data);

    return textureID;
}

unsigned int TextureFromMemory(const aiTexture* embeddedTexture) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = nullptr;

    if (embeddedTexture->mHeight == 0) {
        // Case A: Compressed (PNG/JPG buffer)
        data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(embeddedTexture->pcData),
            embeddedTexture->mWidth, &width, &height, &nrComponents, 0);
    }
    else {
        // Case B: Raw (Uncompressed RGBA)
        // We don't need STB here; we just point OpenGL to the bytes
        width = embeddedTexture->mWidth;
        height = embeddedTexture->mHeight;
        data = reinterpret_cast<unsigned char*>(embeddedTexture->pcData);
    }

    if (data) {
        GLenum format = (nrComponents == 4) ? GL_RGBA : GL_RGB;
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        if (embeddedTexture->mHeight == 0) stbi_image_free(data); // Only free if STB loaded it
    }
    return textureID;
}