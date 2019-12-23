#ifndef ANG__SD_MODEL_H_
#define ANG__SD_MODEL_H_

#include <unordered_map>
#include <vector>

#include "angrygl/player_mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/vector3.h"
#include "opengl/shader.h"
#include "opengl/vertex.h"
#include <assimp/scene.h>

class PlayerModel {
public:
  /*  Functions   */
  PlayerModel(char *path) { loadModel(path); }

  void Draw(Shader shader, bool applyTextures = true) const;
  unsigned int GetNodeVAO() const;
  void UpdatePointsForAnim(
      bool isMeasuredFrame,
      const glm::vec2 movementDir,
      const float aimTheta,
      float time);
  glm::mat4 getAnimatedGunTransform() const;

  std::vector<PlayerMesh> meshes;
private:
  glm::mat4 gunTransform;
  unsigned int nodeVAO;
  unsigned int nodeVBO;
  int numNodes;
  Assimp::Importer importer;
  const aiScene *scene;
  aiMatrix4x4 globalInv;
  /*  Model Data  */
  std::string directory;
  /*  Functions   */
  void loadModel(std::string path);
  void processNode(int* meshesProcessed, bool isMeasuredFrame, aiNode *node, const aiScene *scene, int depth,
                  std::unordered_map<std::string, aiMatrix4x4> &);
  PlayerMesh processMesh(bool isMeasuredFrame, aiMesh *mesh, const aiScene *scene,
                         std::unordered_map<std::string, aiMatrix4x4> &);
  std::vector<Vertex> getMeshVertices(const bool isMeasuredFrame,
      aiMesh *mesh, const aiScene *scene,
      std::unordered_map<std::string, aiMatrix4x4> &nodeTransformMap);
};

inline void scaledAdd(aiMatrix4x4& m1, const float scale, const aiMatrix4x4& m2)
{
  m1.a1 += scale * m2.a1;
  m1.a2 += scale * m2.a2;
  m1.a3 += scale * m2.a3;
  m1.a4 += scale * m2.a4;
  m1.b1 += scale * m2.b1;
  m1.b2 += scale * m2.b2;
  m1.b3 += scale * m2.b3;
  m1.b4 += scale * m2.b4;
  m1.c1 += scale * m2.c1;
  m1.c2 += scale * m2.c2;
  m1.c3 += scale * m2.c3;
  m1.c4 += scale * m2.c4;
  m1.d1 += scale * m2.d1;
  m1.d2 += scale * m2.d2;
  m1.d3 += scale * m2.d3;
  m1.d4 += scale * m2.d4;
}

inline std::ostream &operator<<(std::ostream &os, const aiMatrix4x4 &m) {
  os << "{Matrix ";
  os << m.a1 << ", ";
  os << m.a2 << ", ";
  os << m.a3 << ", ";
  os << m.a4 << " || ";
  os << m.b1 << ", ";
  os << m.b2 << ", ";
  os << m.b3 << ", ";
  os << m.b4 << " || ";
  os << m.c1 << ", ";
  os << m.c2 << ", ";
  os << m.c3 << ", ";
  os << m.c4 << " || ";
  os << m.d1 << ", ";
  os << m.d2 << ", ";
  os << m.d3 << ", ";
  os << m.d4;
  os << "}";
  return os;
}

#endif // ANG__SD_MODEL_H_
