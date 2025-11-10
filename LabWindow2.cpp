#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>

float sudutBuka = 0.0f;
bool terbuka = false;
float rotasiY = 0.0f; // sudut rotasi jendela terhadap sumbu Y

// Fungsi menggambar kotak 3D
void kotak(float w, float h, float d, float r, float g, float b, float a = 1.0f) {
    glColor4f(r, g, b, a);
    glBegin(GL_QUADS);

    // depan
    glVertex3f(-w / 2, -h / 2, d / 2);
    glVertex3f(w / 2, -h / 2, d / 2);
    glVertex3f(w / 2, h / 2, d / 2);
    glVertex3f(-w / 2, h / 2, d / 2);

    // belakang
    glVertex3f(-w / 2, -h / 2, -d / 2);
    glVertex3f(-w / 2, h / 2, -d / 2);
    glVertex3f(w / 2, h / 2, -d / 2);
    glVertex3f(w / 2, -h / 2, -d / 2);

    // atas
    glVertex3f(-w / 2, h / 2, -d / 2);
    glVertex3f(-w / 2, h / 2, d / 2);
    glVertex3f(w / 2, h / 2, d / 2);
    glVertex3f(w / 2, h / 2, -d / 2);

    // bawah
    glVertex3f(-w / 2, -h / 2, -d / 2);
    glVertex3f(w / 2, -h / 2, -d / 2);
    glVertex3f(w / 2, -h / 2, d / 2);
    glVertex3f(-w / 2, -h / 2, d / 2);

    // kiri
    glVertex3f(-w / 2, -h / 2, -d / 2);
    glVertex3f(-w / 2, -h / 2, d / 2);
    glVertex3f(-w / 2, h / 2, d / 2);
    glVertex3f(-w / 2, h / 2, -d / 2);

    // kanan
    glVertex3f(w / 2, -h / 2, -d / 2);
    glVertex3f(w / 2, h / 2, -d / 2);
    glVertex3f(w / 2, h / 2, d / 2);
    glVertex3f(w / 2, -h / 2, d / 2);

    glEnd();
}

// Fungsi utama menggambar jendela
void gambarJendela() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    glTranslatef(0.0f, 0.0f, -5.0f);
    glRotatef(20, 1, 0, 0);
    glRotatef(rotasiY, 0, 1, 0); // rotasi dengan tombol panah
    glRotatef(-15, 0, 1, 0);

    float frameThickness = 0.1f;      // tebal bingkai
    float outerW = 1.5f;
    float outerH = 2.5f;
    float outerD = 0.1f;
    float halfW = outerW / 2.0f;
    float halfH = outerH / 2.0f;

    // bingkai kiri
    glPushMatrix();
    glTranslatef(-halfW + frameThickness / 2.0f, 0.0f, 0.0f);
    kotak(frameThickness, outerH, outerD, 1.0f, 1.0f, 1.0f);
    glPopMatrix();

    // bingkai kanan
    glPushMatrix();
    glTranslatef(+halfW - frameThickness / 2.0f, 0.0f, 0.0f);
    kotak(frameThickness, outerH, outerD, 1.0f, 1.0f, 1.0f);
    glPopMatrix();

    // bingkai atas
    glPushMatrix();
    glTranslatef(0.0f, +halfH - frameThickness / 2.0f, 0.0f);
    kotak(outerW - 2.0f * frameThickness, frameThickness, outerD, 1.0f, 1.0f, 1.0f);
    glPopMatrix();

    // bingkai bawah
    glPushMatrix();
    glTranslatef(0.0f, -halfH + frameThickness / 2.0f, 0.0f);
    kotak(outerW - 2.0f * frameThickness, frameThickness, outerD, 1.0f, 1.0f, 1.0f);
    glPopMatrix();

    // daun jendela (bergerak)
    glPushMatrix();
    glTranslatef(0.0f, 1.25f, 0.05f);  // engsel di atas
    glRotatef(sudutBuka, 1, 0, 0);
    glTranslatef(0.0f, -1.25f, 0.0f);

    // bingkai daun jendela
    kotak(1.3f, 2.3f, 0.05f, 0.9f, 0.9f, 0.9f);

    // kaca transparan dua sisi
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.03f);
    kotak(1.1f, 2.1f, 0.01f, 0.3f, 0.6f, 1.0f, 0.4f);
    glPopMatrix();

    glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    glPopMatrix();

    glPopMatrix();
    glutSwapBuffers();
}

// Animasi buka/tutup
void timerFunc(int value) {
    if (terbuka) {
        if (sudutBuka < 70) sudutBuka += 2;
    }
    else {
        if (sudutBuka > 0) sudutBuka -= 2;
    }
    glutPostRedisplay();
    glutTimerFunc(16, timerFunc, 0);
}

// Kontrol keyboard huruf
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'o': case 'O': terbuka = true; break;
    case 'c': case 'C': terbuka = false; break;
    case 27: exit(0); // ESC
    }
}

// Kontrol tombol panah
void specialKeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        rotasiY -= 5.0f; // putar ke kiri
        break;
    case GLUT_KEY_RIGHT:
        rotasiY += 5.0f; // putar ke kanan
        break;
    }
    glutPostRedisplay();
}

// Inisialisasi
void initGL() {
    glClearColor(0.8f, 0.8f, 0.85f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // proyeksi awal
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 1.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / (float)h, 1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900, 700);
    glutCreateWindow("Jendela Lab");

    // ** Initialize GLEW after context creation **
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Using GLEW %s\n", glewGetString(GLEW_VERSION));

    initGL();

    glutDisplayFunc(gambarJendela);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timerFunc, 0);

    glutMainLoop();
    return 0;
}