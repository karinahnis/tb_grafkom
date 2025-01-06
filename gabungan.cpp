#include <GL/glut.h>

float angleX = 0.0f, angleY = 0.0f; // Sudut rotasi untuk sumbu X dan Y
float aspect = 1.0;



void drawCube(float x, float y, float z, float width) {
    glPushMatrix();
    glTranslatef(x, y, z); // Pindahkan posisi kubus
    glScalef(width, 0.5f, width); // Skala untuk lebar horizontal
    glBegin(GL_QUADS);
    
    // Persegi depan
    glNormal3f(0.0, 0.0, 1.0); // Normal untuk depan
    glColor3f(1.0, 0.0, 0.0); // Warna merah
    glVertex3f(-1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    
    // Persegi belakang
    glNormal3f(0.0, 0.0, -1.0); // Normal untuk belakang
    glColor3f(0.0, 1.0, 0.0); // Warna hijau
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(1.0f, 0.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    // Persegi kiri
    glNormal3f(-1.0, 0.0, 0.0); // Normal untuk kiri
    glColor3f(0.0, 0.0, 1.0); // Warna biru
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    // Persegi kanan
    glNormal3f(1.0, 0.0, 0.0); // Normal untuk kanan
    glColor3f(1.0, 1.0, 0.0); // Warna kuning
    glVertex3f(1.0f, 0.0f, -1.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    // Persegi atas
    glNormal3f(0.0, 1.0, 0.0); // Normal untuk atas
    glColor3f(1.0, 0.0, 1.0); // Warna ungu
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    // Persegi bawah
    glNormal3f(0.0, -1.0, 0.0); // Normal untuk bawah
    glColor3f(0.0, 1.0, 1.0); // Warna cyan
    glVertex3f(-1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glEnd();

    glPopMatrix();
    
    
    glColor3f(0.0f, 0.0f, 0.0f); // Warna hitam untuk garis
    glBegin(GL_LINES);
    
    // Garis depan
    glVertex3f(-1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);

    glVertex3f(1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 0.0f, 1.0f);

    // Garis belakang
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(1.0f, 0.0f, -1.0f);

    glVertex3f(1.0f, 0.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 0.0f, -1.0f);

    // Hubungkan depan dan belakang
    glVertex3f(-1.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 0.0f, -1.0f);

    glVertex3f(1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 0.0f, -1.0f);

    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glEnd();

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

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Membersihkan buffer warna dan depth

    glLoadIdentity();
    gluLookAt(0.0, 10.0, 10.0, // Posisi kamera
              0.0, 0.0, 0.0,   // Titik yang dilihat
              0.0, 1.0, 0.0);  // Vector up

    glRotatef(angleX, 1.0f, 0.0f, 0.0f); // Rotasi pada sumbu X
    glRotatef(angleY, 0.0f, 1.0f, 0.0f); // Rotasi pada sumbu Y

    drawPyramid(); // Gambar piramida
    glutSwapBuffers(); // Menukar buffer untuk animasi halus
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // Warna latar belakang putih
    glEnable(GL_DEPTH_TEST); // Mengaktifkan depth test
    glEnable(GL_LIGHTING); // Mengaktifkan pencahayaan
    glEnable(GL_LIGHT0); // Mengaktifkan sumber cahaya pertama
    
    GLfloat light_diffuse[] = {0.0f, 1.0f, 1.0f, 1.0f}; // Warna cahaya putih
    GLfloat light_position[] = {5.0f, 5.0f, 5.0f, 1.0f}; // Posisi cahaya
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glClearColor(1.0, 1.0, 1.0, 1.0); // Warna latar belakang putih
    glEnable(GL_DEPTH_TEST); // Mengaktifkan depth test
    glOrtho(-aspect, aspect, -1.0, 1.0, -1.0, 1.0);
    
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
    glutInitWindowSize(1000, 600); // Ukuran jendela
    glutInitWindowPosition(200, -200); // Posisi jendela
    glutCreateWindow("Piramida 3D"); // Judul jendela

    init();
    glutDisplayFunc(display); // Callback untuk display
    glutKeyboardFunc(keyboard); // Callback untuk input keyboard
    glutMainLoop(); // Memulai loop GLUT

    return 0;
}

