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

    object cube1;//in object
    cube1.buffer();
    cube1.useTexture = 1;

    object cube2;
    cube2.p.push_back(glm::vec3(4.0f, 2.0f, -5.0f));
    cube2.buffer();
    cube2.color = glm::vec3(1.0f, 1.0f, 1.0f);
    cube2.useTexture = 0;
    cube2.lightObject = 1;

    stbi_set_flip_vertically_on_load(true);

    texture texture1(1, "textures/collage.jpg");//pointing to texture
    ourShader.use(); 
    ourShader.setVec3("ambient", ambient_light);

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
        ourShader.setMat4("view", view);

        texture1.run(ourShader);

        cube1.draw(model, ourShader);
        cube2.draw(model, ourShader);

        glfwSwapBuffers(ourWindow.window);
        glfwPollEvents();
    }
    cube1.del();
    cube2.del();
    glDeleteProgram(ourShader.ID);
    glfwTerminate();
    return 0;
}