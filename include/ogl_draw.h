#pragma once

#include <vector>
#include <glm/glm.hpp>

// Forward declarations to avoid circular includes
class shader;
class mesh;
class camera;

 void simpleWorldDraw(glm::mat4& model, shader& ourShader, mesh* objMesh, std::vector<glm::vec3>& p);

void simpleHudDraw(glm::mat4& model, shader& ourShader, mesh* objMesh, std::vector<glm::vec3>& p, camera& cam);