#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec4 rotQuat;
layout (location = 3) in vec3 posOffset;

out vec2 TexCoord;

// Transformation matrices
uniform mat4 PV;

vec4 hamiltonProduct(vec4 q1, const vec4 q2) {
  return vec4(
    q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
    q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
    q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w,
    q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z);
}

vec3 rotateByQuat(vec3 v, vec4 q) {
  vec4 qPrime = vec4(-q.x, -q.y, -q.z, q.w);
  vec4 vPrime = hamiltonProduct(hamiltonProduct(q, vec4(v.x, v.y, v.z, 0.0)), qPrime);
  return vec3(vPrime.x, vPrime.y, vPrime.z);
}


void main() {
  vec3 rotatedInPos = rotateByQuat(inPos, rotQuat);
  gl_Position = PV * vec4(rotatedInPos + posOffset, 1.0);
  TexCoord = inTexCoord;
}
