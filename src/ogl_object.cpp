#include "../include/ogl_object.h"

void object::draw(glm::mat4& model, shader& ourShader, camera& cam)
{
    objMesh->bind();
    ourShader.use();

    ourShader.setBool("useDirLight", useDirLight);
    
    ourShader.setBool("flatShade", flatShade);
    if(flatShade)
    {
        ourShader.setBool("useFlatTex", useFlatTex);
        if (useFlatTex) flatTex->run(ourShader, "material.mainTex");
        else ourShader.setVec3("material.mainVec", color);
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
        
        if(!useDirLight)
        {
            ourShader.setVec3("posLight.ambient", posLight->ambient);
            ourShader.setVec3("posLight.diffuse", posLight->diffuse);
            ourShader.setVec3("posLight.specular", posLight->specular);
            ourShader.setVec3("posLight.position", posLight->position);

            ourShader.setFloat("posLight.constant", posLight->constant);
            ourShader.setFloat("posLight.linear", posLight->linear);
            ourShader.setFloat("posLight.quadratic", posLight->quadratic);
        }
        else
        {
            ourShader.setVec3("dirLight.ambient", dirLight->ambient);
            ourShader.setVec3("dirLight.diffuse", dirLight->diffuse);
            ourShader.setVec3("dirLight.specular", dirLight->specular);
            ourShader.setVec3("dirLight.direction", dirLight->direction);
        }

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

void mesh::buffer()
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

void mesh::del()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    return;
}

void mesh::bind()
{
    glBindVertexArray(VAO);
    return;
}
void mesh::unbind()
{
    glBindVertexArray(0);
    return;
}
