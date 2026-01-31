#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ogl_presets.h"

class camera
{
public:
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 direction = glm::vec3(1.0f);

    float cam_speed = 0.0f;
    float delta_time = 0.0f;
    float last_frame = 0.0f;
    float current_frame = 0.0f;

    void run(GLFWwindow* window);
    void cam_movement(GLFWwindow* window);
    void set_view(glm::mat4& view);
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void set_callback(GLFWwindow* window);