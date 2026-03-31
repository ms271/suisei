#include "../include/ogl_lighting.h"

void cLight::run(Shader& ourShader)
{
    if (lightType == 0)
    {
        std::string name = "posLight[";
        name += std::to_string(index);

        ourShader.setVec3(name + "].ambient", posLight->ambient);
        ourShader.setVec3(name + "].diffuse", posLight->diffuse);
        ourShader.setVec3(name + "].specular", posLight->specular);
        ourShader.setVec3(name + "].position", posLight->position);

        ourShader.setFloat(name + "].constant", posLight->constant);
        ourShader.setFloat(name + "].linear", posLight->linear);
        ourShader.setFloat(name + "].quadratic", posLight->quadratic);
    }
    else if (lightType == 1)
    {
        ourShader.setVec3("dirLight.ambient", dirLight->ambient);
        ourShader.setVec3("dirLight.diffuse", dirLight->diffuse);
        ourShader.setVec3("dirLight.specular", dirLight->specular);

        ourShader.setVec3("dirLight.direction", dirLight->direction);
    }
    else if (lightType == 2)
    {
        ourShader.setVec3("flashLight.ambient", flashLight->ambient);
        ourShader.setVec3("flashLight.diffuse", flashLight->diffuse);
        ourShader.setVec3("flashLight.specular", flashLight->specular);

        ourShader.setVec3("flashLight.position", flashLight->position);
        ourShader.setVec3("flashLight.direction", flashLight->direction);
        ourShader.setFloat("flashLight.cutOff", flashLight->cutOff);
        ourShader.setFloat("flashLight.cutOff2", flashLight->cutOff2);

        ourShader.setFloat("flashLight.constant", flashLight->constant);
        ourShader.setFloat("flashLight.linear", flashLight->linear);
        ourShader.setFloat("flashLight.quadratic", flashLight->quadratic);
    }
}