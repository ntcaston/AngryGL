#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "glm/glm.hpp"

class Shader {
public:
  // the program ID
  const unsigned int id;

  static Shader create(const GLchar *vertexPath, const GLchar *fragmentPath);

  // constructor reads and builds the shader
  explicit Shader(const unsigned int _id) : id(_id) {}

  // use/activate the shader
  void use() const;

  // utility uniform functions
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setVec3(const std::string &name, float x, float y, float z) const;
  void setVec3(const std::string &name, const glm::vec3 &v) const;
};

#endif
