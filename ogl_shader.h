#ifndef OGL_SHADER
#define OGL_SHADER

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "ogl_utils.h"

class shader
{
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    shader(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
};

class bufferSetUp
{
public:
    unsigned int VBO, VAO, EBO;
    bufferSetUp(std::vector<float>& points, std::vector<int>& lines);
}
;
#endif