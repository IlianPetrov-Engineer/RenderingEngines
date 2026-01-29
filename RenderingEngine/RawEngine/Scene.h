#pragma once
#include <vector>
#include "GameObject.h"
#include "Camera.h"

class Scene
{
public:
	std::vector<GameObject> objects;

	glm::vec3 ambientColour{ 0.1f, 0.1f, 0.1f };
	glm::vec3 lightColor{ 1.0f, 1.0f, 1.0f };
	glm::vec3 lightPos{ 0.0f, 3.0f, 0.0f };

	float ambientStrength = 0.1f;
	float diffuseStrength = 1.0f;

	GLuint shaderProgram = 0;
    GLint modelUniform = -1;
    GLint viewUniform = -1;
    GLint projectionUniform = -1;
    GLint objectColorUniform = -1;
    GLint specularStrengthUniform = -1;
    GLint ambientColourUniform = -1;
    GLint lightColorUniform = -1;
    GLint lightPosUniform = -1;
    GLint viewPosUniform = -1;
    GLint ambientStrengthUniform = -1;
    GLint diffuseStrengthUniform = -1;
    GLint textureUniform = -1;

    void Render(const Camera& cam, const glm::mat4& view, const glm::mat4& projection);
};
