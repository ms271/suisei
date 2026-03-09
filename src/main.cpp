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
    shader ourShader("shader/vertex.vs", "shader/fragment.fs");//in shader
    camera cam1;//in cam

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 trans = glm::mat4(1.0f);

    glm::vec3 ambient_light = glm::vec3(1.0f, 1.0f, 1.0f);
    ambient_light *= 0.1;

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

    object cube1;//in object
    cube1.useTexture = 1;
    /*cube1.shinyMulti = 0.8f;
    cube1.shinyExp = 128;
    cube1.color = glm::vec3(0.6, 0.4, 0.8);*/

    object cube2;
    cube2.p[0] = glm::vec3(4.0f, 0.0f, -2.0f);
    cube2.color = glm::vec3(0.0f, 0.7f, 1.0f);
    cube2.lightObject = 1;

    object cube3;
    cube3.p[0] = glm::vec3(2.0f, 0.0f, -2.0f);
    cube3.useTexture = 1;

    object cube4;
    cube4.p[0] = glm::vec3(-3.0, 1.0, -2.5f);
    cube4.color = glm::vec3(0.8f, 0.5f, 0.3f);
    cube4.shinyMulti = 0.8f;
    cube4.shinyExp = 128;

    //object cube5 ("hud");
    //cube5.useTexture = 1;
    //cube5.flatShade = 1;

    stbi_set_flip_vertically_on_load(true);

    texture texture1(1, "textures/HnWyn.png");//pointing to texture
    texture texture2(2, "textures/wall.jpg");
    //texture texture3(3, "textures/image.jpg");

    ourShader.use(); 
    ourShader.setVec3("ambient", ambient_light);
    ourShader.setVec3("lightPos", cube2.p[0]);
    ourShader.setVec3("lightColor", cube2.color);

    float bgred = 0.0f;
    float bggreen = 0.0f;
    float bgblue = 0.0f;
    glEnable(GL_DEPTH_TEST);
    ourShader.setMat4("projection", PROJ);

    cubeMesh.bind();
    
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

        texture1.run(ourShader);

        cube1.draw(model, ourShader, cam1);
        cube2.draw(model, ourShader, cam1);

        texture2.run(ourShader);

        cube3.draw(model, ourShader, cam1);
        cube4.draw(model, ourShader, cam1);

        //texture3.run(ourShader);

        //cube5.draw(model, ourShader, cam1);

        glfwSwapBuffers(ourWindow.window);
        glfwPollEvents();
    }
    cubeMesh.unbind();
    cubeMesh.del();
    glDeleteProgram(ourShader.ID);
    glfwTerminate();
    return 0;
}