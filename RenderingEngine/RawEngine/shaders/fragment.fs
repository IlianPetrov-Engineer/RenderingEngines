#version 400 core

out vec4 FragColor;
in vec3 fPos;
in vec3 fNor;
in vec2 uv;

in vec4 b;

void main()
{
	FragColor = vec4(fNor,1);
}