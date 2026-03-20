#include "../include/ogl_cam.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    camera* cam = static_cast<camera*>(glfwGetWindowUserPointer(window));

    glViewport(0, 0, width, height);
    cam->PROJ = glm::perspective(glm::radians(cam->FOV), ((float)width / (float)height), 0.1f, 100.0f);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    camera* cam = static_cast<camera*>(glfwGetWindowUserPointer(window));
    
    if (cam->FIRST_MOUSE)
    {
        cam->LAST_X = xPos;
        cam->LAST_Y = yPos;
        cam->FIRST_MOUSE = false;
    }

    float xoffset = xPos - cam->LAST_X;
    float yoffset = cam->LAST_Y - yPos;
    cam->LAST_X = xPos;
    cam->LAST_Y = yPos;

    float SENSITIVITY = 0.1f;
    xoffset *= SENSITIVITY;
    yoffset *= SENSITIVITY;

    cam->YAW += xoffset;
    cam->PITCH += yoffset;

    if (cam->PITCH > 89.0f) cam->PITCH = 89.0f;
    if (cam->PITCH < -89.0f) cam->PITCH = -89.0f;

    return;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera* cam = static_cast<camera*>(glfwGetWindowUserPointer(window));

    cam->FOV -= (float)yoffset;
    if (cam->FOV < 1.0f) cam->FOV = 1.0f;
    if (cam->FOV > 45.0f) cam->FOV = 45.0f;

    cam->PROJ = glm::perspective(glm::radians(cam->FOV), ((float)SCR_WIDTH / (float)SCR_HEIGHT), 0.1f, 100.0f);

    return;
}

void set_callback(GLFWwindow* window, camera& cam)
{
    glfwSetWindowUserPointer(window, &cam);
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
        cameraPos += cam_speed * glm::normalize(glm::cross(glm::cross(cameraFront, cameraUp), cameraFront));
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        cameraPos -= cam_speed * glm::normalize(glm::cross(glm::cross(cameraFront, cameraUp), cameraFront));
    }
}

void camera::run(GLFWwindow* window)
{
    current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;
    cam_speed = CAM_BASE_SPEED * delta_time;

    camDirection.x = cos(glm::radians(YAW)) * cos(glm::radians(PITCH));
    camDirection.y = sin(glm::radians(PITCH));
    camDirection.z = sin(glm::radians(YAW)) * cos(glm::radians(PITCH));
    cameraFront = glm::normalize(camDirection);

    if(camLight != nullptr)
    {
        camLight->position = cameraPos;
        camLight->direction = camDirection;
    }
    
    cam_movement(window);
    return;
}

void camera::set_view(glm::mat4& view)
{
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    return;
}