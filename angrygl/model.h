#ifndef ANG_SD_MODEL_H_
#define ANG_SD_MODEL_H_

#include <vector>

#include "angrygl/player_mesh.h"
#include "opengl/shader.h"
#include "opengl/texture.h"
#include "opengl/vertex.h"
#include <assimp/scene.h>

class Model {
public:
  /*  Functions   */
  Model(char *path, bool enableTextures = true) {
    this->enableTextures = enableTextures;
    loadModel(path);
  }

  void Draw(Shader shader) const;

private:
  /*  Model Data  */
  bool enableTextures;
  std::vector<PlayerMesh> meshes;
  std::string directory;
  std::vector<Texture> texturesLoaded;
  /*  Functions   */
  void loadModel(std::string path);
  void processNode(aiNode *node, const aiScene *scene);
  PlayerMesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                            TextureType type2);
};

#endif // ANG_SD_MODEL_H_
