#pragma once
#include <vector>
#include "GameObject.h"

class Scene
{
public:
	virtual ~Scene();

	virtual void OnEnter() {}
	virtual void OnExit() {}

	void Update(float holder);

	void Render();

protected:
	std::vector<GameObject*> objects;
};
