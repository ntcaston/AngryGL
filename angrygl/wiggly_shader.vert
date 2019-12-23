#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNorm;
layout (location = 2) in vec2 inTexCoord;

out vec2 TexCoord;
out vec3 Norm;
out vec4 FragPosLightSpace;

// Transformation matrices
uniform mat4 model;
uniform mat4 PV;
uniform mat4 aimRot;
uniform mat4 lightSpaceMatrix;

uniform vec3 nosePos;
uniform float time;

const float wiggleMagnitude = 3.0;
const float wiggleDistModifier = 0.12;
const float wiggleTimeModifier = 9.4;

void main() {
  float xOffset = sin(wiggleTimeModifier * time + wiggleDistModifier * distance(nosePos, inPos)) * wiggleMagnitude;
  gl_Position = PV * model * vec4(inPos.x + xOffset, inPos.y, inPos.z, 1.0);
  TexCoord = inTexCoord;
  FragPosLightSpace = lightSpaceMatrix * model * vec4(inPos, 1.0);
  // TODO fix norm for wiggle
  Norm = vec3(aimRot * vec4(inNorm, 1.0));
}

