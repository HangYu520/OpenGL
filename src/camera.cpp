#include "camera.h"

void Camera::adjustCameraPos(Movement direction, float offset)
{
    switch (direction)
    {
    case Movement::FORWARD:
        cameraPos += cameraFront * offset;
        break;
    case Movement::BACKWARD:
        cameraPos -= cameraFront * offset;
        break;
    case Movement::LEFT:
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * offset;
        break;
    case Movement::RIGHT:
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * offset;
        break;
    case Movement::UP:
        cameraPos += cameraUp * offset;
        break;
    case Movement::DOWN:
        cameraPos -= cameraUp * offset;
        break;
    default:
        break;
    }
}

void Camera::adjustCameraFront()
{
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void Camera::adjustYaw(float offset) { yaw += offset;}
    
void Camera::adjustPitch(float offset)
{ 
    pitch += offset;
    if (pitch > 89.0f) pitch = 89.0f;
    else if (pitch < -89.0f) pitch = -89.0f;
}