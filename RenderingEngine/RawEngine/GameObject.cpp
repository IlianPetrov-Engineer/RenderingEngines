#include "GameObject.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

GameObject::GameObject(const core::Model& model, core::Texture* texture, GLuint shaderProgram, GLint textuteModelUniform, GLint mvpUniform)
    : model(model), texture(texture), shaderProgram(shaderProgram), textuteModelUniform(textuteModelUniform), mvpUniform(mvpUniform)
{}

void GameObject::Render(const glm::mat4& view, const glm::mat4& projection)
{
    glUseProgram(shaderProgram);

    if (texture)
    {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture->getId());
        glUniform1i(textuteModelUniform, 2);
    }

    glUniformMatrix4fv(textuteModelUniform, 1,GL_FALSE,glm::value_ptr(projection * view * model.getModelMatrix()));

    model.render();
}

void GameObject::LitRender(GLuint shader, GLint modelUniform, GLint viewUniform, GLint projectionUniform, GLint objectColorUniform,
    GLint specularStrengthUniform, const glm::mat4& view, const glm::mat4& projection)
{
    glUseProgram(shader);
    glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model.getModelMatrix()));
    glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));

    glm::vec3 color = model.getObjectColor();
    glUniform3f(objectColorUniform, color.x, color.y, color.z);
    glUniform1f(specularStrengthUniform, model.getSpecularStrength());

    model.render();
}
