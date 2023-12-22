#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

extern int SCR_WIDTH;
extern int SCR_HEIGHT;

enum class CameraMovement{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera{
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;

    float movement_speed;
    float mouse_sensitivity;

    float zoom;
    float yaw;
    float pitch;

    void updateCameraVectors();

public:
    Camera(glm::vec3 position=glm::vec3(0.0f, 0.0f, 0.0f), 
        glm::vec3 up=glm::vec3(0.0f, 1.0f, 0.0f), 
        float yaw=-90.0f,
        float pitch=0.0f);
    Camera(float x_pos, float y_pos, float z_pos, 
        float x_up, float y_up, float z_up, 
        float yaw, float pitch);

    glm::vec3 getPosition();
    glm::vec3 getFront();
    glm::mat4 getView();
    glm::mat4 getProjection();

    void processKeyboard(CameraMovement direction, float delta_time);
    void processMouseMovement(float x_ofs, float y_ofs, bool constrain_pitch=true);
    void processMouseScroll(float y_ofs);
};

inline glm::vec3 Camera::getPosition()
{
    return position;
}

inline glm::vec3 Camera::getFront()
{
    return front;
}

inline glm::mat4 Camera::getView()
{
    return glm::lookAt(position, position+front, up);
}

inline glm::mat4 Camera::getProjection()
{
    return glm::perspective(glm::radians(zoom), (float)SCR_WIDTH/SCR_HEIGHT, 0.1f, 100.0f);
}
