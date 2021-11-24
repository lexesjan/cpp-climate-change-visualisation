#version 330

in vec2 texture_coordinates;
in vec3 translated_vertex_position;
in vec3 translated_normal;

out vec4 frag_colour;

struct Material {
  sampler2D texture_diffuse1;
  sampler2D texture_specular1;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct PointLight {
  vec3 position;
  float constant;
  float linear;
  float quadratic;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct DirectedLight {
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform Material material;
const int MAX_POINT_LIGHTS = 2;
uniform PointLight point_light[MAX_POINT_LIGHTS];
uniform DirectedLight directed_light;
uniform vec3 view_position;

vec3 CalculateDirectedLight(DirectedLight light, vec3 normal,
                            vec3 vertex_position, vec3 view_direction) {
  vec3 light_direction = normalize(-light.direction);

  // Diffuse.
  float diff = max(dot(normal, light_direction), 0.0);

  // Specular.
  vec3 reflection_direction = reflect(-light_direction, normal);
  float spec = pow(max(dot(view_direction, reflection_direction), 0.0),
                   material.shininess);

  vec3 ambient =
      light.ambient *
      (vec3(texture(material.texture_diffuse1, texture_coordinates)) +
       material.diffuse);
  vec3 diffuse =
      light.diffuse * diff *
      (vec3(texture(material.texture_diffuse1, texture_coordinates)) +
       material.diffuse);
  vec3 specular =
      light.specular * spec *
      (vec3(texture(material.texture_specular1, texture_coordinates)) +
       material.specular);

  return ambient + diffuse + specular;
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 vertex_position,
                         vec3 view_direction) {
  vec3 light_direction = normalize(light.position - vertex_position);

  // Diffuse.
  float diff = max(dot(normal, light_direction), 0.0f);

  // Specular.
  vec3 reflection_direction = reflect(-light_direction, normal);
  float spec = pow(max(dot(view_direction, reflection_direction), 0.0f),
                   material.shininess);

  // Attenuation.
  float dist = length(light.position - vertex_position);
  float attenuation = 1.0f / (light.constant + light.linear * dist +
                              light.quadratic * (dist * dist));

  vec3 ambient =
      light.ambient *
      (vec3(texture(material.texture_diffuse1, texture_coordinates)) +
       material.diffuse);
  vec3 diffuse =
      light.diffuse * diff *
      (vec3(texture(material.texture_diffuse1, texture_coordinates)) +
       material.diffuse);
  vec3 specular =
      light.specular * spec *
      (vec3(texture(material.texture_specular1, texture_coordinates)) +
       material.specular);
  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;

  return ambient + diffuse + specular;
}

void main() {
  vec3 normal = normalize(translated_normal);
  vec3 view_direction = normalize(view_position - translated_vertex_position);

  vec3 result = CalculateDirectedLight(
      directed_light, normal, translated_vertex_position, view_direction);

  for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
    result += CalculatePointLight(point_light[i], normal,
                                  translated_vertex_position, view_direction);
  }

  frag_colour = vec4(result, 1.0f);
}