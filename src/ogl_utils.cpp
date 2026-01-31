#include"../include/ogl_utils.h"

initWindow::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "\nFailed to create GLFW window\n" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    GLFWmonitor* primary = glfwGetPrimaryMonitor();

    // 2. Get the video mode of that monitor
    const GLFWvidmode* mode = glfwGetVideoMode(primary);

    // 3. Store the values
    SCR_WIDTH = mode->width;
    SCR_HEIGHT = mode->height;

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "\nFailed to initialize GLAD\n" << std::endl;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
    return;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    return;
}

void makeBlue(GLFWwindow *window, float &red, float &green, float &blue)
{
    if(glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        red = 0.0f;
        green = 0.0f;
        blue = 1.0f;
    }
    return;
}

buffer::buffer(object obj)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, obj.v.size() * sizeof(float), obj.v.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    ////glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);
}

void buffer::del()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}