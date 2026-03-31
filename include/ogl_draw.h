#pragma once

#include <vector>
#include <glm/glm.hpp>

// Forward declarations to avoid circular includes
class Shader;
class cMesh;
class camera;

 void simpleWorldDraw(glm::mat4& model, Shader& ourShader, cMesh* objMesh, std::vector<glm::vec3>& p);

void simpleHudDraw(glm::mat4& model, Shader& ourShader, cMesh* objMesh, std::vector<glm::vec3>& p, camera& cam);