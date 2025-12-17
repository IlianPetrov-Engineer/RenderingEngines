#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

struct RenderContext
{
    GLuint textureShader;
    GLuint modelShader;
    GLuint lightShader;

    glm::mat4 projection;
    glm::mat4 view;
};
