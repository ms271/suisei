#include"../include/ogl_utils.h"

initWindow::initWindow(int MAJOR_VERSION, int MINOR_VERSION, int SCR_WIDTH, int SCR_HEIGHT)
{
    glfw_init(MAJOR_VERSION, MINOR_VERSION);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "\nFailed to create GLFW window\n" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "\nFailed to initialize GLAD\n" << std::endl;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


}

void initWindow::framebufferreset(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    float aspectRatio = (float)width / (float)height;
    perspmatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void glfw_init(int MAJOR_VERSION, int MINOR_VERSION)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
}

void check_error(unsigned int & param, std::string type)
{
    int success;
    char infoLog[1024];

    if (type == "program") // Check if we are validating the PROGRAM
    {
        glGetProgramiv(param, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(param, 1024, NULL, infoLog);
            std::cout << "\nERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- \n";
        }
    }
    else if(type == "shader") // Otherwise, we are validating a SHADER
    {
        glGetShaderiv(param, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(param, 1024, NULL, infoLog);
            std::cout << "\nERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- \n";
        }
    }
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void makeBlue(GLFWwindow *window, float &red, float &green, float &blue)
{
    if(glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        red = 0.0f;
        green = 0.0f;
        blue = 1.0f;
    }
}

void cam_movement(GLFWwindow* window, glm::vec3& cam_Pos, glm::vec3& cam_front, glm::vec3& cam_up, float cam_speed)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cam_Pos += cam_speed * glm::normalize(cam_front);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cam_Pos -= cam_speed * glm::normalize(cam_front);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cam_Pos += cam_speed * glm::normalize(glm::cross(cam_front, cam_up));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cam_Pos -= cam_speed * glm::normalize(glm::cross(cam_front, cam_up));
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        cam_Pos += cam_speed * glm::normalize(cam_up);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        cam_Pos -= cam_speed * glm::normalize(cam_up);
    }
}