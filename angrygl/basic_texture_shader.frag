#version 330 core
in vec2 TexCoord;

out vec4 FragColor;

struct DirectionLight {
  vec3 dir;
  vec3 color;
};
uniform DirectionLight directionLight;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;
uniform bool useLight;
uniform vec3 ambient;

void main() {
  vec4 color = texture(texture_diffuse, TexCoord);
  if (useLight) {
    vec3 lightDir = normalize(-directionLight.dir);
    vec3 normal = vec3(texture(texture_normal, TexCoord));
    normal = normalize(normal * 2.0 - 1.0);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 amb = ambient * vec3(texture(texture_diffuse, TexCoord));
    color = vec4(directionLight.color, 1.0) * color * diff + vec4(amb, 1.0);
  }
  FragColor = color;
}

