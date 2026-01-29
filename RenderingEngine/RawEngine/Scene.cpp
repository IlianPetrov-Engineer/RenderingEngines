#include "Scene.h"
#include <glm/gtc/type_ptr.hpp>

void Scene::Render(const Camera& cam, const glm::mat4& view, const glm::mat4& projection)
{
    glUseProgram(shaderProgram);

    glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));

    if (ambientColourUniform != -1)
    {
        glUniform3fv(ambientColourUniform, 1, &ambientColour.x);
        glUniform3fv(lightColorUniform, 1, &lightColor.x);
        glUniform3fv(lightPosUniform, 1, &lightPos.x);
        glUniform3fv(viewPosUniform, 1, &cam.cameraPos.x);
        glUniform1f(ambientStrengthUniform, ambientStrength);
        glUniform1f(diffuseStrengthUniform, diffuseStrength);
    }

    for (auto& object : objects)
    {
        glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(object.model.getModelMatrix()));

        if (objectColorUniform != -1)
        {
            glm::vec3 colour = object.model.getObjectColor();

            glUniform3f(objectColorUniform, colour.x, colour.y, colour.z);
            glUniform1f(specularStrengthUniform, object.model.getSpecularStrength());
        }

        if (object.texture && textureUniform != -1)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, object.texture->getId());
            glUniform1i(textureUniform, 0);
        }

        object.model.render();
    }
}
