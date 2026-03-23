#pragma once
#include "ogl_presets.h"
#include "ogl_shader.h"

class posLgt
{
public:
    
    glm::vec3 position = glm::vec3(1.0f, 0.5f, 0.31f);

    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);

    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
};

class flashLgt
{
public:
    glm::vec3 position = glm::vec3(1.0f, 0.5f, 0.31f);
    glm::vec3 direction = glm::vec3(1.0f, 0.5f, 0.31f);
    float cutOff = glm::cos(glm::radians(40.0f));
    float cutOff2 = glm::cos(glm::radians(44.0f));

    glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);

    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
};

class dirLgt
{
public:
    glm::vec3 direction = glm::vec3(1.0f, 0.5f, 0.31f);

    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
};

class light
{
public:
    posLgt* posLight;
    dirLgt* dirLight;
    flashLgt* flashLight;
    int lightType = 0;//0 pos, 1 dir, 2 flash
    int index = 0;

    void run(shader& ourShader);
};