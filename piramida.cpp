#include <GL/glut.h>

const int NUM_STEPS = 20;
const float STEP_WIDTH = 1.0f;
const float STEP_HEIGHT = 0.2f;
const float STEP_DEPTH = 2.0f;
float rotateX = 0.0f; // Rotasi pada sumbu X
float rotateY = 0.0f; // Rotasi pada sumbu Y
int lastMouseX, lastMouseY; // Posisi terakhir mouse
bool isDragging = false;    // Status apakah mouse sedang di-drag
void drawSingleBlock();

void drawBox(float width, float height, float depth) {
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f);  // Warna merah

    // Depan
    glVertex3f(-width / 2, 0, depth / 2);
    glVertex3f(width / 2, 0, depth / 2);
    glVertex3f(width / 2, height, depth / 2);
    glVertex3f(-width / 2, height, depth / 2);

    // Belakang
    glVertex3f(-width / 2, 0, -depth / 2);
    glVertex3f(width / 2, 0, -depth / 2);
    glVertex3f(width / 2, height, -depth / 2);
    glVertex3f(-width / 2, height, -depth / 2);

    // Kiri
    glVertex3f(-width / 2, 0, -depth / 2);
    glVertex3f(-width / 2, 0, depth / 2);
    glVertex3f(-width / 2, height, depth / 2);
    glVertex3f(-width / 2, height, -depth / 2);

    // Kanan
    glVertex3f(width / 2, 0, -depth / 2);
    glVertex3f(width / 2, 0, depth / 2);
    glVertex3f(width / 2, height, depth / 2);
    glVertex3f(width / 2, height, -depth / 2);

    
    glColor3f(0.5f, 0.0f, 0.0f);  // Warna merah
	// Atas
    glVertex3f(-width / 2, height, -depth / 2);
    glVertex3f(width / 2, height, -depth / 2);
    glVertex3f(width / 2, height, depth / 2);
    glVertex3f(-width / 2, height, depth / 2);
    
    // Bawah
    glVertex3f(-width / 2, 0, -depth / 2);
    glVertex3f(width / 2, 0, -depth / 2);
    glVertex3f(width / 2, 0, depth / 2);
    glVertex3f(-width / 2, 0, depth / 2);

    glEnd();
}

void drawPyramid() {
    float baseWidth = 2.0f;   // Lebar dasar
    float baseDepth = 2.0f;   // Kedalaman dasar
    float height = 0.2f;      // Tinggi setiap tingkat
    float shrinkFactor = 0.9f; // Pengurangan ukuran untuk setiap tingkat
    int levels = 10;          // Jumlah tingkatan

    for (int i = 0; i < levels; i++) {
        glPushMatrix();

        // Naikkan posisi ke atas berdasarkan tingkatan
        glTranslatef(0.0f, i * height, 0.0f);

        // Gambar kotak dengan ukuran yang sesuai
        drawBox(baseWidth, height, baseDepth);
       

        // Kurangi ukuran dasar untuk tingkat berikutnya
        baseWidth *= shrinkFactor;
        baseDepth *= shrinkFactor;

        glPopMatrix();
    }
    
    //depan
    glPushMatrix();
    glTranslatef(0.0f, 0.80f, 0.35f);
    glRotatef(70.0f, 1.0f, 0.0f, 0.0f);
    drawSingleBlock();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.0f, 0.80f, -0.35f); // Belakang
    glRotatef(-70.0f, 1.0f, 0.0f, 0.0f);
    drawSingleBlock();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.35f, 0.80f, 0.0f); // Kiri
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(70.0f, 1.0f, 0.0f, 0.0f);
    drawSingleBlock();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.35f, 0.80f, 0.0f); // Kanan
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(70.0f, 1.0f, 0.0f, 0.0f);
    drawSingleBlock();
    glPopMatrix();
}

void drawSingleBlock() {
    float blockWidth = 0.2f;  // Lebar balok
    float blockHeight = 0.3f; // Tinggi balok
    float blockDepth = 2.2f;  // Kedalaman balok

    // Posisikan balok di tempat tangga sebelumnya
    glTranslatef(0.0f, blockHeight / 2.0f, 0.0f);

    // Gambar balok
    drawBox(blockWidth, blockHeight, blockDepth);    
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = true; // Mulai drag
            lastMouseX = x;
            lastMouseY = y;
        } else if (state == GLUT_UP) {
            isDragging = false; // Hentikan drag
        }
    }
}

void motion(int x, int y) {
    if (isDragging) {
        // Hitung perbedaan posisi mouse
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;

        // Perbarui sudut rotasi berdasarkan pergerakan mouse
        rotateX += dy * 0.2f; // Sensitivitas rotasi pada sumbu X
        rotateY += dx * 0.2f; // Sensitivitas rotasi pada sumbu Y

        // Simpan posisi terakhir mouse
        lastMouseX = x;
        lastMouseY = y;

        // Refresh layar
        glutPostRedisplay();
    }
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(4.0, 3.0, 5.0, 
			  0.0, 1.0, 0.0, 0.0, 1.0, 0.0);

	// Terapkan rotasi berdasarkan input mouse
    glRotatef(rotateX, 1.0f, 0.0f, 0.0f); // Rotasi pada sumbu X
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f); // Rotasi pada sumbu Y
    
    // Gambar piramida
    drawPyramid();

    // Gambar tangga pada sisi piramida
   

    glutSwapBuffers();
}


void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.33, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Tangga OpenGL");
    init();
    
    glutDisplayFunc(display);
    glutMouseFunc(mouse);    // Registrasi fungsi mouse
    glutMotionFunc(motion);  // Registrasi fungsi gerakan mouse
    glutMainLoop();
    return 0;
}
