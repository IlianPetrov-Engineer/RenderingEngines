#include "GameObject.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

GameObject::GameObject(const core::Model& model, core::Texture* texture, GLuint shaderProgram, GLint textuteModelUniform, GLint mvpUniform)
    : model(model), texture(texture), shaderProgram(shaderProgram), textuteModelUniform(textuteModelUniform), mvpUniform(mvpUniform)
{
}

void GameObject::Render(const glm::mat4& view, const glm::mat4& projection)
{
    glUseProgram(shaderProgram);

    if (texture)
    {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture->getId());
        glUniform1i(textuteModelUniform, 2);
    }

    glUniformMatrix4fv(textuteModelUniform, 1,GL_FALSE,glm::value_ptr(projection * view * model.getModelMatrix()));

    model.render();
}


//#include "AllGameObjects.h"
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <glad/glad.h> 
//
//extern float rotationStrength;

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
