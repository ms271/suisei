#include "../include/ogl_object.h"

void object::buffer()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float), v.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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
    for (int i = 0; i < p.size(); i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, p[i]);
        ourShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, v.size() / 5);
    }
    unbind();
}

object::object()
{

}
