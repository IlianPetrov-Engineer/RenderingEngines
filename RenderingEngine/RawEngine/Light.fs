#version 400 core
out vec4 b;

in vec3 fPos;
in vec3 fNor;
  
uniform vec3 objectColor;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform vec3 viewPos;


uniform float ambientStrength;

uniform vec3 ambientColour;

uniform float specularStrength;

uniform float diffuseStength;

void main()
{
    vec3 ambient = ambientStrength * lightColor * ambientColour;

    vec3 norm = normalize(fNor);
    vec3 lightDir = normalize(lightPos - fPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor * diffuseStength;

    vec3 viewDir = normalize(viewPos - fPos);
    vec3 reflectionDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir, reflectionDir), 0.0f), specularStrength);
    vec3 specular = spec * lightColor;

    float constand = 1;
    float linear = 0.5;
    float quadratic = 0.025;
    float distance = length(lightPos - fPos);

    vec3 attenuation = (diffuse + specular) / (constand + linear*distance + quadratic*pow(distance, 2));

    vec3 result = (ambient + attenuation) * objectColor;
    b = vec4(result,1);
}