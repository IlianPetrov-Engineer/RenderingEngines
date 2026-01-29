#version 400 core

out vec4 FragColor;
in vec2 uv;

uniform sampler2D screenTexture;

void main()
{
    float offset = 0.001;
    
    vec2 offsets[9] = vec2[ ](
        vec2(-offset, offset), vec2(0.0f, offset), vec2(offset, offset),
        vec2(-offset, 0.0f), vec2(0.0f, 0.0f), vec2( offset, 0.0f),
        vec2(-offset, -offset), vec2(0.0f, -offset), vec2( offset, -offset)
    );

    float kernel[9] = float[ ](
        1.0, 1.0, 1.0,
        1.0, -8.0, 1.0,
        1.0, 1.0, 1.0
    );

    vec3 sampleTex[9];

    for(int i = 0; i < 9; i++)
        sampleTex[i] = texture(screenTexture, uv + offsets[i]).rgb;

    vec3 colour = vec3(0.0);

    for(int i = 0; i < 9; i++)
        colour += sampleTex[i] * kernel[i];

    FragColor = vec4(colour, 1.0);
}