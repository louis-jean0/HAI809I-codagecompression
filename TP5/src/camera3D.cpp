#include "camera3D.h"

///////////////////////////////////////////
////////         CONSTRUCT          ///////
///////////////////////////////////////////

Camera3D::Camera3D()
{
    this->position = Eigen::Vector3f::Zero();
    this->forward = Eigen::Vector3f::UnitZ();
    this->up = Eigen::Vector3f::UnitY();
    this->right = this->forward.cross(this->up).normalized();
}

///////////////////////////////////////////
////////           LOOKAT           ///////
///////////////////////////////////////////

void Camera3D::lookAt(const Eigen::Vector3f &target)
{
    if (this->position != target)
    {
        this->forward = (target - this->position).normalized();
        this->right = this->forward.cross(Eigen::Vector3f::UnitY()).normalized();
        this->up = this->right.cross(this->forward).normalized();
    }
}

///////////////////////////////////////////
////////         MOVEMENTS          ///////
///////////////////////////////////////////

void Camera3D::move(float dx, float dy, float speed)
{
    this->position += (dx * this->right + dy * this->up) * speed;
}

void Camera3D::rotate(float yaw, float pitch, float speed)
{
    // Adjust yaw and pitch based on speed
    yaw *= speed;
    pitch *= speed;

    // Rotate around the up vector (yaw)
    Eigen::Quaternionf yawRotation(Eigen::AngleAxisf(yaw, Eigen::Vector3f::UnitY()));
    this->forward = yawRotation * this->forward;
    this->up = yawRotation * this->up;

    // Recalculate the right vector after yaw rotation
    this->right = this->forward.cross(this->up).normalized();

    // Rotate around the right vector (pitch)
    Eigen::Quaternionf pitchRotation(Eigen::AngleAxisf(pitch, this->right));
    this->forward = pitchRotation * this->forward;
    this->up = pitchRotation * this->up;
}

void Camera3D::zoom(float speed)
{
    this->position += this->forward * speed;
}

///////////////////////////////////////////
////////            GLUT            ///////
///////////////////////////////////////////

void Camera3D::sendModelView() const
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(this->position.x(), this->position.y(), this->position.z(),
              this->position.x() + this->forward.x(), this->position.y() + this->forward.y(), this->position.z() + this->forward.z(),
              this->up.x(), this->up.y(), this->up.z());
}

void Camera3D::sendProjection(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(fovAngle, (float)width / (float)height, nearPlane, farPlane);
}

///////////////////////////////////////////
////////           DEBUG            ///////
///////////////////////////////////////////

void Camera3D::drawDebug() const
{
    glDisable(GL_LIGHTING);

    Eigen::Vector3f posRight = this->position + this->right;
    Eigen::Vector3f posUp = this->position + this->up;
    Eigen::Vector3f posForward = this->position + this->forward;

    glPointSize(20.0f);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3fv(this->position.data());
    glEnd();

    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f); // X axis
    glVertex3fv(this->position.data());
    glVertex3fv(posRight.data());

    glColor3f(0.0f, 1.0f, 0.0f); // Y axis
    glVertex3fv(this->position.data());
    glVertex3fv(posUp.data());

    glColor3f(0.0f, 0.0f, 1.0f); // Z axis
    glVertex3fv(this->position.data());
    glVertex3fv(posForward.data());
    glEnd();

    glEnable(GL_LIGHTING);
}
