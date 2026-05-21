#version 330 core
uniform sampler2D screenTexture;
in vec2 TexCoords;
out vec4 fragColor;

void main() {
    vec4 color = texture(screenTexture, TexCoords);
    vec2 center = abs(TexCoords - 0.5);
    bool onCrosshair = (center.x < 0.002 && center.y < 0.02) || (center.y < 0.002 && center.x < 0.02);

    if (onCrosshair) {
        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    } else {
        fragColor = color;
    }
}