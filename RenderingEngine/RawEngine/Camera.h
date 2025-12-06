#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>

class Camera
{
public:
    glm::vec3 cameraPos;
    glm::vec3 cameraTarget;
    glm::vec3 cameraForward;
    glm::vec3 up;
    glm::vec3 cameraRight;
    glm::vec3 cameraUp;
    float yaw;
    float pitch;
    float mouseSensitivity;

    Camera();

    void Variables();

    void Forward(float fwd);

    void Right(float right);

    void Up(float up);

    void Rotate(float xoffset, float yoffset, bool pitchLimit = true);
};