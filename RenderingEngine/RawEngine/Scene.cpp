#include "Scene.h"
//#include 

void Scene::Render(const glm::mat4& view, const glm::mat4& projection)
{
    for (int i = 0; i < objects.size(); i++)
    {
        objects[i].Render(view, projection);
    }
}

//Scene::~Scene()
//{
//    for (GameObject* obj : objects)
//        delete obj;
//}
//
//void Scene::Update(float holder)
//{
//    for (GameObject* obj : objects)
//        obj->Update(holder);
//}
//
//void Scene::Render()
//{
//    for (GameObject* obj : objects)
//        obj->Render();
//}
