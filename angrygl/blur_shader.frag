#version 330 core
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D image;

uniform bool horizontal;
#define BLUR_DIST 28
const float weight[BLUR_DIST] = float[] (0.049835, 0.049448, 0.048304, 0.046456, 0.043987, 0.041004, 0.037631, 0.034002, 0.030246, 0.026489, 0.022839, 0.019388, 0.016203, 0.013331, 0.010799, 0.008612, 0.006762, 0.005227, 0.003978, 0.00298, 0.002199, 0.001597, 0.001142, 0.000804, 0.000557, 0.00038, 0.000255, 0.000169);

float CalcBrightness(vec3 col) {
  return (col.x + col.y + col.z) / (3);
}

void main() {
  vec3 baseSample = texture(image, TexCoord).rgb;
  vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel
  vec3 result = baseSample * weight[0]; // current fragment's contribution
  if (horizontal) {
    for(int i = 1; i < BLUR_DIST; ++i) {
      result += texture(image, TexCoord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
      result += texture(image, TexCoord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
    }
  } else {
    for(int i = 1; i < BLUR_DIST; ++i) {
      result += texture(image, TexCoord + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
      result += texture(image, TexCoord - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
    }
  }
  FragColor = vec4(result, 1.0);
}
