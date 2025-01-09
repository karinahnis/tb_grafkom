#include <GL/glut.h>
#include <math.h>

// Camera position and orientation
float camX = 2.3f, camY = 4.0f, camZ = 4.7f;
float yaw = -90.0f, pitch = 0.0f;
float lastX = 400, lastY = 300;
bool firstMouse = true;

// Camera direction
float dirX = 0.0f, dirY = 0.0f, dirZ = -1.0f;

int windowWidth = 800;
int windowHeight = 600;
int savedWindowWidth = 800;
int savedWindowHeight = 600;
int savedWindowPosX = 100;
int savedWindowPosY = 100;
bool isFullscreen = false;

const int NUM_STEPS = 20;
const float STEP_WIDTH = 1.0f;
const float STEP_HEIGHT = 0.2f;
const float STEP_DEPTH = 2.0f;
float rotateX = 0.0f; // Rotasi pada sumbu X
float rotateY = 0.0f; // Rotasi pada sumbu Y
float cloudPosition = -10.0f;
void  drawSingleBlock();

void toggleFullscreen() {
    isFullscreen = !isFullscreen;
    if (isFullscreen) {
        // Save current window position and size
        savedWindowPosX = glutGet(GLUT_WINDOW_X);
        savedWindowPosY = glutGet(GLUT_WINDOW_Y);
        savedWindowWidth = glutGet(GLUT_WINDOW_WIDTH);
        savedWindowHeight = glutGet(GLUT_WINDOW_HEIGHT);
        
        // Switch to fullscreen
        glutFullScreen();
    } else {
        // Restore window position and size
        glutReshapeWindow(savedWindowWidth, savedWindowHeight);
        glutPositionWindow(savedWindowPosX, savedWindowPosY);
    }
}

void init() {
    glEnable(GL_DEPTH_TEST);
     glClearColor(0.5, 0.7, 1.0, 1.0); // Warna langit
   
    glutSetCursor(GLUT_CURSOR_NONE);
    glutFullScreen();
    isFullscreen = true;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.33, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

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

// Membuat Piramida
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

// membuat balok miring di tengah tembok piramida
void drawSingleBlock() {
    float blockWidth = 0.2f;  // Lebar balok
    float blockHeight = 0.3f; // Tinggi balok
    float blockDepth = 2.2f;  // Kedalaman balok
     

    // Posisikan balok di tempat tangga sebelumnya
    glTranslatef(0.0f, blockHeight / 2.0f, 0.0f);

    // Gambar balok
    drawBox(blockWidth, blockHeight, blockDepth);    
}

// Membuat alas piramida
void drawPyramidBase(float width, float depth) {
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.5f, 0.0f);  // Warna hijau untuk alas piramida

    // Gambar alas piramida (persegi panjang)
    glVertex3f(-width / 2, -0.1f, depth / 2);  // Titik 1
    glVertex3f(width / 2, -0.1f, depth / 2);   // Titik 2
    glVertex3f(width / 2, -0.1f, -depth / 2);  // Titik 3
    glVertex3f(-width / 2, -0.1f, -depth / 2); // Titik 4

    glEnd();
}

void awan(void){
glPushMatrix(); 
glColor3ub(153, 223, 255);
glutSolidSphere(10, 50, 50);
glPopMatrix();
glPushMatrix();
glTranslatef(10,0,1);
glutSolidSphere(5, 50, 50);
glPopMatrix();   
glPushMatrix();
glTranslatef(-2,6,-2);
glutSolidSphere(7, 50, 50);
glPopMatrix();   
glPushMatrix();
glTranslatef(-10,-3,0);
glutSolidSphere(7, 50, 50);
glPopMatrix();  
glPushMatrix();
glTranslatef(6,-2,2);
glutSolidSphere(7, 50, 50);
glPopMatrix();      

glPushMatrix();
glTranslatef(100,100,-100);  
glScalef(1.8, 1.0, 1.0);
awan();
glPopMatrix();

glPushMatrix();
glTranslatef(90,90,-95);  
glScalef(1.8, 1.0, 1.0);
awan();
glPopMatrix();

glPushMatrix();
glTranslatef(120,95,-95);  
glScalef(1.8, 1.0, 1.0);
awan();
glPopMatrix();

glPushMatrix();
glTranslatef(-100,120,-100);  
glScalef(1.8, 1.0, 1.0);
awan();
glPopMatrix();

glPushMatrix();
glTranslatef(-90,110,-95);  
glScalef(1.8, 1.0, 1.0);
awan();
glPopMatrix();

glPushMatrix();
glTranslatef(-120,115,-95);  
glScalef(1.8, 1.0, 1.0);
awan();
glPopMatrix();
}

void reshape(int w, int h) {
     windowWidth = w;
     windowHeight = h;
     glViewport(0, 0, w, h);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     gluPerspective(45.0f, (float)w / h, 0.1f, 100.0f);
     glMatrixMode(GL_MODELVIEW);
}

void display() {
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glLoadIdentity();

    gluLookAt(camX, camY, camZ,
              camX + dirX, camY + dirY, camZ + dirZ,
              0.0f, 1.0f, 0.0f);

    // Terapkan rotasi berdasarkan input mouse
    glRotatef(rotateX, 1.0f, 0.0f, 0.0f); // Rotasi pada sumbu X
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f); // Rotasi pada sumbu Y

    // Gambar alas piramida lebih besar dari piramida
    drawPyramidBase(3.0f, 3.0f); 
	
	// awan
	awan();  
    
    // Gambar piramida
    drawPyramid();

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    float speed = 0.1f;
    
    switch (key) {
        case 'w':
            camX += dirX * speed;
            camZ += dirZ * speed;
            break;
        case 's':
            camX -= dirX * speed;
            camZ -= dirZ * speed;
            break;
        case 'a':
            camX += dirZ * speed;
            camZ -= dirX * speed;
            break;
        case 'd':
            camX -= dirZ * speed;
            camZ += dirX * speed;
            break;
        case 'f': // Tombol F untuk toggle fullscreen
            toggleFullscreen();
            break;
        case 27:  // ESC key
            if (isFullscreen) {
                toggleFullscreen(); // Keluar dari fullscreen dulu
            } else {
                exit(0); // Keluar program jika sudah dalam mode window
            }
            break;
    }
    glutPostRedisplay();
}

void mouseMove(int x, int y) {
    float xoffset = x - windowWidth/2;
    float yoffset = windowHeight/2 - y;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    dirX = cos(yaw * M_PI / 180.0f) * cos(pitch * M_PI / 180.0f);
    dirY = sin(pitch * M_PI / 180.0f);
    dirZ = sin(yaw * M_PI / 180.0f) * cos(pitch * M_PI / 180.0f);
    
    // Reset mouse position to center of window
    glutWarpPointer(windowWidth/2, windowHeight/2);
    
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
     // Get screen resolution
    windowWidth = glutGet(GLUT_SCREEN_WIDTH);
    windowHeight = glutGet(GLUT_SCREEN_HEIGHT);
    
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Tangga OpenGL");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(mouseMove);

    glutMainLoop();
    return 0;
}  