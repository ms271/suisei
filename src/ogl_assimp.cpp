#include "../include/ogl_assimp.h"

std::vector<ass_texture> textures_loaded;

unsigned int TextureFromAssimp(const aiTexture* aiTex)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    int width, height, nrComponents;
    unsigned char* data = nullptr;

    // Assimp quirk: If mHeight is 0, the texture is compressed (e.g., a PNG/JPEG in memory)
    // mWidth is actually the file size in bytes!
    if (aiTex->mHeight == 0)
    {
        data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(aiTex->pcData), aiTex->mWidth, &width, &height, &nrComponents, 4);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failed to load embedded texture from memory!" << std::endl;
        }
    }
    else
    {
        // Uncompressed raw ARGB array (less common, but good to handle)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, aiTex->mWidth, aiTex->mHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, aiTex->pcData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // Set texture wrapping to GL_REPEAT (default)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return textureID;
}

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 4);
    if (data)
    {
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
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

bool hasSignificantTransforms(aiNode* node) {
    // 1. If this node isn't an Identity Matrix (1s on diagonal, 0s elsewhere), it has transforms.
    if (!node->mTransformation.IsIdentity()) {
        return true;
    }

    // 2. Recursively check all children to see if ANY part of the car is offset.
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        if (hasSignificantTransforms(node->mChildren[i])) {
            return true;
        }
    }

    return false;
}

ass_mesh::ass_mesh
(
    std::vector<ass_vertex>& VErtices,
    std::vector<unsigned int>& INdices,
    std::vector<ass_texture>& TExtures,
    float SHininess
)
{
    vertices = VErtices;
    indices = INdices;
    textures = TExtures;
    shininess = SHininess;

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
    // vertex colors
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(ass_vertex), (void*)offsetof(ass_vertex, Color));

    glBindVertexArray(0);
}

void ass_mesh::draw(shader& ourShader, glm::mat4& worldTransform)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    if(textures.size() == 0) ourShader.setBool("alp", false);
    else ourShader.setBool("alp", true);

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
        {
            ourShader.setBool("aspSpec", aspSpec);
            number = std::to_string(specularNr++);
        }

        ourShader.setInt("aspMaterial." + name + number, i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glm::mat4 modelTrans = Transform * worldTransform;
    
    ourShader.setMat4("model", modelTrans);

    glm::mat3 normMatrix = glm::mat3(glm::transpose(glm::inverse(Transform)));
    ourShader.setMat3("normMatrix", normMatrix);

    ourShader.setFloat("specularExponent", this->shininess);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

ass_model::ass_model(std::string path)
{
    loadModel(path);
}

void ass_model::draw(shader& ourShader)
{
    ourShader.use();
    ourShader.setBool("alp", asp);
    ourShader.setBool("assimp", 1);
    
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].draw(ourShader, modelTransform);

    ourShader.setBool("assimp", 0);
}

void ass_model::loadModel(std::string path)
{
    Assimp::Importer import;
    
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace /*| aiProcess_PreTransformVertices */ );
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    glm::mat4 initTransform = glm::mat4(1);

    processNode(scene->mRootNode, scene, initTransform);
}

void ass_model::processNode(aiNode* node, const aiScene* scene, glm::mat4& parentTransform)
{
    glm::mat4 localTransform = aiToGlm(node->mTransformation);
    
    glm::mat4 globalNodeTransform = parentTransform * localTransform;

    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ass_mesh newMesh = processMesh(mesh, scene);
        newMesh.Transform = globalNodeTransform;
        meshes.push_back(std::move(newMesh));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, globalNodeTransform);
    }
}

ass_mesh ass_model::processMesh(aiMesh* mesh, const aiScene* scene)
{

    std::vector<ass_vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<ass_texture> textures;
    float extractedShininess = 32.0f;
    
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // process textures
    
    glm::vec4 baseMaterialColor = glm::vec4(1.0f);

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        aiColor4D aiColor;

        if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor))
        {
            baseMaterialColor = glm::vec4(aiColor.r, aiColor.g, aiColor.b, aiColor.a);
        }

        if (AI_SUCCESS == material->Get(AI_MATKEY_SHININESS, extractedShininess))
        {
            // 3. Protect your shader! Never let shininess hit exactly 0.0
            if (extractedShininess < 2.0f) {
                extractedShininess = 2.0f;
            }
        }

        std::vector<ass_texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<ass_texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", scene);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        std::vector<ass_texture> opacityMaps = loadMaterialTextures(material, aiTextureType_OPACITY, "texture_opacity", scene);
        textures.insert(textures.end(), opacityMaps.begin(), opacityMaps.end());

        std::cout<<opacityMaps.size();
    }

    //process vertices
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

        if (mesh->mColors[0])
        {
            // Priority 2: Use the spray-painted Vertex Color!
            vertex.Color = glm::vec4(mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b, mesh->mColors[0][i].a);
        }
        else
        {
            // Priority 3: Fall back to the Material's Base Color! 
            // (If the artist didn't set this either, it safely defaults to the white we set earlier)
            vertex.Color = glm::vec4(baseMaterialColor.r, baseMaterialColor.g, baseMaterialColor.b, baseMaterialColor.a);
        }

        vertices.push_back(vertex);
    }

    return ass_mesh(vertices, indices, textures, extractedShininess);
}

std::vector<ass_texture> ass_model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName, const aiScene* scene)
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

            const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());
            if (embeddedTexture)
            {
                // It is a *0 GLB file! Load it from RAM!
                texture.id = TextureFromAssimp(embeddedTexture);
            }
            else
            {
                // It is a normal file path. Load from hard drive!
                texture.id = TextureFromFile(str.C_Str(), directory, 0);
            }

            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); // add to loaded textures
        }
    }
    return textures;
}