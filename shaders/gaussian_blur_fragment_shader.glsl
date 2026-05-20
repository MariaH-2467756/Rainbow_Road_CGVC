#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform int passIndex;

const float weight[5] = float[](0.227027, 0.194595, 0.121622, 0.054054, 0.016216);

void main() {
    vec2 texOffset = 1.0 / textureSize(screenTexture, 0);
    vec3 result = texture(screenTexture, TexCoords).rgb * weight[0];

    vec2 dir = (passIndex == 0)
        ? vec2(texOffset.x, 0.0)
        : vec2(0.0, texOffset.y);

    for (int i = 1; i < 5; ++i) {
        result += texture(screenTexture, TexCoords + dir * i).rgb * weight[i];
        result += texture(screenTexture, TexCoords - dir * i).rgb * weight[i];
    }

    FragColor = vec4(result, 1.0);
}