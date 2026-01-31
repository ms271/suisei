#include "../include/ogl_cam.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    PROJ = glm::perspective(glm::radians(FOV), ((float)width / (float)height), 0.1f, 100.0f);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    if (FIRST_MOUSE)
    {
        LAST_X = xPos;
        LAST_Y = yPos;
        FIRST_MOUSE = false;
    }

    float xoffset = xPos - LAST_X;
    float yoffset = LAST_Y - yPos;
    LAST_X = xPos;
    LAST_Y = yPos;

    float SENSITIVITY = 0.1f;
    xoffset *= SENSITIVITY;
    yoffset *= SENSITIVITY;

    YAW += xoffset;
    PITCH += yoffset;

    if (PITCH > 89.0f) PITCH = 89.0f;
    if (PITCH < -89.0f) PITCH = -89.0f;

    return;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    FOV -= (float)yoffset;
    if (FOV < 1.0f) FOV = 1.0f;
    if (FOV > 45.0f) FOV = 45.0f;
    return;
}

void set_callback(GLFWwindow* window)
{
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
}

void camera::cam_movement(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPos += cam_speed * glm::normalize(cameraFront);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPos -= cam_speed * glm::normalize(cameraFront);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPos += cam_speed * glm::normalize(glm::cross(cameraFront, cameraUp));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPos -= cam_speed * glm::normalize(glm::cross(cameraFront, cameraUp));
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        cameraPos += cam_speed * glm::normalize(cameraUp);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        cameraPos -= cam_speed * glm::normalize(cameraUp);
    }
}

void camera::run(GLFWwindow* window)
{
    current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;
    cam_speed = CAM_BASE_SPEED * delta_time;

    direction.x = cos(glm::radians(YAW)) * cos(glm::radians(PITCH));
    direction.y = sin(glm::radians(PITCH));
    direction.z = sin(glm::radians(YAW)) * cos(glm::radians(PITCH));
    cameraFront = glm::normalize(direction);
    
    cam_movement(window);
    return;
}

void camera::set_view(glm::mat4& view)
{
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    return;
}