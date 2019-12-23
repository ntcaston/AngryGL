#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 2) in vec2 inTexCoord;

out vec2 TexCoord;

// Transformation matrices
uniform mat4 model;
uniform mat4 PV;

void main() {
  gl_Position = PV * model * vec4(inPos, 1.0);
  TexCoord = inTexCoord;
}

