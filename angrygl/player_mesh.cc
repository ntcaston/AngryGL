#include "angrygl/player_mesh.h"

#include <string>

PlayerMesh::PlayerMesh(std::vector<Vertex> _vertices,
                       std::vector<unsigned int> _indices,
                       std::vector<Texture> _textures)
    : vertices(std::move(_vertices)),
      indices(std::move(_indices)),
      textures(std::move(_textures)) {
  setupMesh();
}

void PlayerMesh::Draw(Shader shader) const {
  glBindVertexArray(VAO);
  if (verticesDirty) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0],
                 GL_STREAM_DRAW);
    verticesDirty = false;
  }
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void PlayerMesh::updateVertices(std::vector<Vertex> newVertices) {
  this->vertices = newVertices;
  verticesDirty = true;
}

void PlayerMesh::setupMesh() {
  verticesDirty = true;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               &indices[0], GL_STATIC_DRAW);

  // vertex positions
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  glEnableVertexAttribArray(0);
  // vertex normals
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, normal));
  glEnableVertexAttribArray(1);
  // vertex texture coords
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, texCoords));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
}

PlayerMesh::PlayerMesh(PlayerMesh &&m)
    : vertices(std::move(m.vertices)), indices(std::move(m.indices)), textures(std::move(m.textures)) {
  setupMesh();
}

PlayerMesh::~PlayerMesh() {
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteVertexArrays(1, &VAO);
}
