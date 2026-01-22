#version 400 core

    out vec4 FragColor;
    in vec2 uv;

    uniform sampler2D screenTexture;

    uniform bool grayscale;
    uniform bool invert;

    uniform bool pixelize;
    uniform float pixelAmount;

    uniform bool edge;

    void main()
    {
        vec2 sampleUV = uv;
        
        if(edge)
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

            vec4 sampleTex[9];
            vec4 col = vec4(0.0);

            for(int i = 0; i < 9; i++)
            {
                col += texture(screenTexture, uv.st + offsets[i]) * kernel[i];
            }

            col.a = 1.0;
            FragColor = col;
        }

        else
        {
            if (pixelize)
            {
                sampleUV = floor(uv * pixelAmount) / pixelAmount;
            }
        
            vec3 color = texture(screenTexture, sampleUV).rgb;

            if (grayscale)
            {
                float g = dot(color, vec3(0.2126, 0.7152, 0.0722)); 
                color = vec3(g);
            }

            if (invert)
                color = vec3(1.0) - color;

            FragColor = vec4(color, 1.0);
        }
    }