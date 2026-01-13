#include "PostProcessing.h"
#include <iostream>
#include "DefaultPrograms.h"

void PostProcessing::Initialise(int width, int height)
{
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &colourTexture);
    glBindTexture(GL_TEXTURE_2D, colourTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
        GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourTexture, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "PostProcessing framebuffer is not complete\n";

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    InitialQuad();
    InitialShader();
}

void PostProcessing::Begin()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PostProcessing::End()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "grayscale"), grayscale);
    glUniform1i(glGetUniformLocation(shaderProgram, "invert"), invert);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colourTexture);
    // something's missing here...!
    //  glUniform...

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void PostProcessing::InitialQuad()
{
    float quadVertices[] = {
        -1,  1,  0, 1,
        -1, -1,  0, 0,
         1, -1,  1, 0, // 0? 1? ???

        -1,  1,  0, 1,
         1, -1,  1, 0,
         1,  1,  1, 1
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void PostProcessing::InitialShader()
{
    const char* vs = R"(#version 400 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec2 aUV;
    out vec2 uv;
    void main()
    {
        uv = aUV;
        gl_Position = vec4(aPos, 0.0, 1.0);
    })";

    const char* fs = R"(#version 400 core
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
            float g = dot(color, vec3(0.2126, 0.7152, 0.0722)); // ponder on this... why + what does the dot product do?
            color = vec3(g);
        }

        if (invert)
            color = vec3(1.0) - color;

        FragColor = vec4(color, 1.0);
    })";

   /* GLuint ppVertexShader = generateShader("PostProcessing.vs", GL_VERTEX_SHADER);
    GLuint ppFragmentShader = generateShader("PostProcessing.fs", GL_FRAGMENT_SHADER);

    int success;
    char infoLog[512];
    const unsigned int ppShaderProgram = glCreateProgram();
    glAttachShader(ppShaderProgram, ppVertexShader);
    glAttachShader(ppShaderProgram, ppFragmentShader);
    glLinkProgram(ppShaderProgram);*/

    //glUseProgram(ppShaderProgram);

    unsigned int v = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v, 1, &vs, nullptr);
    glCompileShader(v);

    unsigned int f = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f, 1, &fs, nullptr);
    glCompileShader(f);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, v);
    glAttachShader(shaderProgram, f);
    glLinkProgram(shaderProgram);

    glDeleteShader(v);
    glDeleteShader(f);
    //glDeleteShader(ppVertexShader);
    //glDeleteShader(ppFragmentShader);
}
