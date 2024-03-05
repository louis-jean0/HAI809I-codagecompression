#include "handleGlut.h"

GLHandler &GLHandler::getInstance()
{
    static GLHandler instance;
    return instance;
}

void GLHandler::run(int screenWidth, int screenHeight, const char *name)
{
    // Init Window
    this->screenWidth_ = screenWidth;
    this->screenHeight_ = screenHeight;
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(this->screenWidth_, this->screenHeight_);
    this->window_ = glutCreateWindow(name);

    // Init Camera
    this->camera_.sendProjection(this->screenWidth_, this->screenHeight_);
    this->camera_.setPosition(Eigen::Vector3f(0.0, 3.0, 4.0));
    this->camera_.lookAt({0.0, 1.0, 0.0});

    // Init Light
    GLfloat ambient[4] = {0.25f, 0.25f, 0.25f, 0.25f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
    GLfloat light_position1[4] = {0.0f, 20.0f, 0.0f, 0.0f};
    GLfloat direction1[3] = {0.0f, -20.0f, 0.0f};
    GLfloat color1[4] = {0.75f, 0.75f, 0.75f, 0.25f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, color1);
    glLightfv(GL_LIGHT0, GL_SPECULAR, color1);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    // Init Glut
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    // Init Wrapper Callback
    glutDisplayFunc(this->displayWrapper);
    glutIdleFunc(this->idleWrapper);
    glutKeyboardFunc(this->keyWrapper);
    glutReshapeFunc(this->reshapeWrapper);
    glutMouseFunc(this->mouseWrapper);
    glutMotionFunc(this->motionWrapper);

    // Loop
    glutMainLoop();
}

///////////////////////////////////////////
////////     CORE FUNCTION GLUT     ///////
///////////////////////////////////////////

void GLHandler::idle()
{
    glutPostRedisplay();
}

void GLHandler::display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPointSize(2.0);

    this->camera_.sendModelView();

    for (const auto &meshPtr : this->meshes_)
    {
        meshPtr->draw(this->displayNormals_);
    }

    glutSwapBuffers();
}

void GLHandler::key(unsigned char keyPressed, int x, int y)
{
    GLint polygonMode[2];
    switch (keyPressed)
    {
    case 'f':
        if (this->fullScreen_ == true)
        {
            glutReshapeWindow(this->screenWidth_, this->screenHeight_);
            this->fullScreen_ = false;
        }
        else
        {
            glutFullScreen();
            this->fullScreen_ = true;
        }
        break;

    case 'w':
        glGetIntegerv(GL_POLYGON_MODE, polygonMode);
        if (polygonMode[0] != GL_FILL)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;

    case 'p':
        glGetIntegerv(GL_POLYGON_MODE, polygonMode);
        if (polygonMode[0] != GL_POINT)
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;

    case 'n':
        this->displayNormals_ = !this->displayNormals_;
        break;

    default:
        break;
    }
    this->idle();
}

void GLHandler::motion(int x, int y)
{
    if (this->mouseRotatePressed == true)
    {
        this->camera_.rotate((x - this->lastX) / (float)this->screenWidth_, (y - this->lastY) / (float)this->screenHeight_, 1.0f);
        this->lastX = x;
        this->lastY = y;
    }
    else if (this->mouseMovePressed == true)
    {
        this->camera_.move((this->lastX - x) / (float)this->screenWidth_, (y - this->lastY) / (float)this->screenHeight_, 1.0f);
        this->lastX = x;
        this->lastY = y;
    }
}

void GLHandler::mouse(int button, int state, int x, int y)
{
    if (state == GLUT_UP)
    {
        this->mouseRotatePressed = false;
        this->mouseMovePressed = false;
    }
    else
    {
        if (button == GLUT_LEFT_BUTTON)
        {
            this->mouseRotatePressed = true;
            this->mouseMovePressed = false;
            this->lastX = x;
            this->lastY = y;
        }
        else if (button == GLUT_RIGHT_BUTTON)
        {
            this->mouseRotatePressed = false;
            this->mouseMovePressed = true;
            this->lastX = x;
            this->lastY = y;
        }
        else if (button == 3 || button == 4)
        {
            // 3 = Scroll Up ; 4 = Scroll Down
            camera_.zoom((button == 3) ? 0.1f : -0.1f);
        }
        this->idle();
    }
}

void GLHandler::reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    this->camera_.sendProjection(w, h);
    this->screenWidth_ = w;
    this->screenHeight_ = h;
}

///////////////////////////////////////////
////////        WRAPPER GLUT        ///////
///////////////////////////////////////////

void GLHandler::idleWrapper()
{
    getInstance().idle();
}
void GLHandler::displayWrapper()
{
    getInstance().display();
}
void GLHandler::keyWrapper(unsigned char keyPressed, int x, int y)
{
    getInstance().key(keyPressed, x, y);
}
void GLHandler::motionWrapper(int x, int y)
{
    getInstance().motion(x, y);
}
void GLHandler::mouseWrapper(int button, int state, int x, int y)
{
    getInstance().mouse(button, state, x, y);
}
void GLHandler::reshapeWrapper(int w, int h)
{
    getInstance().reshape(w, h);
}