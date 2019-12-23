#include "opengl/shader.h"

namespace {

// Returns true iff successful
bool checkShaderCompilation(const unsigned int shaderId) {
  int success;
  // TODO(nick.c): This doesn't need to get allocated repeatedly... not a biggie
  // though.
  char infoLog[512];
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
  return success == 1;
}

bool checkProgramLinking(const unsigned int programId) {
  int success;
  char infoLog[512];
  glGetProgramiv(programId, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(programId, 512, NULL, infoLog);
    std::cout << "Error linking program\n" << infoLog << std::endl;
  }
  return success == 1;
}

} // namespace

// static
Shader Shader::create(const GLchar *vertexPath, const GLchar *fragmentPath) {
  // 1. retrieve the vertex/fragment source code from filePath
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;
  // ensure ifstream objects can throw exceptions:
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
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
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  } catch (std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    exit(1);
  }
  const char *vShaderCode = vertexCode.c_str();
  const char *fShaderCode = fragmentCode.c_str();

  const unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vShaderCode, NULL);
  glCompileShader(vertexShader);
  const unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
  glCompileShader(fragmentShader);
  if (!checkShaderCompilation(vertexShader) ||
      !checkShaderCompilation(fragmentShader)) {
    exit(1);
  }

  const unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  if (!checkProgramLinking(shaderProgram)) {
    exit(1);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return Shader(shaderProgram);
}

void Shader::use() const { glUseProgram(id); }

void Shader::setBool(const std::string &name, bool value) const {
  const int loc = glGetUniformLocation(id, name.c_str());
  if (loc < 0) {
    std::cerr << "Couldn't find " << name << std::endl;
    exit(1);
  }
  glUniform1i(loc, (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
  const int loc = glGetUniformLocation(id, name.c_str());
  if (loc < 0) {
    std::cerr << "Couldn't find " << name << std::endl;
    exit(1);
  }
  glUniform1i(loc, value);
}

void Shader::setFloat(const std::string &name, float value) const {
  const int loc = glGetUniformLocation(id, name.c_str());
  if (loc < 0) {
    std::cerr << "Couldn't find " << name << std::endl;
    exit(1);
  }
  glUniform1f(loc, value);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const {
  const int loc = glGetUniformLocation(id, name.c_str());
  if (loc < 0) {
    std::cerr << "Couldn't find " << name << std::endl;
    exit(1);
  }
  glUniform3f(loc, x, y, z);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &v) const {
  const int loc = glGetUniformLocation(id, name.c_str());
  if (loc < 0) {
    std::cerr << "Couldn't find " << name << std::endl;
    exit(1);
  }
  glUniform3f(loc, v.x, v.y, v.z);
}
