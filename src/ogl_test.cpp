#include"../include/ogl_test.h"

void testScene::run()
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 trans = glm::mat4(1.0f);

    cMesh cubeMesh;

    cObject cube1;
    cube1.objMesh = &cubeMesh;
    cube1.drawWorld = &simpleWorldDraw;
    cube1.material.mainVec = glm::vec3(0.1, 0.1, 0.1);

    cDirLgt dirLight;
    dirLight.ambient = glm::vec3(0.8);
    dirLight.diffuse = glm::vec3(0.8);
    dirLight.specular = glm::vec3(0);

    cLight light2;
    light2.dirLight = &dirLight;
    light2.lightType = 1;

    sceneShader->use();

    float bgred = 0.0f;
    float bggreen = 0.0f;
    float bgblue = 0.0f;

    glEnable(GL_DEPTH_TEST);

    std::vector<double> frameTimes (200);
    std::vector<int> noOfVertices (200);
    
    glfwSwapInterval(0);

    for(int i = 0; i < 200; i++)
    {
        int cubex = randomInt(0, 60);
        int cubey = randomInt(0, 60);
        int cubez = randomInt(0, 60);

        float sidex = 100.0f / (float)cubex;
        float sidey = 100.0f / (float)cubey;
        float sidez = 100.0f / (float)cubez;

        cube1.p.clear();//cube1.p stores the positions where cube will be rendered
        for (int i = 0; i < 2 * cubex + 1; i++)
        {
            for (int j = 0; j < 2 * cubey + 1; j++)
            {
                for (int k = 0; k < 2 * cubez + 1; k++)
                {
                    cube1.p.push_back(glm::vec3(-100 + i * sidex, -100 + j * sidey, -100 + k * sidez));
                }
            }
        }

        noOfVertices[i] = cube1.p.size() * 36;

        auto start = std::chrono::high_resolution_clock::now();

        glClearColor(bgred, bggreen, bgblue, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sceneShader->use();
        sceneCamera->set_view(view);
        sceneShader->setVec3("camPos", sceneCamera->cameraPos);
        sceneShader->setMat4("view", view);
        sceneShader->setMat4("projection", sceneCamera->PROJ);

        light2.run(*sceneShader);

        cube1.draw(model, *sceneShader, *sceneCamera);

        glfwSwapBuffers(sceneWindow->window);
        glfwPollEvents();

        auto end = std::chrono::high_resolution_clock::now();
        frameTimes[i] = std::chrono::duration<double, std::milli>(end - start).count();
    }
    
    glfwSwapInterval(1);

    std::ofstream csv("python/profiler_data.csv");
    csv << "Vertices,RenderTime\n";

    for (int i = 0; i < 200; i++)
    {
        csv << noOfVertices[i] << ","
            << frameTimes[i] << "\n";
    }

    csv.close();
    glfwDestroyWindow(sceneWindow->window);
    return;
}

testScene::testScene(initWindow& thisWindow, Shader& thisShader, camera& thisCamera)
{
    sceneWindow = &thisWindow;
    sceneShader = &thisShader;
    sceneCamera = &thisCamera;
}

int randomInt(int min, int max) {
    static std::mt19937 rng(std::random_device{}());
    return std::uniform_int_distribution<int>(min, max)(rng);
}