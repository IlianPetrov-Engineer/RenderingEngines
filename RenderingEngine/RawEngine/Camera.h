#pragma once
#include <glm/glm.hpp>

class Camera 
{
 public:
    glm::vec3 cameraPos;
    glm::vec3 cameraTarget;
    glm::vec3 cameraForward;
    glm::vec3 up;
    glm::vec3 cameraRight;
    glm::vec3 cameraUp;
    
    Camera();

    void Variables();

    void Forward(float fwd);

    void Right(float right);

    void Up(float up);

    void Rotate(glm::vec3 axis, float rotate);

    /*void Transform()
    {
        cameraPos == cameraPos + cameraForward;
        cameraTarget == cameraTarget + cameraForward;
    }

    void Rotate(glm::vec3 axis) {
        up = glm::vec3(up);
    }*/

};