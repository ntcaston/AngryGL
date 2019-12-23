#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNorm;
layout (location = 2) in vec2 inTexCoord;

out vec2 TexCoord;
out vec3 Norm;
out vec4 FragPosLightSpace;
out vec3 FragWorldPos;

// Transformation matrices
uniform mat4 model;
uniform mat4 PV;
uniform mat4 aimRot;
uniform mat4 lightSpaceMatrix;

void main() {
  gl_Position = PV * model * vec4(inPos, 1.0);
  TexCoord = inTexCoord;
  Norm = vec3(aimRot * vec4(inNorm, 1.0));
  FragWorldPos = vec3(model * vec4(inPos, 1.0));
  FragPosLightSpace = lightSpaceMatrix * vec4(FragWorldPos, 1.0);
}
