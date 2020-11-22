#version 330
in vec3 worldPosition;
in vec3 worldNormal;

layout(location = 0) out vec4 fragColor;

struct Material {
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform Material material;
uniform vec3 viewPosition;

void main()
{
    vec3 normal = normalize(worldNormal);
    vec3 lightDir = normalize(light.position - worldPosition);

    // ambient
    vec3 ambient = light.ambient * material.diffuse;

    // diffuse
    float diffuseFactor = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diffuseFactor * material.diffuse);

    // specular
    vec3 viewDir = normalize(viewPosition - worldPosition);
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (specularFactor * material.specular);

    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0);
}
