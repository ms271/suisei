#pragma once

#include "ogl_presets.h"
#include "ogl_lighting.h"

class cFlashLgt;

class camera
{
public:
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 camDirection = glm::vec3(1.0f);

    float cam_speed = 0.0f;
    float delta_time = 0.0f;
    float last_frame = 0.0f;
    float current_frame = 0.0f;

    float YAW = -90.0f;
    float PITCH = 0.0f;
    float FOV = 45.0f;

    float LAST_X = SCR_WIDTH / 2;
    float LAST_Y = SCR_HEIGHT / 2;

    const float SENSITIVITY = 0.01f;
    float CAM_BASE_SPEED = 2.0f;

    bool FIRST_MOUSE = 1;

    glm::mat4 PROJ = glm::perspective(glm::radians(FOV), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);;


    cFlashLgt* camLight;

    void run(GLFWwindow* window);
    void cam_movement(GLFWwindow* window);
    void set_view(glm::mat4& view);
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void set_callback(GLFWwindow* window, camera& cam);