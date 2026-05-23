#version 330 core
out vec4 fragmentColor;
#define NUM_LIGHTS 16

// hardcoded materials cause we believe in our project diffrent materials for all meshes seems unecessery.
const vec3 matAmbient = vec3(1.0, 1.0, 1.0);
const vec3 matDiffuse = vec3(1.0, 1.0, 1.0);
const vec3 matSpecular = vec3(0.5);
const float matShininess = 32.0;

struct LightPoint {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 fragmentPosition;  
in vec3 normal;  
in vec2 textureCoordinate; 

uniform sampler2D texture1;
uniform vec3 viewPosition;
uniform LightPoint lights[NUM_LIGHTS];

void main()
{
    vec4 texColor = texture(texture1, textureCoordinate);
    vec3 result = vec3(0.0);

    for (int i = 0; i < NUM_LIGHTS; i++) {
        vec3 ambient = lights[i].ambient * matAmbient * texColor.rgb;

        vec3 norm = normalize(normal);
        vec3 lightDirection = normalize(lights[i].position - fragmentPosition);
        float diff = max(dot(norm, lightDirection), 0.0);
        vec3 diffuse = lights[i].diffuse * (diff * matDiffuse) * texColor.rgb;

        vec3 viewDirection = normalize(viewPosition - fragmentPosition);
        vec3 reflectDirection = reflect(-lightDirection, norm);
        float spec = pow(max(dot(viewDirection, reflectDirection), 0.0),matShininess);
        vec3 specular = lights[i].specular * (spec * matSpecular);

        result += ambient + diffuse + specular;
    }

    fragmentColor = vec4(result, 1.0);
}