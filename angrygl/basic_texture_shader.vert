#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexCoord;

out vec2 TexCoord;
out vec4 FragPosLightSpace;
out vec3 FragWorldPos;

// Transformation matrices
uniform mat4 model;
uniform mat4 PV;
uniform mat4 lightSpaceMatrix;

void main() {
  gl_Position = PV * model * vec4(inPos, 1.0);
  TexCoord = inTexCoord;
  FragWorldPos = vec3(model * vec4(inPos, 1.0));
  FragPosLightSpace = lightSpaceMatrix * vec4(FragWorldPos, 1.0);
}

