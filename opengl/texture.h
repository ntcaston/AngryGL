#ifndef SD_TEXTURE_H_
#define SD_TEXTURE_H_

#include <string>

enum class TextureType { DIFFUSE, SPECULAR };

struct Texture {
  unsigned int openGlId;
  TextureType type;
  std::string
      path; // we store the path of the texture to compare with other textures
};

#endif // SD_TEXTURE_H_
