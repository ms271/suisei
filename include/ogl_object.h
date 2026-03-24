#pragma once

#include "ogl_presets.h"
#include "ogl_shader.h"
#include "ogl_texture.h"
#include "ogl_cam.h"
#include "ogl_lighting.h"

class camera;

static const inline std::vector<float> cube_model = {
        //position            //tex coord  //normal
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
};

struct vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class mesh
{
public:
    unsigned int VBO = 0, VAO = 0;
    int vertexSize = 8;
    const std::vector<float>* v = &cube_model;

    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<texture> textures;

    void buffer();
    ~mesh();
    void del();
    void bind();
    void unbind();
};

class matrl
{
public:
    glm::vec3 mainVec = glm::vec3(1.0f, 1.0f, 1.0f);
    texture* mainTex;

    glm::vec3 ambVec = glm::vec3(1.0f, 0.5f, 0.31f);
    texture* diffTex;
    glm::vec3 diffVec = glm::vec3(1.0f, 0.5f, 0.31f);
    
    texture* specTex;
    glm::vec3 specVec = glm::vec3(0.5f, 0.5f, 0.5f);
    float shininess = 32.0f;
};

void simpleWorldDraw(glm::mat4& model, shader& ourShader, mesh* objMesh, std::vector<glm::vec3>& p);

void simpleHudDraw(glm::mat4& model, shader& ourShader, mesh* objMesh, std::vector<glm::vec3>& p, camera& cam);

class object
{
public:
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
    
    bool useDiffTex = 0;
    bool useMainTex = 0;
    bool useSpecTex = 0;

    bool useFlatTex = 0;

    int useLightType = 0;//0 posLight, 1 dirLight, 2 flashLight
    bool flatShade = 0;

    mesh* objMesh;
    matrl material;
    texture* flatTex;

    void setTexture();
    void draw (glm::mat4& model, shader& ourShader, camera& cam);

    void (*drawWorld)(glm::mat4&, shader&, mesh*, std::vector<glm::vec3>&);
    void (*drawHUD)(glm::mat4&, shader&, mesh*, std::vector<glm::vec3>&, camera&);
    
    int type = 1;//1 == world, 0 == hud

    std::vector<glm::vec3> p = 
    {
        glm::vec3(0.0f,  0.0f,  0.0f)
    };
};