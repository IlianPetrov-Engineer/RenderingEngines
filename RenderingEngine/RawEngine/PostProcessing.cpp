#include "PostProcessing.h"
#include "DefaultPrograms.h"
#include <iostream>
#include <algorithm>

void PostProcessing::InitFrameBuffer(GLuint& fbo, GLuint& texture, int width, int height)
{
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_Linear, returns the averages value of the closest group pixels inside a texture (texel), based on the uv 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_Nearest, returns the closest pixel inside a texture (texel), based on the uv 

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessing::Init(int width, int height)
{
    InitFrameBuffer(fboA, texA, width, height);
    InitFrameBuffer(fboB, texB, width, height);

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

    //refers to vertex shader aPos, aUV
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); 

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0);

    GLuint vs = generateShader("PostProcessing.vs", GL_VERTEX_SHADER);
    GLuint fs = generateShader("PostProcessing.fs", GL_FRAGMENT_SHADER);

    {
        GLuint fs = generateShader("GrayscaleShader.fs", GL_FRAGMENT_SHADER);
        grayscaleShader = glCreateProgram();
        glAttachShader(grayscaleShader, vs);
        glAttachShader(grayscaleShader, fs);
        glLinkProgram(grayscaleShader);
        glDeleteShader(fs);
    }

    {
        GLuint fs = generateShader("InvertShader.fs", GL_FRAGMENT_SHADER);
        invertShader = glCreateProgram();
        glAttachShader(invertShader, vs);
        glAttachShader(invertShader, fs);
        glLinkProgram(invertShader);
        glDeleteShader(fs);
    }

    {
        GLuint fs = generateShader("PixelizeShader.fs", GL_FRAGMENT_SHADER);
        pixelShader = glCreateProgram();
        glAttachShader(pixelShader, vs);
        glAttachShader(pixelShader, fs);
        glLinkProgram(pixelShader);
        glDeleteShader(fs);
    }

    {
        GLuint fs = generateShader("EdgeDetectionShader.fs", GL_FRAGMENT_SHADER);
        edgeDetectionShader = glCreateProgram();
        glAttachShader(edgeDetectionShader, vs);
        glAttachShader(edgeDetectionShader, fs);
        glLinkProgram(edgeDetectionShader);
        glDeleteShader(fs);
    }

    {
        GLuint fs = generateShader("FinalShader.fs", GL_FRAGMENT_SHADER);
        finalShader = glCreateProgram();
        glAttachShader(finalShader, vs);
        glAttachShader(finalShader, fs);
        glLinkProgram(finalShader);
        glDeleteShader(fs);
    }

    glDeleteShader(vs);
}

void PostProcessing::BeginRender()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fboA);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLuint PostProcessing::ApplyEffect()
{
    glDisable(GL_DEPTH_TEST);
    GLuint inputTexture = texA; 
    GLuint outputTexture = texB;
    GLuint outputFBO = fboB; 

    auto RunPass = [&](GLuint shader)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, outputFBO);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, inputTexture);
        glUniform1i(glGetUniformLocation(shader, "screenTexture"), 0);

        GLint pixelLoc = glGetUniformLocation(shader, "pixelAmount");
        if (pixelLoc != -1)
        glUniform1f(pixelLoc, pixelAmount);

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        std::swap(inputTexture, outputTexture);

        if(outputFBO == fboA)
            outputFBO = fboB;
        else
            outputFBO = fboA;
    };

    if (grayscale) RunPass(grayscaleShader);
    if (invert)    RunPass(invertShader);
    if (pixelize)  RunPass(pixelShader);
    if (edge)      RunPass(edgeDetectionShader);
    
    return inputTexture;
}

void PostProcessing::EndRender(GLuint texture)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(finalShader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(finalShader, "screenTexture"), 0);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
