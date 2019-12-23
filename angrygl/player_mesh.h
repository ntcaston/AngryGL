#ifndef ANG_SD_MESH_H_
#define ANG_SD_MESH_H_

#include <vector>

#include "opengl/shader.h"
#include "opengl/texture.h"
#include "opengl/vertex.h"

class PlayerMesh {
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  PlayerMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
             std::vector<Texture> textures);

  void Draw(Shader shader) const;
  ~PlayerMesh();

  PlayerMesh(PlayerMesh &&);
  PlayerMesh(const PlayerMesh &) = delete;

  void updateVertices(std::vector<Vertex> newVertices);

private:
  mutable bool verticesDirty = false;
  unsigned int VAO, VBO, EBO;
  void setupMesh();
};

#endif // ANG_SD_MESH_H_
