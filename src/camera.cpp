#include "glm/ext/matrix_transform.hpp"
#include <camera.h>

Camera::Camera(glm::vec3 in_position, glm::vec3 in_front, glm::vec3 in_up) {
    position = in_position;
    front = in_front;
    up = in_up;
};

glm::mat4 Camera::GetViewMatrix() { return glm::lookAt(position, position + front, up); };

void Camera::processMovement(MovementDirection direction, float deltaTime) {
    float moveSpeed = speed * deltaTime;
    if (direction == FORWARD) {
        position += moveSpeed * front;
    }
    if (direction == BACK) {
        position -= moveSpeed * front;
    }
    if (direction == LEFT) {
        position -= glm::normalize(glm::cross(front, up)) * moveSpeed;
    }
    if (direction == RIGHT) {
        position += glm::normalize(glm::cross(front, up)) * moveSpeed;
    }
};

void Camera::processMouse(float xOffset, float yOffset) {
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    yaw = glm::mod(yaw + xOffset, 360.f);
    pitch += yOffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);
};
