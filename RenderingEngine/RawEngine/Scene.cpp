#include "Scene.h"

void Scene::Render(const glm::mat4& view, const glm::mat4& projection)
{
    for (int i = 0; i < objects.size(); i++)
    {
        objects[i].Render(view, projection);
    }
}

void Scene::LitRender(GLuint shader, GLint modelUniform, GLint viewUniform, GLint projectionUniform, GLint objectColorUniform,
    GLint specularStrengthUniform, GLint ambientColourUniform, GLint lightColorUniform, GLint lightPosUniform, GLint viewPosUniform,
    GLint ambientStrengthUniform, GLint diffuseStrengthUniform, const glm::mat4& view, const glm::mat4& projection, const Camera& cam)
{
    glUseProgram(shader);

    glUniform3f(ambientColourUniform, ambientColour.x, ambientColour.y, ambientColour.z);

    glUniform3f(lightColorUniform,lightColor.x, lightColor.y, lightColor.z);

    glUniform3f(lightPosUniform,lightPos.x, lightPos.y, lightPos.z);

    glUniform3f(viewPosUniform,cam.cameraPos.x, cam.cameraPos.y, cam.cameraPos.z);

    glUniform1f(ambientStrengthUniform, ambientStrength);
    glUniform1f(diffuseStrengthUniform, diffuseStrength);

    for (auto& obj : objects)
    {
        obj.LitRender(shader, modelUniform, viewUniform, projectionUniform,
            objectColorUniform,specularStrengthUniform, view, projection);
    }
}
