#version 400 core

out vec4 FragColor;
in vec2 uv;

uniform sampler2D screenTexture;
uniform float pixelAmount;

void main()
{
    vec2 pixelUV = floor(uv * pixelAmount) / pixelAmount;
    vec3 colour = texture(screenTexture, pixelUV).rgb;
    FragColor = vec4(colour, 1.0);
}