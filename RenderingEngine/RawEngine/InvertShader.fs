#version 400 core

out vec4 FragColor;
in vec2 uv;

uniform sampler2D screenTexture;

void main()
{
    vec3 colour = texture(screenTexture, uv).rgb;
    FragColor = vec4(vec3(1.0) - colour, 1.0);
}