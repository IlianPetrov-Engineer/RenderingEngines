#pragma once
#include "Scene.h"

class SceneManager
{
public:
    void SetScene(Scene* newScene);

    void Update(float holder);
    void Render();

private:
    Scene* currentScene = NULL;
};
