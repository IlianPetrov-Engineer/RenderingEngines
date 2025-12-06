#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
    cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
    cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    
    yaw = -90.0f;
    pitch = 0.0f;
    mouseSensitivity = 0.1f;
}

void Camera::Variables()
{
    cameraForward = glm::normalize(cameraPos - cameraTarget);
    cameraRight = glm::normalize(glm::cross(up, cameraForward));
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

void Camera::Rotate(float xoffset, float yoffset, bool pitchLimit)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitchLimit)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
   
    cameraForward = glm::normalize(front);
    cameraRight = glm::normalize(glm::cross(up, cameraForward));
    cameraUp = glm::normalize(glm::cross(cameraForward, cameraRight));

    cameraTarget = cameraPos + cameraForward;
}