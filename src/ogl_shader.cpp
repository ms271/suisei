#include "../include/ogl_shader.h"

shader::shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    check_error(vertex, "vshader");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    check_error(fragment, "fshader");

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    check_error(ID, "program");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

shader::~shader()
{
    if (ID != 0) glDeleteProgram(ID);
}

void shader::use()
{
    glUseProgram(ID);
}

int shader::find(const std::string name) const
{
    if(address.find(name) != address.end())
        return address[name];

    int location = glGetUniformLocation(ID, name.c_str());
    if(location != -1)
    {
        address[name] = location;
        return location;
    }

    std::cout<<name<<" uniform is not in the shader\n";

    return location;
}

void shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(find(name), (int)value);
}
// ------------------------------------------------------------------------
void shader::setInt(const std::string& name, int value) const
{
    glUniform1i(find(name), value);
}
// ------------------------------------------------------------------------
void shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(find(name), value);
}
// ------------------------------------------------------------------------
void shader::setVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(find(name), 1, &value[0]);
}
void shader::setVec2(const std::string& name, float x, float y) const
{
    glUniform2f(find(name), x, y);
}
// ------------------------------------------------------------------------
void shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(find(name), 1, &value[0]);
}
void shader::setVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(find(name), x, y, z);
}
// ------------------------------------------------------------------------
void shader::setVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(find(name), 1, &value[0]);
}
void shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(find(name), x, y, z, w);
}
// ------------------------------------------------------------------------
void shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(find(name), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(find(name), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(find(name), 1, GL_FALSE, &mat[0][0]);
}