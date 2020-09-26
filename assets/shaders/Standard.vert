#version 330
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

out vec3 fragPosition;
out vec3 normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(inPosition, 1.0);
    fragPosition = vec3(modelMatrix * vec4(inPosition, 1.0));
    normal = normalize(normalMatrix * inNormal);
}
