#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;
uniform vec2  texelSize;
uniform int   effectMode;

vec3 convolve3x3(sampler2D tex, vec2 uv, float kernel[9], vec2 ts) {
    vec2 offsets[9] = vec2[](
        vec2(-ts.x,  ts.y), vec2(0.0,  ts.y), vec2( ts.x,  ts.y),
        vec2(-ts.x,  0.0 ), vec2(0.0,  0.0 ), vec2( ts.x,  0.0 ),
        vec2(-ts.x, -ts.y), vec2(0.0, -ts.y), vec2( ts.x, -ts.y)
    );

    vec3 result = vec3(0.0);
    for (int i = 0; i < 9; i++) {
        result += texture(tex, uv + offsets[i]).rgb * kernel[i];
    }
    return result;
}

vec3 gaussianBlur(sampler2D tex, vec2 uv, vec2 ts) {
    float kernel[9] = float[](
        0.0751, 0.1238, 0.0751,
        0.1238, 0.2042, 0.1238,
        0.0751, 0.1238, 0.0751
    );
    return convolve3x3(tex, uv, kernel, ts);
}



vec3 laplacianEdge(sampler2D tex, vec2 uv, vec2 ts) {
    float kernel[9] = float[](
        0.0, -1.0,  0.0,
        -1.0,  4.0, -1.0,
        0.0, -1.0,  0.0
    );
    return convolve3x3(tex, uv, kernel, ts);
}

void main() {
    if (effectMode == 1) {
        FragColor = vec4(gaussianBlur(screenTexture, TexCoords, texelSize), 1.0);

    } else if (effectMode == 2) {
        vec3 original = texture(screenTexture, TexCoords).rgb;
        vec3 edges    = laplacianEdge(screenTexture, TexCoords, texelSize);
        vec3 result   = original + clamp(edges, 0.0, 1.0);
        FragColor     = vec4(clamp(result, 0.0, 1.0), 1.0);

    } else if (effectMode == 3) {
        vec3 scene = texture(screenTexture, TexCoords).rgb;
        vec3 bloom = gaussianBlur(bloomTexture,  TexCoords, texelSize);
        vec3 combined = scene + bloom * 1.5;
        FragColor = vec4(combined/ (combined + vec3(1.0)), 1.0);

    } else {
        FragColor = texture(screenTexture, TexCoords);
    }
}

