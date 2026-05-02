#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D chromaTexture;

vec3 RGBtoYCbCr(vec3 rgb) {
    float Y  =  0.299  * rgb.r + 0.587  * rgb.g + 0.114  * rgb.b;
    float Cb = 0.564 * (rgb.b - Y) + 0.5;
    float Cr = 0.713 * (rgb.r - Y) + 0.5;
    return vec3(Y, Cb, Cr);
}

void main() {
    vec4 texColor = texture(chromaTexture, TexCoord);
    vec3 ycbcr = RGBtoYCbCr(texColor.rgb);

    float Cb = ycbcr.y;
    float Cr = ycbcr.z;

    // Get rid of the green
    if (Cb > 0.48 && Cb < 0.62 && Cr > 0.30 && Cr < 0.45) {
        discard;
    }

    FragColor = texColor;
}