#pragma once

#include "ogl_presets.h"

void check_error(unsigned int& param, std::string type);
void processInput(GLFWwindow* window);
void makeBlue(GLFWwindow* window, float& red, float& green, float& blue);

class initWindow
{
public:
    GLFWwindow* window;
    initWindow();
};

