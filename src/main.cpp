#define STB_IMAGE_IMPLEMENTATION

#include "../include/ogl_presets.h"
#include "../include/ogl_utils.h"
#include "../include/ogl_shader.h"
#include "../include/ogl_texture.h"
#include "../include/ogl_cam.h"
#include "../include/ogl_object.h"

int main()
{
    initWindow ourWindow;//in util
    set_callback(ourWindow.window);//in cam
    shader ourShader("shader/vertex.glsl", "shader/fragment.glsl");//in shader
    camera cam1;//in cam

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 trans = glm::mat4(1.0f);

    std::vector<glm::vec3> cubePositions = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    mesh cubeMesh;
    cubeMesh.buffer();

    texture texture1(1, "textures/container2.png");
    texture texture2(2, "textures/container2_specular.png");

    object cube1;//in object
    cube1.useDiffTex = 1;
    cube1.useSpecTex = 1;
    cube1.material.diffTex = &texture1;
    cube1.material.specTex = &texture2;
    cube1.objMesh = &cubeMesh;

    object cube2;
    cube2.p[0] = glm::vec3(12.0f, 15.0f, -6.0f);
    cube2.objMesh = &cubeMesh;
    cube2.lightObject = 1;
    cube2.flatShade = 1;
    lgt light1;
    light1.position = cube2.p[0];

    stbi_set_flip_vertically_on_load(true);

    ourShader.use(); 
    
    float bgred = 0.0f;
    float bggreen = 0.0f;
    float bgblue = 0.0f;
    glEnable(GL_DEPTH_TEST);
    ourShader.setMat4("projection", PROJ);
    
    while (!glfwWindowShouldClose(ourWindow.window))
    {
        cam1.run(ourWindow.window);
        processInput(ourWindow.window);
        makeBlue(ourWindow.window, bgred , bggreen , bgblue);

        glClearColor(bgred, bggreen, bgblue, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        ourShader.use();
        cam1.set_view(view);
        ourShader.setVec3("camPos", cam1.cameraPos);
        ourShader.setMat4("view", view);

        cube1.draw(model, ourShader, cam1, &light1);
        cube2.draw(model, ourShader, cam1, &light1);
        
        glfwSwapBuffers(ourWindow.window);
        glfwPollEvents();
    }
    cubeMesh.del();
    glDeleteProgram(ourShader.ID);
    glfwTerminate();
    return 0;
}