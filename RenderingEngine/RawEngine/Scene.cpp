#include "Scene.h"

Scene::~Scene()
{
    for (GameObject* obj : objects)
        delete obj;
}

void Scene::Update(float holder)
{
    for (GameObject* obj : objects)
        obj->Update(holder);
}

void Scene::Render()
{
    for (GameObject* obj : objects)
        obj->Render();
}
