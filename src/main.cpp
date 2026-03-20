#define STB_IMAGE_IMPLEMENTATION

#include "../include/ogl_presets.h"
#include "../include/ogl_utils.h"
#include "../include/ogl_shader.h"
#include "../include/ogl_texture.h"
#include "../include/ogl_cam.h"
#include "../include/ogl_object.h"
#include "../include/ogl_draw.h"

int main()
{
    initWindow ourWindow;//in util
    shader ourShader("shader/vertex.glsl", "shader/fragment.glsl");//in shader
    camera cam1;//in cam
    set_callback(ourWindow.window, cam1);//in cam

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
    texture texture3(3, "textures/wall.jpg");

    object cube1;
    //set coords
    cube1.p.clear();
    for (int i = -5; i <= 5; i++)
    {
        for(int j = -5; j <= 5; j++)
        {
            cube1.p.push_back(glm::vec3(i, -2, j));
            cube1.p.push_back(glm::vec3(i, +2, j));
        }
    }
    //set bools
    cube1.useDiffTex = 1;
    cube1.useSpecTex = 1;
    //set tex
    cube1.material.diffTex = &texture1;
    cube1.material.specTex = &texture2;
    //set mesh
    cube1.objMesh = &cubeMesh;
    //set draw type
    cube1.drawWorld = &simpleWorldDraw;

    object cube2;
    cube2.p[0] = glm::vec3(10.0f, 0.0f, 0.0f);
    cube2.objMesh = &cubeMesh;
    cube2.material.mainVec = glm::vec3(1);
    cube2.material.ambVec = glm::vec3(1);
    cube2.material.diffVec = glm::vec3(1);
    cube2.material.specVec = glm::vec3(1);

    cube2.flatShade = 1;
    cube2.drawWorld = &simpleWorldDraw;

    object cube4;
    cube4.p[0] = glm::vec3(0.0f, 10.0f, 0.0f);
    cube4.objMesh = &cubeMesh;
    cube4.material.mainVec = glm::vec3(1);
    
    cube4.flatShade = 1;
    cube4.drawWorld = &simpleWorldDraw;

    object cube3;
    cube3.p.clear();
    for (int i = -5; i <= +5; i++)
    {
        for(int j = -1; j <= +1; j++)
        {
            cube3.p.push_back(glm::vec3(i, j, 5));
            cube3.p.push_back(glm::vec3(i, j, -5));
            cube3.p.push_back(glm::vec3(5, j, i));
            cube3.p.push_back(glm::vec3(-5, j, i));
        }
    }
    cube3.useDiffTex = 1;
    //set tex
    cube3.material.diffTex = &texture3;
    //set mesh
    cube3.objMesh = &cubeMesh;
    //set draw type
    cube3.drawWorld = &simpleWorldDraw;
    cube3.material.specVec = glm::vec3(0.2, 0.2, 0.2);

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

    //light light1;
    //light1.flashLight = &flashLight;
    //light1.lightType = 2;
    //cam1.camLight = light1.flashLight;
    //
    light light2;
    light2.posLight = &posLight1;
    light2.index = 0;
    light2.posLight->position = cube2.p[0];

    light light3;
    light3.posLight = &posLight2;
    light3.index = 1;
    light3.posLight->position = cube4.p[0];

    stbi_set_flip_vertically_on_load(true);

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

        //light1.run(ourShader);
        light2.run(ourShader);
        light3.run(ourShader);

        cube2.draw(model, ourShader, cam1);
        cube4.draw(model, ourShader, cam1);
        cube3.draw(model, ourShader, cam1);
        cube1.draw(model, ourShader, cam1);

        glfwSwapBuffers(ourWindow.window);
        glfwPollEvents();
    }
    glDeleteProgram(ourShader.ID);
    glfwTerminate();
    return 0;
}