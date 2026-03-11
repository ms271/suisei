#include "../include/ogl_object.h"

void object::draw(glm::mat4& model, shader& ourShader, camera& cam, lgt* light, mesh* meshUsed)
{
    meshUsed->bind();
    ourShader.use();

    ourShader.setBool("useTexture", useTexture);
    ourShader.setBool("lightObject", lightObject);
    ourShader.setBool("flatShade", flatShade);
    
    ourShader.setVec3("object_color", color);

    ourShader.setVec3("material.ambient", material.ambient);
    ourShader.setVec3("material.diffuse", material.diffuse);
    ourShader.setVec3("material.specular", material.specular);
    ourShader.setFloat("material.shininess", material.shininess);

    ourShader.setVec3("light.ambient", light->ambient);
    ourShader.setVec3("light.diffuse", light->diffuse);
    ourShader.setVec3("light.specular", light->specular);
    ourShader.setVec3("light.position", light->position);

    if(type == 1)
    {
        for (int i = 0; i < p.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, p[i]);
            float angle = 0;
            float time = glfwGetTime();
            model = glm::rotate(model, glm::radians(angle * time), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);
            glm::mat4 normalMatrix = (glm::transpose(glm::inverse(glm::mat3(model))));
            ourShader.setMat3("normMatrix", normalMatrix);

            glm::vec3 lightColor;
            lightColor.x = sin(glfwGetTime() * 2.0f);
            lightColor.y = sin(glfwGetTime() * 0.7f);
            lightColor.z = sin(glfwGetTime() * 1.3f);

            glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
            glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

            ourShader.setVec3("light.ambient", ambientColor);
            ourShader.setVec3("light.diffuse", diffuseColor);

            glDrawArrays(GL_TRIANGLES, 0, meshUsed->v->size() / meshUsed->vertexSize);            
        }
    }

    else if (type == 0)
    {
        for (int i = 0; i < p.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cam.cameraPos);
            float angle = 0.0f;
            float time = glfwGetTime();
            model = glm::rotate(model, glm::radians(angle * time), glm::vec3(1.0f, 0.3f, 0.5f));
            model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
            ourShader.setMat4("model", model);
            glm::mat4 normalMatrix = (glm::transpose(glm::inverse(glm::mat3(model))));
            ourShader.setMat3("normMatrix", normalMatrix);

            glDrawArrays(GL_TRIANGLES, 0, meshUsed->v->size() / meshUsed->vertexSize);
        }
    }
    meshUsed->unbind();
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

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
