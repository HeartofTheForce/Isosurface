#version 330
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

out vec3 worldPosition;
out vec3 worldNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(inPosition, 1.0);
    worldPosition = vec3(modelMatrix * vec4(inPosition, 1.0));
    worldNormal = normalize(normalMatrix * inNormal);
}
