#include"../include/ogl_assimp.h"

aMesh::aMesh(std::vector<aVertex> VErtices, std::vector<unsigned int> INdices, std::vector<aTexture> TExtures, aMaterial MAterial)
{
    this->vertices = VErtices;
    this->indices = INdices;
    this->textures = TExtures;
    this->material = MAterial;
    
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

void aMesh::Draw(Shader& shader, glm::mat4& worldTransform)
{
//textures and materials have been preloaded into their respective ids, what we have to do is bind them at their respective postions
    
    shader.setVec3("asiMaterial.base", material.baseColor);
    shader.setVec3("asiMaterial.ambient", material.ambientColor);
    shader.setVec3("asiMaterial.diffuse", material.diffuseColor);
    shader.setVec3("asiMaterial.specular", material.specularColor);
    shader.setFloat("asiMaterial.specularStrength", material.specularStrength);
    shader.setFloat("asiMaterial.specularExponent", material.specularExponent);
    
    shader.setInt("asiNumOfTextures", textures.size());

    for (int i = 0; i < textures.size(); i++)
    {
        if(i > 12) break;
        glActiveTexture(GL_TEXTURE0 + i);
        shader.setInt("asiTexture[" + std::to_string(i) + "]", i);
        
        if(textures[i].type == 12) textures[i].type = 1;
        
        shader.setInt("asiTexType[" + std::to_string(i) + "]", textures[i].type);//1 is diffuse, 2 is specular
        //shader.setFloat("asiTexBlend[" + std::to_string(i) + "]", textures[i].blend);
        //shader.setFloat("asiTexBlendOp[" + std::to_string(i) + "]", textures[i].blendOp);

        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    glm::mat4 modelTrans = Transform * worldTransform;

    shader.setMat4("model", modelTrans);

    glm::mat3 normMatrix = glm::mat3(glm::transpose(glm::inverse(Transform)));
    shader.setMat3("normMatrix", normMatrix);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

aModel::aModel(std::string path)
{
    loadModel(path);
}

void aModel::Draw(Shader& shader)
{
    shader.setBool("assimp", true);
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader, modelTransform);

    shader.setBool("assimp", false);
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
    processNode(scene->mRootNode, scene, modelTransform);
}

void aModel::processNode(aiNode* node, const aiScene* scene, glm::mat4 parentTransform)
{
    glm::mat4 localTransform = aiToGlm(node->mTransformation);

    glm::mat4 globalNodeTransform = parentTransform * localTransform;

    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        aMesh newMesh = processMesh(mesh, scene);
        newMesh.Transform = globalNodeTransform;
        meshes.push_back(std::move(newMesh));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, globalNodeTransform);
    }
}

