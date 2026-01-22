#version 400 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 fPos;
out vec3 fNor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    fPos = vec3(model * vec4(aPos, 1.0)); 

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    fNor = normalize(normalMatrix * aNormal); 

    gl_Position = projection * view * model * vec4(aPos, 1.0);
} 