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

    object cube1;//in object
    cube1.buffer();

    object cube2;
    cube2.p.push_back(glm::vec3(4.0f, 2.0f, -5.0f));
    cube2.buffer();

    stbi_set_flip_vertically_on_load(true);

    texture texture1(1, "textures/collage.jpg");//in texture
    ourShader.use(); 
    ourShader.setInt("ourTexture1", 0);

    float bgred = 1.0f;
    float bggreen = 0.9f;
    float bgblue = 0.9f;
    glEnable(GL_DEPTH_TEST);
    ourShader.setMat4("projection", PROJ);
    
    while (!glfwWindowShouldClose(ourWindow.window))
    {
        cam1.run(ourWindow.window);
        processInput(ourWindow.window);
        makeBlue(ourWindow.window, bgred , bggreen , bgblue);

        glClearColor(bgred, bggreen, bgblue, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        texture1.run();
        
        ourShader.use();
        cam1.set_view(view);
        ourShader.setMat4("view", view);
        
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