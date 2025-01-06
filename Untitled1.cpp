#include <GL/glut.h>

// Variabel global untuk sudut rotasi
float angleX = 0.0f, angleY = 0.0f;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Terapkan rotasi berdasarkan sudut
    glPushMatrix();
    glRotatef(angleX, 1.0f, 0.0f, 0.0f); // Rotasi pada sumbu X
    glRotatef(angleY, 0.0f, 1.0f, 0.0f); // Rotasi pada sumbu Y

    float stepHeight = 0.2f;
    float stepWidth = 0.4f;
    float stepDepth = 0.2f;

    // Membuat tangga segitiga dari bawah ke atas
    for (int i = 4; i >= 0; --i) { // Iterasi dari tingkat bawah ke atas
        for (int j = 0; j <= i; ++j) {
            glPushMatrix();

            // Menyesuaikan posisi balok untuk membentuk pola segitiga
            float offsetX = j * stepWidth;
            float offsetY = (4 - i) * stepHeight; // Mulai dari dasar (4 - i)

            glTranslatef(offsetX, offsetY, 0.0f);
            glScalef(stepWidth, stepHeight, stepDepth);
            glutSolidCube(1.0f);
            glPopMatrix();
        }
    }

    glPopMatrix(); // Kembalikan transformasi
    glutSwapBuffers();
}

// Fungsi untuk menangani input keyboard
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': // Rotasi ke atas
            angleX -= 5.0f;
            break;
        case 's': // Rotasi ke bawah
            angleX += 5.0f;
            break;
        case 'a': // Rotasi ke kiri
            angleY -= 5.0f;
            break;
        case 'd': // Rotasi ke kanan
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
    glutInitWindowSize(800, 600);
    glutCreateWindow("Tangga Segitiga 3D dari Bawah");

    // Callback fungsi
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glEnable(GL_DEPTH_TEST); // Aktifkan depth test

    // Atur kamera awal
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f); // Set proyeksi perspektif
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -5.0f); // Jarak awal kamera

    glutMainLoop();

    return 0;
}
