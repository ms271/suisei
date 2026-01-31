#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ogl_presets.h"
#include "ogl_object.h"

void check_error(unsigned int& param, std::string type);
void processInput(GLFWwindow* window);
void makeBlue(GLFWwindow* window, float& red, float& green, float& blue);

class initWindow
{
public:
    GLFWwindow* window;
    initWindow();
};

class buffer
{
public:
    unsigned int VBO, VAO;
    buffer(object obj);
    void del();
};