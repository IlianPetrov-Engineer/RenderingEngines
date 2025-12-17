#include "AllGameObjects.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h> 

extern float rotationStrength;

//Suzanne::Suzanne()
//{
//    model.render();
//}
//
//void Suzanne::Update(float holder)
//{
//    model.rotate(glm::vec3(0, 1, 0),
//        glm::radians(rotationStrength) * holder);
//}
//
//void Suzanne::Render()
//{
//    model.render();
//}
//
//
//QuadImage::QuadImage()
//{
//    core::Mesh quad = core::Mesh::generateQuad();
//    model = core::Model({ quad });
//
//    model.translate(glm::vec3(0, 0, -2.5f));
//    model.scale(glm::vec3(5, 5, 1));
//}
//
//void QuadImage::Render()
//{
//    model.render();
//}
