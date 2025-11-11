#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cstdlib>
#include <cstdio>

static float windowOpenAngle = 0.0f;
static bool windowIsOpen = false;

static float rotationY = 0.0f;              // angle of rotation around Y axis
static const float automaticRotationSpeed = 20.0f; // degrees per second

static float lastFrameTime = 0.0f;
static float lastManualInputTime = 0.0f;
static const float autoRotationPauseDuration = 1.0f; // seconds to pause auto‐rotation after manual input

static bool spaceKeyPreviouslyDown = false;

void drawBox(float width, float height, float depth,
             float r, float g, float b, float a = 1.0f)
{
    glColor4f(r, g, b, a);
    glBegin(GL_QUADS);
    // front
    glVertex3f(-width/2, -height/2,  depth/2);
    glVertex3f( width/2, -height/2,  depth/2);
    glVertex3f( width/2,  height/2,  depth/2);
    glVertex3f(-width/2,  height/2,  depth/2);
    // back
    glVertex3f(-width/2, -height/2, -depth/2);
    glVertex3f(-width/2,  height/2, -depth/2);
    glVertex3f( width/2,  height/2, -depth/2);
    glVertex3f( width/2, -height/2, -depth/2);
    // top
    glVertex3f(-width/2,  height/2, -depth/2);
    glVertex3f(-width/2,  height/2,  depth/2);
    glVertex3f( width/2,  height/2,  depth/2);
    glVertex3f( width/2,  height/2, -depth/2);
    // bottom
    glVertex3f(-width/2, -height/2, -depth/2);
    glVertex3f( width/2, -height/2, -depth/2);
    glVertex3f( width/2, -height/2,  depth/2);
    glVertex3f(-width/2, -height/2,  depth/2);
    // left
    glVertex3f(-width/2, -height/2, -depth/2);
    glVertex3f(-width/2, -height/2,  depth/2);
    glVertex3f(-width/2,  height/2,  depth/2);
    glVertex3f(-width/2,  height/2, -depth/2);
    // right
    glVertex3f( width/2, -height/2, -depth/2);
    glVertex3f( width/2,  height/2, -depth/2);
    glVertex3f( width/2,  height/2,  depth/2);
    glVertex3f( width/2, -height/2,  depth/2);
    glEnd();
}

void renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    glTranslatef(0.0f, 0.0f, -5.0f);
    glRotatef(20.0f, 1.0f, 0.0f, 0.0f);

    float currentTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
    float sinceManual = currentTime - lastManualInputTime;
    if (sinceManual >= autoRotationPauseDuration) {
        // automatic rotation resumes
        glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    } else {
        // manual override is still active; we still apply rotationY
        glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    }

    glRotatef(-15.0f, 0.0f, 1.0f, 0.0f);

    // window frame
    const float frameThickness = 0.1f;
    const float outerWidth  = 1.5f;
    const float outerHeight = 2.5f;
    const float outerDepth  = 0.1f;
    const float halfW = outerWidth / 2.0f;
    const float halfH = outerHeight / 2.0f;

    // left frame
    glPushMatrix();
      glTranslatef(-halfW + frameThickness/2.0f, 0.0f, 0.0f);
      drawBox(frameThickness, outerHeight, outerDepth, 1.0f, 1.0f, 1.0f);
    glPopMatrix();

    // right frame
    glPushMatrix();
      glTranslatef(+halfW - frameThickness/2.0f, 0.0f, 0.0f);
      drawBox(frameThickness, outerHeight, outerDepth, 1.0f, 1.0f, 1.0f);
    glPopMatrix();

    // top frame
    glPushMatrix();
      glTranslatef(0.0f,  +halfH - frameThickness/2.0f, 0.0f);
      drawBox(outerWidth - 2.0f * frameThickness, frameThickness, outerDepth, 1.0f, 1.0f, 1.0f);
    glPopMatrix();

    // bottom frame
    glPushMatrix();
      glTranslatef(0.0f,  -halfH + frameThickness/2.0f, 0.0f);
      drawBox(outerWidth - 2.0f * frameThickness, frameThickness, outerDepth, 1.0f, 1.0f, 1.0f);
    glPopMatrix();

    // the panel (hinged part) + transparent box behind
    glPushMatrix();
      glTranslatef(0.0f, 1.25f, 0.05f); // hinge at top
      glRotatef(windowOpenAngle, 1.0f, 0.0f, 0.0f);
      glTranslatef(0.0f, -1.25f, 0.0f);

      // Draw only the frame of the panel (i.e., a “hole” inside)
      {
        const float panelTotalW = 1.3f;
        const float panelTotalH = 2.3f;
        const float panelDepth  = 0.05f;
        const float border = 0.1f;   // width of the panel’s own frame
        const float innerW = panelTotalW - 2.0f*border;
        const float innerH = panelTotalH - 2.0f*border;

        // left border
        glPushMatrix();
          glTranslatef(- (panelTotalW/2.0f) + (border/2.0f), 0.0f, 0.0f);
          drawBox(border, panelTotalH, panelDepth, 0.9f, 0.9f, 0.9f);
        glPopMatrix();

        // right border
        glPushMatrix();
          glTranslatef(+ (panelTotalW/2.0f) - (border/2.0f), 0.0f, 0.0f);
          drawBox(border, panelTotalH, panelDepth, 0.9f, 0.9f, 0.9f);
        glPopMatrix();

        // top border
        glPushMatrix();
          glTranslatef(0.0f, + (panelTotalH/2.0f) - (border/2.0f), 0.0f);
          drawBox(panelTotalW - 2.0f*border, border, panelDepth, 0.9f, 0.9f, 0.9f);
        glPopMatrix();

        // bottom border
        glPushMatrix();
          glTranslatef(0.0f, - (panelTotalH/2.0f) + (border/2.0f), 0.0f);
          drawBox(panelTotalW - 2.0f*border, border, panelDepth, 0.9f, 0.9f, 0.9f);
        glPopMatrix();
      }

      // Transparent inner glass (behind the hole/frame)
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDepthMask(GL_FALSE);
      glDisable(GL_CULL_FACE);

        glPushMatrix();
          glTranslatef(0.0f, 0.0f, 0.03f);
          drawBox(1.1f, 2.1f, 0.01f, 0.3f, 0.6f, 1.0f, 0.4f);
        glPopMatrix();

      glEnable(GL_CULL_FACE);
      glDepthMask(GL_TRUE);
      glDisable(GL_BLEND);

    glPopMatrix();

    glPopMatrix();

    glutSwapBuffers();
}

