
#ifndef HANDLE_GLUT_H
#define HANDLE_GLUT_H

#include <vector>

#include <GL/glut.h>

#include "camera3D.h"
#include "mesh.h"

class GLHandler
{
private:
    // Glut
    unsigned int screenWidth_;
    unsigned int screenHeight_;
    int window_;

    // Camera
    Camera3D camera_;
    bool mouseMovePressed = false;
    bool mouseRotatePressed = false;
    float lastX = 0.0f, lastY = 0.0f;

    // Options
    bool fullScreen_ = false;
    bool displayNormals_ = false;

    // Meshes
    std::vector<const Mesh *> meshes_;

    ///////////////////////////////////////////
    ////////     SINGLETON  PATTERN     ///////
    ///////////////////////////////////////////

    GLHandler() = default;
    ~GLHandler() = default;
    GLHandler(const GLHandler &) = delete;
    GLHandler &operator=(const GLHandler &) = delete;

    ///////////////////////////////////////////
    ////////     CORE FUNCTION GLUT     ///////
    ///////////////////////////////////////////

    void idle();
    void display();
    void key(unsigned char keyPressed, int x, int y);
    void motion(int x, int y);
    void mouse(int button, int state, int x, int y);
    void reshape(int w, int h);

    ///////////////////////////////////////////
    ////////        WRAPPER GLUT        ///////
    ///////////////////////////////////////////

    static void idleWrapper();
    static void displayWrapper();
    static void keyWrapper(unsigned char keyPressed, int x, int y);
    static void motionWrapper(int x, int y);
    static void mouseWrapper(int button, int state, int x, int y);
    static void reshapeWrapper(int w, int h);

public:
    static GLHandler &getInstance();

    void addMesh(const Mesh *mesh) { meshes_.push_back(mesh); }
    void run(int screenWidth, int screenHeight, const char *name);
};

#endif
