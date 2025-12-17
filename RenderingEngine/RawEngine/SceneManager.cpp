#include "SceneManager.h"

void SceneManager::SetScene(Scene* newScene)
{
    if (currentScene)
    {
        currentScene->OnExit();
        delete currentScene;
    }

    currentScene = newScene;

    if (currentScene)
        currentScene->OnEnter();
}

void SceneManager::Update(float dt)
{
    if (currentScene)
        currentScene->Update(dt);
}

void SceneManager::Render()
{
    if (currentScene)
        currentScene->Render();
}
