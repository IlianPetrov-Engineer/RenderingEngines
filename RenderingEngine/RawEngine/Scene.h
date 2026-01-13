#pragma once
#include <vector>
#include "GameObject.h"
//#include <vector>
//#include "GameObject.h"

class Scene
{
public:
	std::vector<GameObject> objects;

	void Render(const glm::mat4& view, const glm::mat4& projection);
	
	/*virtual ~Scene();

	virtual void OnEnter() {}
	virtual void OnExit() {}

	void Update(float holder);

	void Render();

protected:
	std::vector<GameObject*> objects;*/
};