aMesh aModel::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<aVertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<aTexture> textures;
    aMaterial meshMat;
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

        if (mesh->HasVertexColors(0))
        {
            // Assimp stores colors as aiColor4D (RGBA), so we extract RGB
            vertex.Color.x = mesh->mColors[0][i].r;
            vertex.Color.y = mesh->mColors[0][i].g;
            vertex.Color.z = mesh->mColors[0][i].b;
        }
        else
        {
            // The model has no vertex colors. Default to pure white so the lighting math doesn't multiply by zero!
            vertex.Color = glm::vec3(1.0f, 1.0f, 1.0f);
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
            if (textures.size() == 12)
            {
                std::cout << "texture limit reached";
                break;
            }

            std::vector<aTexture> Maps = loadMaterialTextures(material, (aiTextureType)i, scene);
            if(Maps.size() != 0)
            textures.insert(textures.end(), Maps.begin(), Maps.end());
        }
        std::cout <<"size - " << textures.size() << '\n';
        //base----------------------------------------------------------------------------------------------------------
        aiColor3D aiBase(1.0f, 1.0f, 1.0f);

        if (material->Get(AI_MATKEY_BASE_COLOR, aiBase) != AI_SUCCESS)
        {
            std::cout << "[Material Warning]: No Base color found. Defaulting to white.\n";
        }

        meshMat.baseColor = glm::vec3(aiBase.r, aiBase.g, aiBase.b);
        if (meshMat.baseColor == glm::vec3(0))
        {
            std::cout<<"\nbaseColor is 0, set to 1\n";
            meshMat.baseColor = glm::vec3(1);
            aiBase.r = 1;aiBase.g = 1;aiBase.b = 1;
        }

        //Diffuse-------------------------------------------------------------------------------------------------------------------
        aiColor3D aiDiffuse(aiBase.r, aiBase.g, aiBase.b);

        if (material->Get(AI_MATKEY_COLOR_DIFFUSE, aiDiffuse) != AI_SUCCESS)
        {
            std::cout << "[Material Warning]: No diffuse color found. Defaulting to base.\n";
        }

        if (meshMat.diffuseColor == glm::vec3(0))
        {
            std::cout << "\ndiffuseColor is 0, set to 1\n";
            meshMat.diffuseColor = glm::vec3(1);
            aiDiffuse.r = 1;aiDiffuse.g = 1;aiDiffuse.b = 1;
        }

        meshMat.diffuseColor = glm::vec3(aiDiffuse.r, aiDiffuse.g, aiDiffuse.b);

        //ambient-------------------------------------------------------------------------------------------------------------------
        aiColor3D aiAmbient(aiDiffuse.r, aiDiffuse.g, aiDiffuse.b);

        if (material->Get(AI_MATKEY_COLOR_AMBIENT, aiAmbient) != AI_SUCCESS)
        {
            std::cout << "[Material Warning]: No Ambient color found. Defaulting to diffuse.\n";
        }
        meshMat.ambientColor = glm::vec3(aiAmbient.r, aiAmbient.g, aiAmbient.b);

        if (meshMat.ambientColor == glm::vec3(0))
        {
            std::cout << "\nambientColor is 0, set to 1\n";
            meshMat.ambientColor = glm::vec3(1);
            aiAmbient.r = 1;aiAmbient.g = 1;aiAmbient.b = 1;
        }

        //specular----------------------------------------------------------------------------------------------------------
        aiColor3D aiSpecular(0.0f, 0.0f, 0.0f);

        if (material->Get(AI_MATKEY_COLOR_SPECULAR, aiSpecular) != AI_SUCCESS)
        {
            std::cout << "[Material Warning]: No Specular color found. Defaulting to black.\n";
        }

        meshMat.specularColor = glm::vec3(aiSpecular.r, aiSpecular.g, aiSpecular.b);

        //shinyStrenth----------------------------------------------------------------------------------------------------------
        float shinyStrength = 1.0f;

        if (material->Get(AI_MATKEY_SHININESS_STRENGTH, shinyStrength) != AI_SUCCESS)
        {
            // Goal #4: Error handling if the model doesn't define a diffuse color
            std::cout << "[Material Warning]: No shinyStrength found. Defaulting to 1.\n";
        }
        meshMat.specularStrength = shinyStrength;
        
        //shinyExponent----------------------------------------------------------------------------------------------------------
        float shinyExponent = 32.0f;

        if (material->Get(AI_MATKEY_SHININESS, shinyExponent) != AI_SUCCESS)
        {
            std::cout << "[Material Warning]: No shinyExponent found. Defaulting to 32.\n";
        }

        if (shinyExponent < 1)
        {
            std::cout << "\nBad shinyExponent value, increasing to 1";
            shinyExponent = 1;
        }
        meshMat.specularExponent = shinyExponent;

    }
    return aMesh(vertices, indices, textures, meshMat);
}

std::vector<aTexture> aModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const aiScene* scene)
{
    std::vector<aTexture> textures;
    int typeName = -1;

    if (type != (aiTextureType)1 && type != (aiTextureType)2 && type != (aiTextureType)12)
    {
        if (mat->GetTextureCount(type) > 0)
        {
            std::cout<<"\ncounts of non supported texture - "<< mat->GetTextureCount(type)<<'\n';
            std::cout<<"texture type - " << type << "\n\n";
            return textures;
        }
    }
    else
    {
        std::cout<<"\n"<<type<<"\n";
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

            float blend = 1.0f; // Default fallback
            
            // AI_MATKEY_TEXBLEND requires both the type AND the specific index (i)
            if (mat->Get(AI_MATKEY_TEXBLEND(type, i), blend) != AI_SUCCESS) {
                blend = 1.0f;
            }

            int blendOp = 0;
            if (mat->Get(AI_MATKEY_TEXOP(type, i), blendOp) != AI_SUCCESS) {
                blendOp = 0;
            }

            texture.blendOp = blendOp;
            texture.blend = blend;
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

glm::mat4 aiToGlm(aiMatrix4x4& from)
{
    glm::mat4 to;
    // [col][row]
    to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
    to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
    to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
    to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
    return to;
}