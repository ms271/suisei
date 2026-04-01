#define STB_IMAGE_IMPLEMENTATION

#include "../include/ogl_presets.h"
#include "../include/ogl_utils.h"
#include "../include/ogl_shader.h"
#include "../include/ogl_texture.h"
#include "../include/ogl_cam.h"
#include "../include/ogl_object.h"
#include "../include/ogl_draw.h"
#include "../include/ogl_assimp.h"

int main()
{
    initWindow ourWindow;//in util
    Shader ourShader("shader/vertex.glsl", "shader/fragment.glsl");//in shader
    camera cam1;//in cam
    set_callback(ourWindow.window, cam1);//in cam

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    
    cMesh cubeMesh;
    aModel model1("models/2020-f1-mercedes-benz-w11/source/2020 F1 Mercedes-Benz W11.glb");

    cObject cube1;
    cube1.objMesh = &cubeMesh;
    cube1.drawWorld = &simpleWorldDraw;

    cFlashLgt flashLight;
    
    cLight light1;
    light1.flashLight = &flashLight;
    light1.lightType = 2;
    cam1.camLight = light1.flashLight;
    
    ourShader.use(); 
    
    float bgred = 0.0f;
    float bggreen = 0.0f;
    float bgblue = 0.0f;

    glEnable(GL_DEPTH_TEST);
    
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
        ourShader.setMat4("projection", cam1.PROJ);
        
        light1.run(ourShader);

        model1.Draw(ourShader);
        //cube1.draw(model, ourShader, cam1);

        glfwSwapBuffers(ourWindow.window);
        glfwPollEvents();
    }
    glDeleteProgram(ourShader.ID);
    glfwTerminate();
    return 0;
}