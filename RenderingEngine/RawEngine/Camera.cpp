#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
    cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
    cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, -1.0f, 0.0f);
}

void Camera::Variables()
{
    cameraForward = glm::normalize(cameraTarget - cameraPos);
    cameraRight = glm::normalize(glm::cross(cameraForward, up));
    cameraUp = glm::normalize(glm::cross(cameraForward, cameraRight));
}

void Camera::Forward(float fwd)
{
    Variables();
    cameraPos += cameraForward * fwd;
    cameraTarget += cameraForward * fwd;
}

void Camera::Right(float right)
{
    Variables();
    cameraPos += cameraRight * right;
    cameraTarget += cameraRight * right;
}

void Camera::Up(float up)
{
    Variables();
    cameraPos += cameraUp * up;
    cameraTarget += cameraUp * up;
}

void Camera::Rotate(glm::vec3 axis,float rotate)
{
    Variables();
        
    float radians = glm::radians(rotate);
   /* glm::vec3 k = glm::normalize(axis);

    glm::vec3 offset = cameraPos - cameraTarget;

    glm::vec3 rotated =
        offset * cos(radians) +
        glm::cross(k, offset) * sin(radians) +
        k * glm::dot(k, offset) * (1.0f - cos(radians));

    cameraPos = cameraTarget + rotated;

    cameraForward = glm::normalize(cameraTarget - cameraPos);

    cameraRight = glm::normalize(glm::cross(cameraForward, glm::vec3(0, 1, 0)));*/

    /*glm::mat4 rot = glm::rotate(glm::mat4(1.0f), radians, glm::normalize(axis));

    cameraForward = glm::normalize(rot * glm::vec4(cameraForward, 0.0f));

    cameraTarget = cameraPos + cameraForward;

    cameraRight = glm::normalize(glm::cross(cameraForward, up)); */

    glm::vec3 k = glm::normalize(axis);
    glm::vec3 v = cameraForward;

    // Rodrigues formula
    glm::vec3 rotated =
        v * cos(radians) +
        glm::cross(k, v) * sin(radians) +
        k * glm::dot(k, v) * (1.0f - cos(radians));

    cameraForward = glm::normalize(rotated);
    cameraTarget = cameraPos + cameraForward;

    // Recalculate the basis
    cameraRight = glm::normalize(glm::cross(cameraForward, up));
}