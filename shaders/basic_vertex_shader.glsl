#version 330 core
layout (location = 0) in vec3 vertexposition;
layout (location = 1) in vec3 vertexNormal;

out vec3 fragmentPosition;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

void main()
{
    fragmentPosition = vec3(model * vec4(vertexposition, 1.0));
    normal = normalMatrix * vertexNormal;  
    
    gl_Position = projection * view * vec4(fragmentPosition, 1.0);
}