void timerCallback(int value)
{
    float currentTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
    float deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;

    // Automatic rotation if allowed
    if ((currentTime - lastManualInputTime) >= autoRotationPauseDuration) {
        rotationY += automaticRotationSpeed * deltaTime;
        if (rotationY >= 360.0f) rotationY -= 360.0f;
    }

    // Update open/close angle
    const float maxOpenAngle = 70.0f;
    const float openSpeed = 90.0f; // degrees per second
    if (windowIsOpen) {
        if (windowOpenAngle < maxOpenAngle) {
            windowOpenAngle += openSpeed * deltaTime;
            if (windowOpenAngle > maxOpenAngle) windowOpenAngle = maxOpenAngle;
        }
    } else {
        if (windowOpenAngle > 0.0f) {
            windowOpenAngle -= openSpeed * deltaTime;
            if (windowOpenAngle < 0.0f) windowOpenAngle = 0.0f;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, timerCallback, 0);
}

void keyPress(unsigned char key, int x, int y)
{
    if (key == ' ') {
        if (!spaceKeyPreviouslyDown) {
            windowIsOpen = !windowIsOpen;
        }
        spaceKeyPreviouslyDown = true;
    }
    else if (key == 27) { // ESC
        std::exit(EXIT_SUCCESS);
    }
}

void keyRelease(unsigned char key, int x, int y)
{
    if (key == ' ') {
        spaceKeyPreviouslyDown = false;
    }
}

void specialKeyHandler(int key, int x, int y)
{
    switch (key) {
      case GLUT_KEY_LEFT:
        rotationY -= 5.0f;
        lastManualInputTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
        break;
      case GLUT_KEY_RIGHT:
        rotationY += 5.0f;
        lastManualInputTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
        break;
      default:
        break;
    }
    glutPostRedisplay();
}

void setupOpenGL()
{
    glClearColor(0.8f, 0.8f, 0.85f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 1.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
}

void reshapeHandler(int w, int h)
{
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, static_cast<float>(w)/static_cast<float>(h), 1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900, 700);
    glutCreateWindow("Window Animation with Hole in Panel");

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::fprintf(stderr, "GLEW init error: %s\n", glewGetErrorString(err));
        return EXIT_FAILURE;
    }
    std::fprintf(stdout, "Using GLEW %s\n", glewGetString(GLEW_VERSION));

    setupOpenGL();
    lastFrameTime       = static_cast<float>(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
    lastManualInputTime = lastFrameTime;

    glutDisplayFunc(renderScene);
    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyRelease);
    glutSpecialFunc(specialKeyHandler);
    glutReshapeFunc(reshapeHandler);
    glutTimerFunc(0, timerCallback, 0);

    glutMainLoop();
    return EXIT_SUCCESS;
}
