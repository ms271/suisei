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

    stbi_set_flip_vertically_on_load(true);

    ogl_texture texture1(1, "textures/collage.jpg");//in texture
    ourShader.use(); 
    ourShader.setInt("ourTexture1", 0);

    float bgred = 1.0f;
    float bggreen = 0.9f;
    float bgblue = 0.9f;
    glEnable(GL_DEPTH_TEST);
    
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
        cube1.bind();
        
        for(int i = 0; i < cube1.p.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cube1.p[i]);
            model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
            trans = PROJ * view * model;
            ourShader.setMat4("transform", trans);
            glDrawArrays(GL_TRIANGLES, 0, cube1.v.size() / 5);
        }

        glfwSwapBuffers(ourWindow.window);
        glfwPollEvents();
    }
    cube1.del();
    glDeleteProgram(ourShader.ID);
    glfwTerminate();
    return 0;
}