#pragma once
#include <glad/glad.h>

class PostProcessing
{
public:
    bool grayscale = false;
    bool invert = false;

    bool pixelize = false;
    float pixelAmount = 64.0f;

    bool edge = false;

    void Init(int width, int height);
    void BeginRender();
    GLuint ApplyEffect();
    void EndRender(GLuint texture);

private:

    GLuint fboA, fboB;
    GLuint texA, texB;
    GLuint rbo;

    GLuint quadVAO, quadVBO;

    GLuint grayscaleShader;
    GLuint invertShader;
    GLuint pixelShader;
    GLuint edgeDetectionShader;
    GLuint finalShader;

    bool multipass = true;

    void InitFrameBuffer(GLuint& fbo, GLuint& texture, int widht, int height);
};
