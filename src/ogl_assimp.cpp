#include"../include/ogl_assimp.h"

aMesh::aMesh(std::vector<aVertex> vertices, std::vector<unsigned int> indices, std::vector<aTexture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

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
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(aVertex), (void*)offsetof(aVertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(aVertex), (void*)offsetof(aVertex, TexCoords));

    glBindVertexArray(0);
}