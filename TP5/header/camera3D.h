#ifndef CAMERA_3D_H
#define CAMERA_3D_H

#include "Eigen/Dense"
#include <GL/glut.h>

class Camera3D
{
private:
    Eigen::Vector3f position;
    Eigen::Vector3f right;   // X
    Eigen::Vector3f up;      // Y
    Eigen::Vector3f forward; // Z

    static constexpr float fovAngle = 45.0f;
    static constexpr float nearPlane = 0.1f;
    static constexpr float farPlane = 10000.0f;

public:
    // Constructor
    Camera3D();

    // Getters / Setters
    Eigen::Vector3f getPosition() const { return this->position; }
    Eigen::Vector3f getRightVector() const { return this->right; }
    Eigen::Vector3f getUpVector() const { return this->up; }
    Eigen::Vector3f getForwardVector() const { return this->forward; }
    void setPosition(const Eigen::Vector3f p) { this->position = p; }

    // LookAt
    void lookAt(const Eigen::Vector3f &target);

    // Movement controls
    void move(float dx, float dy, float speed);
    void rotate(float yaw, float pitch, float speed);
    void zoom(float speed);

    // Model / Projection matrix Glut
    void sendModelView() const;
    static void sendProjection(int width, int height);

    // Debug
    void drawDebug() const;
};

#endif
