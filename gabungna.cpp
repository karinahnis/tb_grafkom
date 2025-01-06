#include <GL/glut.h>

// Variabel global untuk sudut rotasi
float angleX = 0.0f, angleY = 0.0f;

void drawCube(float x, float y, float z, float width) {
    glPushMatrix();
    glTranslatef(x, y, z); // Pindahkan posisi kubus
    glScalef(width, 0.5f, width); // Skala untuk lebar horizontal
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawPyramid() {
    float initialWidth = 4.0f; // Lebar awal piramida
    float heightStep = 0.5f;  // Penambahan tinggi setiap tingkat
    float widthStep = 0.5f;   // Pengurangan lebar setiap tingkat
    float currentY = 0.0f;    // Y awal
    for (int i = 0; i < 6; ++i) { // Jumlah tingkat
        drawCube(0.0f, currentY, 0.0f, initialWidth - i * widthStep);
        currentY += heightStep; // Naikkan posisi
    }
}

void drawStairs(float startX, float startZ, float rotationY) {
    glPushMatrix();
    glTranslatef(startX, 0.0f, startZ); // Posisi awal tangga
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f); // Rotasi menghadap sisi piramida

    float stepHeight = 0.2f;
    float stepWidth = 0.4f;
    float stepDepth = 0.2f;

    for (int i = 0; i < 5; ++i) { // 5 tingkat tangga
        glPushMatrix();
        glTranslatef(0.0f, i * stepHeight, i * stepDepth);
        glScalef(stepWidth, stepHeight, stepDepth);
        glutSolidCube(1.0f);
        glPopMatrix();
    }
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Membersihkan buffer warna dan depth
    glLoadIdentity();
    gluLookAt(0.0, 10.0, 10.0, // Posisi kamera
              0.0, 0.0, 0.0,   // Titik yang dilihat
              0.0, 1.0, 0.0);  // Vector up

    glRotatef(angleX, 1.0f, 0.0f, 0.0f); // Rotasi pada sumbu X
    glRotatef(angleY, 0.0f, 1.0f, 0.0f); // Rotasi pada sumbu Y

    drawPyramid(); // Gambar piramida

    // Tangga di sisi depan
    drawStairs(0.0f, -2.5f, 0.0f);
    // Tangga di sisi belakang
    drawStairs(0.0f, 2.5f, 180.0f);
    // Tangga di sisi kiri
    drawStairs(-2.5f, 0.0f, 90.0f);
    // Tangga di sisi kanan
    drawStairs(2.5f, 0.0f, -90.0f);

    glutSwapBuffers(); // Menukar buffer untuk animasi halus
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // Warna latar belakang putih
    glEnable(GL_DEPTH_TEST); // Mengaktifkan depth test
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 20.0); // Perspektif 3D
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            angleX -= 5.0f;
            break;
        case 's':
            angleX += 5.0f;
            break;
        case 'a':
            angleY -= 5.0f;
            break;
        case 'd':
            angleY += 5.0f;
            break;
        case 27: // Escape key
            exit(0);
    }
    glutPostRedisplay(); // Meminta redisplay setelah perubahan sudut
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500); // Ukuran jendela
    glutInitWindowPosition(100, 100); // Posisi jendela
    glutCreateWindow("Piramida dengan Tangga"); // Judul jendela
    init();
    glutDisplayFunc(display); // Callback untuk display
    glutKeyboardFunc(keyboard); // Callback untuk input keyboard
    glutMainLoop(); // Memulai loop GLUT
    return 0;
}
