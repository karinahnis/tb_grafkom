#include <GL/glut.h>
#include <math.h>

// Kamera
float camX = 2.0f, camY = 2.0f, camZ = 6.0f;
float dirX = 0.0f, dirY = 0.0f, dirZ = -1.0f;

// Ukuran jendela
int windowWidth = 800;
int windowHeight = 600;

// Inisialisasi OpenGL
void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)windowWidth / windowHeight, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

// Gambar kubus
void drawBox(float width, float height, float depth) {
    glBegin(GL_QUADS);

    glColor3f(1.0f, 0.5f, 0.2f);  // Warna oranye
    // Depan
    glVertex3f(-width / 2, 0, depth / 2);
    glVertex3f(width / 2, 0, depth / 2);
    glVertex3f(width / 2, height, depth / 2);
    glVertex3f(-width / 2, height, depth / 2);

    glEnd();
}

// Gambar piramida
void drawPyramid() {
    float baseWidth = 2.0f, baseDepth = 2.0f, height = 0.2f;
    int levels = 10;
    for (int i = 0; i < levels; i++) {
        glPushMatrix();
        glTranslatef(0.0f, i * height, 0.0f);
        drawBox(baseWidth, height, baseDepth);
        baseWidth *= 0.9f;
        baseDepth *= 0.9f;
        glPopMatrix();
    }
}

// Gambar alas
void drawBase() {
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.6f, 0.0f);
    glVertex3f(-3.0f, -0.1f, 3.0f);
    glVertex3f(3.0f, -0.1f, 3.0f);
    glVertex3f(3.0f, -0.1f, -3.0f);
    glVertex3f(-3.0f, -0.1f, -3.0f);
    glEnd();
}

// Fungsi display
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(camX, camY, camZ, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    drawBase();
    drawPyramid();

    glutSwapBuffers();
}

// Fungsi reshape
void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

// Fungsi utama
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Perbaikan Piramida");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
