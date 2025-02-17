#include "glm/glm.hpp"

enum MovementDirection { FORWARD, BACK, RIGHT, LEFT };

class Camera {
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    float pitch = 0.0f;
    float yaw = -90.0f;
    float speed = 15.0f;
    float sensitivity = 0.1f;

  public:
    Camera(glm::vec3 in_position, glm::vec3 in_front, glm::vec3 in_up);

    glm::mat4 GetViewMatrix();

    void processMovement(MovementDirection direction, float deltaTime);

    void processMouse(float xOffset, float yOffset);
};
