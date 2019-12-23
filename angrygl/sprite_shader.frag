#version 330 core
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D spritesheet;

uniform int numCols;
uniform float timePerSprite;
uniform float age;

void main() {
  // Doing this for every fragment is pretty wasteful...
  int col = int(age / timePerSprite);
  vec2 spriteTexCoord = vec2(TexCoord.x / numCols + col * (1.0 / numCols), TexCoord.y);
  // TODO interpolation
  FragColor = texture(spritesheet, spriteTexCoord);
}
