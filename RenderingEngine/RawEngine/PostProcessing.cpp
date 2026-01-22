#include "PostProcessing.h"
#include <iostream>
#include "DefaultPrograms.h"

void PostProcessing::Initialise(int width, int height) //rework the whole pp and look into fbo
{
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo); 

    //stores the rendered scene in as a texture
    glGenTextures(1, &colourTexture);
    glBindTexture(GL_TEXTURE_2D, colourTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourTexture, 0); //attach the texture to the framebuffer

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "PostProcessing framebuffer is not complete\n";

    glBindFramebuffer(GL_FRAMEBUFFER, 0); //returns the render to the screen

    InitialQuad(); //screen wide quad
    InitialShader(); //pp logic
}

void PostProcessing::Begin()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo); //returns the scene as a texture
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

    glUniform1i(glGetUniformLocation(shaderProgram, "pixelize"), pixelize);
    glUniform1f(glGetUniformLocation(shaderProgram, "pixelAmount"), pixelAmount);
    glUniform1i(glGetUniformLocation(shaderProgram, "edge"), edge);

    glActiveTexture(GL_TEXTURE0); //the currently rendered scene is used for post processing    
    glBindTexture(GL_TEXTURE_2D, colourTexture);
    glUniform1i(glGetUniformLocation(shaderProgram, "screenTexture"), 0);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void PostProcessing::InitialQuad()
{
    float quadVertices[] = {
        -1,  1,  0, 1, 
        -1, -1,  0, 0,
         1, -1,  1, 0, 

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
    GLuint vs = generateShader("PostProcessing.vs", GL_VERTEX_SHADER);
    GLuint fs = generateShader("PostProcessing.fs", GL_FRAGMENT_SHADER);

    int success;
    char infoLog[512];
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("Error! Making Shader Program: %s\n", infoLog);
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
}
