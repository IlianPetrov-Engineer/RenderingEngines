#version 400 core
out vec4 b;

in vec3 fPos;
in vec3 fNor;
  
uniform vec3 objectColor;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform vec3 viewPos;

void main()
{
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(fNor);
    vec3 lightDir = normalize(lightPos - fPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 100;
    vec3 viewDir = normalize(viewPos - fPos);
    vec3 reflectionDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir, reflectionDir), 0.0f), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    b = vec4(result,1);
}