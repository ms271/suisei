#include "../include/ogl_draw.h"
#include "../include/ogl_presets.h"
#include "../include/ogl_shader.h"
#include "../include/ogl_object.h"

void simpleWorldDraw(glm::mat4 & model, shader & ourShader, mesh* objMesh, std::vector<glm::vec3>&p)
{
    for (int i = 0; i < p.size(); i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, p[i]);
        ourShader.setMat4("model", model);
        glm::mat4 normalMatrix = (glm::transpose(glm::inverse(glm::mat3(model))));
        ourShader.setMat3("normMatrix", normalMatrix);

        glDrawArrays(GL_TRIANGLES, 0, (objMesh->v->size() / objMesh->vertexSize));
    }
}

void simpleHudDraw(glm::mat4& model, shader& ourShader, mesh* objMesh, std::vector<glm::vec3>& p, camera& cam)
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

        glDrawArrays(GL_TRIANGLES, 0, objMesh->v->size() / objMesh->vertexSize);
    }
}