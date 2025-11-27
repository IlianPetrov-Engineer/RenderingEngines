#pragma once
#include <glm/glm.hpp>

class Light
{
public:
	glm::vec3 position; // maybe: direction?
	glm::vec4 colour;

	glm::vec3 ambient;
	float ambientIntensity;
	glm::vec3 ambientColour;
	glm::vec3 diffusedColour;

	glm::vec3 diffused;

	glm::vec3 lightDirection;

	Light(glm::vec3 position, glm::vec4 colour)
		: position(position), colour(colour) {};

	void Ambient();

	void Test();
	//~Light() = default;

	//Light& operator = (const Light& anotherLight)
	//{
	//	if (this != &anotherLight)
	//	{
	//		this->position = anotherLight.getPos();
	//		this->colour = anotherLight.getColour();
	//	}
	//	return *this;
	//}
	//glm::vec3 getPos() const;
	//glm::vec4 getColour() const;
};
