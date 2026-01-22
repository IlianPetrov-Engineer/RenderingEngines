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

    void Initialise(int width, int height);
    void Begin();
    void End();

private:
    unsigned int fbo = 0; //FrameBuffer Object - offscreen rendering
    unsigned int colourTexture = 0; //stores the final image
    unsigned int rbo = 0; //RenderBuffer Object

    unsigned int quadVAO = 0;
    unsigned int quadVBO = 0;

    unsigned int shaderProgram = 0;

    void InitialQuad();
    void InitialShader();
};
