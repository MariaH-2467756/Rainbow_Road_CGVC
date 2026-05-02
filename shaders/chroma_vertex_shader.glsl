#version 330 core
layout (location = 0) in vec2 aPos; // Pass as vec2 (or vec3 with z=0)
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 worldPos = model * vec4(aPos, 0.0, 1.0);
    
    gl_Position = projection * view * worldPos;
    TexCoord = aTexCoord;
}