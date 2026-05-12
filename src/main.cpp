#define STB_IMAGE_IMPLEMENTATION

#include "../include/ogl_presets.h"
#include "../include/ogl_utils.h"
#include "../include/ogl_shader.h"
#include "../include/ogl_texture.h"
#include "../include/ogl_cam.h"
#include "../include/ogl_object.h"
#include "../include/ogl_draw.h"
#include "../include/ogl_assimp.h"
#include "../include/ogl_test.h"

int main()
{//next 5 lines always
    initWindow secondWindow;
    Shader ourShader2("shader/vertex.glsl", "shader/fragment.glsl");//in shader
    camera cam2;//in cam
    set_callback(secondWindow.window, cam2);//in cam
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);

    testScene alpha(secondWindow, ourShader2, cam2);
    alpha.run();
    glDeleteProgram(ourShader2.ID);

    system("python python/llm.py");

    std::vector<double> weights;

    std::ifstream file("python/model_weights.csv");
    std::string line;
    std::getline(file, line);

    std::stringstream ss(line);
    std::string val;
    while (std::getline(ss, val, ','))
    {
        weights.push_back(std::stod(val));
    }
    file.close();

    initWindow ourWindow;//in util
    Shader ourShader("shader/vertex.glsl", "shader/fragment.glsl");//in shader
    camera cam1;//in cam
    set_callback(ourWindow.window, cam1);//in cam
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);

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

    aModel f1Car("models/2020-f1-mercedes-benz-w11/source/2020 F1 Mercedes-Benz W11.glb", true);
    glm::mat4 yaeTranslate = glm::translate(glm::mat4(1), glm::vec3(0, -1, 0));
    f1Car.modelTransform = yaeTranslate;

    aModel nsx("models/honda_nsx_1990/scene.gltf", true);
    nsx.modelTransform = yaeTranslate;

    cObject cube1;
    cube1.objMesh = &cubeMesh;
    cube1.drawWorld = &simpleWorldDraw;
    cube1.p.clear();//cube1.p stores the positions where cube will be rendered
    for (int i = -50; i < 51; i++)
    {
        for (int j = -10; j < 11; j++)
        {
            cube1.p.push_back(glm::vec3(j, -1.5 , i));
        }
    }
    cube1.p.push_back(glm::vec3(3, 1, 3));
    cube1.material.mainVec = glm::vec3(0.1, 0.1, 0.1);

    cFlashLgt flashLight;
    
    cLight light1;
    light1.flashLight = &flashLight;
    light1.lightType = 2;
    cam1.camLight = light1.flashLight;
    
    cDirLgt dirLight;
    dirLight.ambient = glm::vec3(0.8);
    dirLight.diffuse = glm::vec3(0);
    dirLight.specular = glm::vec3(0);
    
    cLight light2;
    light2.dirLight = &dirLight;
    light2.lightType = 1;
    
    float bgred = 0.0f;
    float bggreen = 0.0f;
    float bgblue = 0.0f;

    int vertices = 0;
    vertices += f1Car.numOfVertices();
    std::cout<<vertices<<'\n';
    vertices += 36*cube1.p.size();
    std::cout << vertices<<'\n';

    double predicted = weights[0]
        + weights[1] * vertices;
    std::cout<<vertices<<',';
    std::cout<<predicted<<',';

    double fps = 1000.0f / predicted;
    std::cout<<fps<<'\n';
    ourShader.use();

    bool lowFps = 0;
    if(fps < 60.0f) 
    {
        std::cout<<"low fps\n";
        lowFps = 1;
    }
    else std::cout<<"high fps\n";

    double TotalTime = 0;
    int numOfFrames = 0;

    while (!glfwWindowShouldClose(ourWindow.window))
    {
        auto start = std::chrono::high_resolution_clock::now();

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
        light2.run(ourShader);

        if(!lowFps) f1Car.Draw(ourShader);
        else nsx.Draw(ourShader);
        //raiden.Draw(ourShader);
        //yae.Draw(ourShader);
        //ei.Draw(ourShader);
        cube1.draw(model, ourShader, cam1);

        glfwSwapBuffers(ourWindow.window);
        glfwPollEvents();
        
        auto end = std::chrono::high_resolution_clock::now();
        TotalTime += std::chrono::duration<double, std::milli>(end - start).count();
        numOfFrames++;
    }
    //next 3 lines always
    double actualTime = TotalTime/numOfFrames;
    std::cout<<actualTime<<','<<1000.0f/actualTime;
    glDeleteProgram(ourShader.ID);
    glfwTerminate();
    return 0;
}