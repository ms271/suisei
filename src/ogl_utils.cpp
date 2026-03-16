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

    GLFWmonitor* primary = glfwGetPrimaryMonitor();

    // 2. Get the video mode of that monitor
    const GLFWvidmode* mode = glfwGetVideoMode(primary);

    // 3. Store the values
    SCR_WIDTH = mode->width * window_factor;
    SCR_HEIGHT = mode->height * window_factor;

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Project Suisei", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "\nFailed to create GLFW window\n" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "\nFailed to initialize GLAD\n" << std::endl;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
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
    else
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

bool lastBState;
void makeBlue(GLFWwindow *window, float &red, float &green, float &blue)
{
    bool currentBState = (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS);

    // Only trigger if the key is pressed NOW, but was NOT pressed last frame
    if (currentBState && !lastBState)
    {
        if (!BgColorStatus) {
            BgColorStatus = 1;
            red = 1.0f; green = 0.7f; blue = 0.7f;
        }
        else {
            BgColorStatus = 0;
            red = 0.0f; green = 0.0f; blue = 0.0f;
        }
    }

    lastBState = currentBState;

    return;
}