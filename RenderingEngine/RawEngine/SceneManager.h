#pragma once
#include "Scene.h"
#include <vector>

class SceneManager
{
public:
    std::vector<Scene> scenes;
    int currentSceneIndex = 0;

    Scene& GetCurrentScene();
    void SetScene(int index);
};
