#pragma once
#include "core/Model.h"
#include "core/texture.h"

class GameObject
{
public:
    core::Model model;
    core::Texture* texture;
    GLuint shaderProgram;
    GLint textuteModelUniform;
    GLint mvpUniform;

    GameObject(const core::Model& model, core::Texture* texture, GLuint shaderProgram, GLint textuteModelUniform, GLint mvpUniform);

    void Render(const glm::mat4& view, const glm::mat4& projection);
};
