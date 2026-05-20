#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D screenTextureV
uniform int passIndex;

const int HALF = 7;
const float weights[8] = float[](
    0.2044, 0.1775, 0.1170, 0.0585,
    0.0220, 0.0062, 0.0012, 0.0002
);

vec3 convolve1D(vec2 dir) {
    vec2 offset = 1.0 / textureSize(screenTexture, 0);
    // center tap with negative weight (the -2 in [1,-2,1])
    vec3 result = texture(screenTexture, TexCoords).rgb * (-2.0 * weights[0]);
    for (int i = 1; i <= HALF; i++) {
        vec2 step = dir * offset * float(i);
        result += (texture(screenTexture, TexCoords + step).rgb +
                   texture(screenTexture, TexCoords - step).rgb) * weights[i];
    }
    return result;
}

void main() {
    if (passIndex == 0) {
        // horizontal second derivative
        FragColor = vec4(abs(convolve1D(vec2(1.0, 0.0))), 1.0);

    } else if (passIndex == 1) {
        // vertical second derivative
        FragColor = vec4(abs(convolve1D(vec2(0.0, 1.0))), 1.0);

    } else {
        // add both derivatives together
        vec3 h = texture(screenTexture,  TexCoords).rgb;
        vec3 v = texture(screenTextureV, TexCoords).rgb;
        FragColor = vec4(clamp(h + v, 0.0, 1.0), 1.0);
    }
}