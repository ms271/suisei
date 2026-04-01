#include "../include/ogl_object.h"

void cObject::draw(glm::mat4& model, Shader& ourShader, camera& cam)
{
    objMesh->bind();
    ourShader.use();
    ourShader.setBool("assimp", false);

    ourShader.setBool("flatShade", flatShade);
    if(flatShade)
    {
        ourShader.setBool("useFlatTex", useFlatTex);
        if (useFlatTex) flatTex->run(ourShader, "material.mainTex");
        else ourShader.setVec3("material.mainVec", material.mainVec);
    }
    else
    {
        ourShader.setBool("useSpecTex", useSpecTex);
        if (useSpecTex) material.specTex->run(ourShader, "material.specTex");
        else ourShader.setVec3("material.specVec", material.specVec);

        ourShader.setBool("useMainTex", useMainTex);
        if (useMainTex) material.mainTex->run(ourShader, "material.mainTex");
        else ourShader.setVec3("material.mainVec", material.mainVec);

        ourShader.setBool("useDiffTex", useDiffTex);
        if (useDiffTex) material.diffTex->run(ourShader, "material.diffTex");
        else ourShader.setVec3("material.diffVec", material.diffVec);

        ourShader.setVec3("material.ambVec", material.ambVec);
        ourShader.setFloat("material.shininess", material.shininess);
    }
    
    if(type == 1)
    {
        drawWorld(model, ourShader, objMesh, p);
    }

    else if (type == 0)
    {
        drawHUD(model, ourShader, objMesh, p, cam);
    }
    objMesh->unbind();
}

void cMesh::buffer()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, v->size() * sizeof(float), v->data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);//positions

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);//texture coordinates

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);//normals

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return;
}

cMesh::~cMesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void cMesh::bind()
{
    glBindVertexArray(VAO);
    return;
}
void cMesh::unbind()
{
    glBindVertexArray(0);
    return;
}

cMesh::cMesh(const std::vector<float>& vertices)
{
    v = &vertices;
    buffer();
}
