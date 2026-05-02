#version 330 core
out vec4 fragmentColor;
#define NUM_LIGHTS 5

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

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
uniform Material material;
uniform LightPoint lights[NUM_LIGHTS];

void main()
{
    vec4 texColor = texture(texture1, textureCoordinate);
    vec3 result = vec3(0.0);

    for (int i = 0; i < NUM_LIGHTS; i++) {
        vec3 ambient = lights[i].ambient * material.ambient * texColor.rgb;

        vec3 norm = normalize(normal);
        vec3 lightDirection = normalize(lights[i].position - fragmentPosition);
        float diff = max(dot(norm, lightDirection), 0.0);
        vec3 diffuse = lights[i].diffuse * (diff * material.diffuse) * texColor.rgb;

        vec3 viewDirection = normalize(viewPosition - fragmentPosition);
        vec3 reflectDirection = reflect(-lightDirection, norm);
        float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
        vec3 specular = lights[i].specular * (spec * material.specular);

        result += ambient + diffuse + specular;
    }

    fragmentColor = vec4(result, 1.0);
}