#include"../include/ogl_test.h"

void testScene::run()
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 trans = glm::mat4(1.0f);

    cMesh cubeMesh;

    //aModel yae("models/yae_miko2.glb");
    //yae.modelTransform = yaeTranslate;

    //aModel ei("models/genshin_impact_-_raiden_ei_boss.glb");
    //yaeTranslate = glm::mat4(1);
    //yaeTranslate = glm::rotate(yaeTranslate, glm::radians(90.0f), glm::vec3(1, 0, 0));
    //yaeTranslate = glm::rotate(yaeTranslate, glm::radians(90.0f), glm::vec3(0, 1, 0));
    //ei.modelTransform = yaeTranslate;

    //aModel raiden("models/raiden-fully-textured-and-rigged/source/raiden_textured_rigged.glb");

    glm::mat4 yaeTranslate = glm::mat4(1);


    cObject cube1;
    cube1.objMesh = &cubeMesh;
    cube1.drawWorld = &simpleWorldDraw;
    cube1.p.clear();//cube1.p stores the positions where cube will be rendered
    for (int i = -50; i < 51; i++)
    {
        for (int j = -10; j < 11; j++)
        {
            cube1.p.push_back(glm::vec3(j, -0.5, i));
        }
    }
    cube1.p.push_back(glm::vec3(3, 1, 3));
    cube1.material.mainVec = glm::vec3(0.1, 0.1, 0.1);

    cFlashLgt flashLight;

    cLight light1;
    light1.flashLight = &flashLight;
    light1.lightType = 2;
    sceneCamera->camLight = light1.flashLight;

    cDirLgt dirLight;
    dirLight.ambient = glm::vec3(0.8);
    dirLight.diffuse = glm::vec3(0);
    dirLight.specular = glm::vec3(0);

    cLight light2;
    light2.dirLight = &dirLight;
    light2.lightType = 1;

    sceneShader->use();

    float bgred = 0.0f;
    float bggreen = 0.0f;
    float bgblue = 0.0f;

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(sceneWindow->window))
    {
        sceneCamera->run(sceneWindow->window);
        processInput(sceneWindow->window);
        makeBlue(sceneWindow->window, bgred, bggreen, bgblue);

        glClearColor(bgred, bggreen, bgblue, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sceneShader->use();
        sceneCamera->set_view(view);
        sceneShader->setVec3("camPos", sceneCamera->cameraPos);
        sceneShader->setMat4("view", view);
        sceneShader->setMat4("projection", sceneCamera->PROJ);

        light1.run(*sceneShader);
        light2.run(*sceneShader);

        yaeTranslate = glm::rotate(yaeTranslate, glm::radians(1.0f), glm::vec3(0, 1, 0));

        cube1.draw(model, *sceneShader, *sceneCamera);

        glfwSwapBuffers(sceneWindow->window);
        glfwPollEvents();
    }
    return;
}

testScene::testScene(initWindow& thisWindow, Shader& thisShader, camera& thisCamera)
{
    sceneWindow = &thisWindow;
    sceneShader = &thisShader;
    sceneCamera = &thisCamera;
}