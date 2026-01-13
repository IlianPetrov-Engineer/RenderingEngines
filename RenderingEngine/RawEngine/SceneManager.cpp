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

//void SceneManager::SetScene()
//{
//    for (int i = currentSceneIndex; i < 2; i++)
//    {
//        
//    }
//}
//
//void SceneManager::Update(float dt)
//{
//    if (currentScene)
//        currentScene->Update(dt);
//}
//
//void SceneManager::Render()
//{
//    if (currentScene)
//        currentScene->Render();
//}
