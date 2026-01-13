#version 400 core

    out vec4 FragColor;
    in vec2 uv;

    uniform sampler2D screenTexture;
    uniform bool grayscale;
    uniform bool invert;

    void main()
    {
        vec3 color = texture(screenTexture, uv).rgb;

        if (grayscale)
        {
            float g = dot(color, vec3(0.2126, 0.7152, 0.0722));
            color = vec3(g);
        }

        if (invert)
            color = vec3(1.0) - color;

        FragColor = vec4(color 1.0);
    }