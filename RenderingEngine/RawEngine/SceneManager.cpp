#include "SceneManager.h"

Scene& SceneManager::GetCurrentScene()
{
    return scenes[currentSceneIndex];
}

void SceneManager::SetScene(int index)
{
    if (index >= 0 && index < scenes.size())
    {
        currentSceneIndex = index;
    }
}
