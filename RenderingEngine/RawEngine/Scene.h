#pragma once
#include <vector>
#include "GameObject.h"
#include "Camera.h"
//#include <vector>
//#include "GameObject.h"

class Scene
{
public:
	std::vector<GameObject> objects;

	glm::vec3 ambientColour{ 0.1f, 0.1f, 0.1f };
	glm::vec3 lightColor{ 1.0f, 1.0f, 1.0f };
	glm::vec3 lightPos{ 0.0f, 3.0f, 0.0f };

	float ambientStrength = 0.1f;
	float diffuseStrength = 1.0f;

	void Render(const glm::mat4& view, const glm::mat4& projection);

	void LitRender(GLuint shader, GLint modelUniform, GLint viewUniform, GLint projectionUniform,GLint objectColorUniform, 
		GLint specularStrengthUniform, GLint ambientColourUniform, GLint lightColorUniform, GLint lightPosUniform,GLint viewPosUniform,
        GLint ambientStrengthUniform, GLint diffuseStrengthUniform, const glm::mat4& view, const glm::mat4& projection, const Camera& cam);
	
	/*virtual ~Scene();

	virtual void OnEnter() {}
	virtual void OnExit() {}

	void Update(float holder);

	void Render();

protected:
	std::vector<GameObject*> objects;*/
};
