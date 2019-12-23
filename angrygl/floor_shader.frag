#version 330 core
in vec2 TexCoord;
in vec4 FragPosLightSpace;
in vec3 FragWorldPos;

out vec4 FragColor;

struct DirectionLight {
  vec3 dir;
  vec3 color;
};
uniform DirectionLight directionLight;

struct PointLight {
  vec3 worldPos;
  vec3 color;
};
uniform PointLight pointLight;
uniform bool usePointLight;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;
uniform sampler2D texture_spec;
uniform bool useLight;
uniform bool useSpec;
uniform vec3 ambient;
uniform sampler2D shadow_map;
uniform vec3 viewPos;

float ShadowCalculation(float bias, vec4 fragPosLightSpace, vec2 offset) {
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  projCoords = projCoords * 0.5 + 0.5;
  float closestDepth = texture(shadow_map, projCoords.xy + offset).r;
  float currentDepth = projCoords.z;
  bias = 0.001;
  float shadow = (currentDepth - bias) > closestDepth ? 1.0 : 0.0;
  return shadow;
}

void main() {
  vec4 color = texture(texture_diffuse, TexCoord);
  if (useLight) {
    vec2 texelSize = 1.0 / textureSize(shadow_map, 0);
    vec3 lightDir = normalize(-directionLight.dir);
    vec3 normal = vec3(texture(texture_normal, TexCoord));
    normal = normalize(normal * 2.0 - 1.0);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 amb = ambient * vec3(texture(texture_diffuse, TexCoord));
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = 0.0;
    for (int x = -1; x <= 1; ++x) {
      for (int y = -1; y <= 1; ++y) {
        shadow += ShadowCalculation(bias, FragPosLightSpace, vec2(x, y) * texelSize);
      }
    }
    shadow /= 9.0;
    shadow *= 0.7;
    color = 0.7* (1.0 - shadow) * vec4(directionLight.color, 1.0) * color * diff + vec4(amb, 1.0);
    if (useSpec) {
      vec3 normal = vec3(0.0, 1.0, 0.0);
      vec3 specLightDir = normalize(vec3(-3.0, 0.0, -1.0));
      vec3 reflectDir = reflect(specLightDir, normal);
      vec3 viewDir = normalize(viewPos - FragWorldPos);
      float shininess = 0.7;
      float str = 1;//0.88;
      float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
      color += str * spec * texture(texture_spec, TexCoord) * vec4(directionLight.color, 1.0);
    }
    if (usePointLight) {
      vec3 lightDir = normalize(pointLight.worldPos - FragWorldPos);
      vec3 normal = vec3(0.0, 1.0, 0.0);
      float diff = max(dot(normal, lightDir), 0.0);
      float distance = length(pointLight.worldPos- FragWorldPos);
      float linear = 0.5;
      float constant = 0;
      float quadratic = 3;
      float attenuation = 1.0 / (constant + linear * distance +
               quadratic * (distance * distance));
      vec3 diffuse  = pointLight.color  * diff * vec3(texture(texture_diffuse, TexCoord));
      diffuse *= attenuation;
      color += vec4(diffuse.xyz, 1.0);
    }
  }
  FragColor = color;
}

