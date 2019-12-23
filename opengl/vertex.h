#ifndef SD_VERTEX_H_
#define SD_VERTEX_H_

#include "glm/glm.hpp"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
};

#endif // SD_VERTEX_H_
