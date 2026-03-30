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
    shader ourShader("shader/vertex.glsl", "shader/fragment.glsl");//in shader
    camera cam1;//in cam
    set_callback(ourWindow.window, cam1);//in cam

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 trans = glm::mat4(1.0f);

    mesh cubeMesh;
    cubeMesh.buffer();

    ass_model mesh1("models/yae_miko.glb");
    
    glm::mat4 transform1 = glm::mat4 (1);
    //transform1 = glm::rotate(transform1, glm::radians(90.0f), glm::vec3(1, 0, 0));
    transform1 = glm::scale(transform1, glm::vec3(2));
    mesh1.modelTransform = transform1;

    //texture texture1(1, "textures/container2.png");
    //texture texture2(2, "textures/container2_specular.png");
    //texture texture3(3, "textures/wall.jpg");

    //object cube1;
    ////set coords
    //cube1.p.clear();
    //for (int i = -5; i <= 5; i++)
    //{
    //    for(int j = -5; j <= 5; j++)
    //    {
    //        cube1.p.push_back(glm::vec3(i, -2, j));
    //        cube1.p.push_back(glm::vec3(i, +2, j));
    //    }
    //}
    ////set bools
    //cube1.useDiffTex = 1;
    //cube1.useSpecTex = 1;
    ////set tex
    //cube1.material.diffTex = &texture1;
    //cube1.material.specTex = &texture2;
    ////set mesh
    //cube1.objMesh = &cubeMesh;
    ////set draw type
    //cube1.drawWorld = &simpleWorldDraw;

    object cube2;
    cube2.p[0] = glm::vec3(0.0f, 0.0f, 4.0f);
    cube2.objMesh = &cubeMesh;
    cube2.material.mainVec = glm::vec3(1);
    cube2.material.ambVec = glm::vec3(1);
    cube2.material.diffVec = glm::vec3(1);
    cube2.material.specVec = glm::vec3(1);
    cube2.drawWorld = &simpleWorldDraw;

    //object cube3;
    //cube3.p.clear();
    //for (int i = -5; i <= +5; i++)
    //{
    //    for(int j = -1; j <= +1; j++)
    //    {
    //        cube3.p.push_back(glm::vec3(i, j, 5));
    //        cube3.p.push_back(glm::vec3(i, j, -5));
    //        cube3.p.push_back(glm::vec3(5, j, i));
    //        cube3.p.push_back(glm::vec3(-5, j, i));
    //    }
    //}
    //cube3.useDiffTex = 1;
    ////set tex
    //cube3.material.diffTex = &texture3;
    ////set mesh
    //cube3.objMesh = &cubeMesh;
    ////set draw type
    //cube3.drawWorld = &simpleWorldDraw;
    //cube3.material.specVec = glm::vec3(0.2, 0.2, 0.2);

    //object cube4;
    //cube4.p.clear();
    //for (int i = 0; i < cubePositions.size(); i++)
    //{
    //    cube4.p.push_back(cubePositions[i] + glm::vec3(0, 0, 2));
    //    cube4.p.push_back(-(cubePositions[i] + glm::vec3(0, 0, 2)));
    //}
    //cube4.useDiffTex = 1;
    //cube4.useSpecTex = 1;
    ////set tex
    //cube4.material.diffTex = &texture1;
    //cube4.material.specTex = &texture2;
    ////set mesh
    //cube4.objMesh = &cubeMesh;
    ////set draw type
    //cube4.drawWorld = &simpleWorldDraw;

    flashLgt flashLight;
    posLgt posLight1;
    posLgt posLight2;
    dirLgt dirLight;

    light light1;
    light1.flashLight = &flashLight;
    light1.lightType = 2;
    cam1.camLight = light1.flashLight;

    light light2;
    light2.dirLight = &dirLight;
    light2.lightType = 1;
    
    stbi_set_flip_vertically_on_load(true);

    ourShader.use();

    float bgred = 0.0f;
    float bggreen = 0.0f;
    float bgblue = 0.0f;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    
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

        //light1.run(ourShader);
        light2.run(ourShader);

        //glm::mat4 assimpModel = glm::mat4(1.0f);
        //ourShader.setMat4("model", assimpModel);
        mesh1.draw(ourShader);
        //light2.run(ourShader);
        //light3.run(ourShader);

        cube2.draw(model, ourShader, cam1);
        //cube4.draw(model, ourShader, cam1);
        //cube3.draw(model, ourShader, cam1);
        //cube1.draw(model, ourShader, cam1);

        glfwSwapBuffers(ourWindow.window);
        glfwPollEvents();
    }
    glDeleteProgram(ourShader.ID);
    glfwTerminate();
    return 0;
}