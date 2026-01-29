#version 400 core

out vec4 FragColor;
in vec2 uv;

uniform sampler2D screenTexture;

void main()
{
    vec3 colour = texture(screenTexture, uv).rgb;

    float gray = dot(colour, vec3(0.2126, 0.7152, 0.0722));
    FragColor = vec4(vec3(gray), 1.0);
}