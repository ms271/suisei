#ifndef OGL_UTILS_H
#define OGL_UTILS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void glfw_init(int MAJOR_VERSION, int MINOR_VERSION);
void check_error(unsigned int& param, std::string type);
void processInput(GLFWwindow* window);
void makeBlue(GLFWwindow* window, float& red, float& green, float& blue);

class initWindow
{
public:
    GLFWwindow* window;
    glm::mat4 perspmatrix = glm::mat4 (1.0f);
    void framebufferreset(GLFWwindow* window, int width, int height);
    initWindow(int MAJOR_VERSION, int MINOR_VERSION, int SCR_WIDTH, int SCR_HEIGHT);
};

#endif