#include "../include/ogl_object.h"

void object::buffer()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float), v.data(), GL_STATIC_DRAW);

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

void object::del()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    return;
}

void object::bind()
{
    glBindVertexArray(VAO);
    return;
}
void object::unbind()
{
    glBindVertexArray(0);
    return;
}

void object::draw(glm::mat4& model,  shader& ourShader)
{
    bind();
    ourShader.use();
    ourShader.setBool("useTexture", useTexture);
    ourShader.setVec3("object_color", color);
    ourShader.setBool("lightObject", lightObject);
    for (int i = 0; i < p.size(); i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, p[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        ourShader.setMat4("model", model);
        normalMatrix = (glm::transpose(glm::inverse(glm::mat3(model))));
        ourShader.setMat3("normMatrix", normalMatrix);
        glDrawArrays(GL_TRIANGLES, 0, v.size() / vertexSize);
    }
    unbind();
}

object::object()
{

}
