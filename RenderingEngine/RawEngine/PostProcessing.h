#pragma once
#include <glad/glad.h>

class PostProcessing
{
public:
    bool grayscale = false;
    bool invert = false;

    void Init(int width, int height);
    void BeginRender();
    void EndRender();

private:
    unsigned int fbo = 0;
    unsigned int colorTexture = 0;
    unsigned int rbo = 0;

    unsigned int quadVAO = 0;
    unsigned int quadVBO = 0;

    unsigned int shaderProgram = 0;

    void initQuad();
    void initShader();
};
