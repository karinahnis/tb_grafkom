#include <GL/glut.h>
#include <math.h>
#include <cstdlib> 
#include <ctime>   
#define PI 3.14159

// Camera position and orientation
float camX = -3.0f, camY = 3.5f, camZ = -5.5f;
float yaw = 28.0f, pitch = -40.0f;
float lastX = 400, lastY = 300;
bool firstMouse = true;

// Camera direction
float dirX = 0.0f, dirY = 0.0f, dirZ = 0.0f;

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

bool showAxis = false;
void hiddenCarte();

void drawCoordinate() {
    if (!showAxis) return;
    glLineWidth(2.0f);
    
    // Sumbu X (Merah)
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex3f(-10000.0f, 0.0f, 0.0f);
    glVertex3f(10000.0f, 0.0f, 0.0f);
    glEnd();
    
    // Sumbu Y (Hijau)
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex3f(0.0f, -10000.0f, 0.0f);
    glVertex3f(0.0f, 10000.0f, 0.0f);
    glEnd();
    
    // Sumbu Z (Biru)
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, -10000.0f);
    glVertex3f(0.0f, 0.0f, 10000.0f);
    glEnd();
}

// variabel untuk posisi matahari
// posisi awal matahari di bawah layar
float sunPositionY = -1.0f;

float scale = 1.0f;

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
    glEnable(GL_COLOR_MATERIAL);
    glutSetCursor(GLUT_CURSOR_NONE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glClearColor(0.5, 0.7, 1.0, 1.0); // Warna langit
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.33, 0.1, 10000.0);
    glMatrixMode(GL_MODELVIEW);
}

// Membuat Objek Piramida Oleh Karina
void drawBox(float width, float height, float depth) { 
    glBegin(GL_QUADS);
    glColor3ub(204, 200, 197);  

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
    
    glColor3f(0.6f, 0.4f, 0.2);  // Warna merah
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

void drawPyramid(float scale) { 
    float baseWidth = 2.0f;   // Lebar dasar
    float baseDepth = 2.0f;   // Kedalaman dasar
    float height = 0.2f;      // Tinggi setiap tingkat
    float shrinkFactor = 0.9f; // Pengurangan ukuran untuk setiap tingkat
    int levels = 10;          // Jumlah tingkatan
	
	
	glPushMatrix();
	glScalef(scale,scale,scale);
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
    
    // Belakang
    glPushMatrix();
    glTranslatef(0.0f, 0.80f, -0.35f); 
    glRotatef(-70.0f, 1.0f, 0.0f, 0.0f);
    drawSingleBlock();
    glPopMatrix();
	
	// Kiri
    glPushMatrix();
    glTranslatef(0.35f, 0.80f, 0.0f); 
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(70.0f, 1.0f, 0.0f, 0.0f);
    drawSingleBlock();
    glPopMatrix();

	// Kanan
    glPushMatrix();
    glTranslatef(-0.35f, 0.80f, 0.0f); 
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(70.0f, 1.0f, 0.0f, 0.0f);
    drawSingleBlock();
    glPopMatrix();
    
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

void lantai(){
	glPushMatrix();
	glEnable(GL_DEPTH_TEST);
	glColor3f(0.0f, 0.5f, 0.0f); 
	glTranslated(0,-1.0,0);
	glScaled(1000, 0.5, 1000);
	glutSolidSphere(1,30,30);
	glPopMatrix();
}

// Membuat Matahari oleh Rivaldi
// Menambahkan pengaruh matahari terhadap pencahayaan
void updateLightingBasedOnSunPosition() {
    GLfloat lightPosition[] = {5.0f, sunPositionY, 0.0f, 1.0f};  // Posisi cahaya mengikuti matahari
    GLfloat lightIntensity = sunPositionY / 5.0f; // Intensitas cahaya bergantung pada posisi matahari

    // Atur intensitas cahaya sesuai dengan ketinggian matahari
    GLfloat lightDiffuse[] = {lightIntensity, lightIntensity, 0.0f, 1.0f};  // Efek keemasan untuk matahari
    GLfloat lightSpecular[] = {lightIntensity, lightIntensity, 1.0f}; // Warna spekular lebih terang
    GLfloat lightAmbient[] = {lightIntensity * 0.2f, lightIntensity * 0.2f, lightIntensity * 0.2f, 1.0f};  // Efek reduksi cahaya saat matahari di bawah

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);  // Posisi cahaya
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);    // Intensitas cahaya difus
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);  // Intensitas cahaya spekular
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);    // Cahaya ambient
}

// Memepengaruhi posisi matahari
void updateSunPosition(int value) {
	if (sunPositionY < 5.0f) {
		// Gerakan matahari ke atas
		sunPositionY += 0.01f; 
	}
	
	updateLightingBasedOnSunPosition();
	
	glutTimerFunc(16, updateSunPosition, 0);
}

// Mmebuat Matahari 
void drawSun() {
	glPushMatrix();
	glDisable(GL_LIGHTING); // Nonaktifkan pencahayaan agar matahari bersinar
	glTranslatef(2.2f, 1.5f, 3.0f);
	glColor3f(1.0, 1.0, 0.0);
	if (sunPositionY > 0.5) {
		glTranslatef(0.0f, sunPositionY, 0.0f);
		glutSolidSphere(0.5, 20, 20);
	} else if (sunPositionY < 0.0f) {
		sunPositionY = 0.0f;
	}
	glEnable(GL_LIGHTING); // Aktifkan kembali pencahayaan 
	glPopMatrix();	
} 


// Mmebuat Pohon oleh Wildan
void drawTree(float x, float y, float z) {
    // Batang pohon
    glPushMatrix();
    glTranslatef(x, y, z);  // Posisi pohon
    glColor3f(0.5f, 0.25f, 0.1f);  // Warna batang pohon (coklat)
    
    // Rotasi batang agar tegak lurus
    glRotatef(90, 1.0f, 0.0f, 0.0f);  // Rotasi 90 derajat di sumbu X untuk membuat batang tegak
    
    GLUquadric *quadratic = gluNewQuadric();
    gluCylinder(quadratic, 0.1f, 0.1f, 1.0f, 32, 32);  // Membuat silinder untuk batang pohon
    glPopMatrix();

    // Daun pohon (bola hijau)
    glPushMatrix();
    glTranslatef(x, y + 0.0f, z);  // Posisikan bola daun di atas batang pohon
    glColor3f(0.0f, 1.0f, 0.0f);  // Warna daun pohon (hijau)
    glutSolidSphere(0.5f, 20, 20);  // Membuat bola sebagai daun pohon
    glPopMatrix();
}

void drawForest() {
    // Posisi pohon-pohon yang berbeda
    drawTree(10.0f, 1.0f, 2.0f);  // Pohon pertama
    drawTree(5.0f, 2.0f, 2.0f);
    drawTree(3.0f, 3.0f, 2.0f);
    drawTree(-1.0f, 3.0f, 3.0f);
    drawTree(-2.0f, 2.0f, 3.0f);
    drawTree(-3.0f, 1.0f, 3.0f);
    drawTree(5.0f, 1.0f, 3.0f);  
}

 
void reshape(int w, int h) {
     windowWidth = w;
     windowHeight = h;       
     glViewport(0, 0, w, h);  
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0f, windowWidth / windowHeight, 0.1f, 10000000.0f);
    gluLookAt(camX, camY, camZ,
              camX + dirX, camY + dirY, camZ + dirZ,
              0.0f, 1.0f, 0.0f);
    glMatrixMode(GL_MODELVIEW);

    // Terapkan rotasi berdasarkan input mouse
    glRotatef(rotateX, 1.0f, 0.0f, 0.0f); // Rotasi pada sumbu X
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f); // Rotasi pada sumbu Y
    
	
	drawCoordinate();
	
    // Gambar alas piramida lebih besar dari piramida
	lantai();
    
    // matahari 
    drawSun();
                      
    // piramida
    drawPyramid(1.0f);
    
	// Pohon
    drawForest();
    
glPopMatrix();	    
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
        case 'c':
        	showAxis = !showAxis;
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

void hiddenCarte() {
	if (showAxis) {
		void drawCoordinate();
	}
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
    
    // Mulai timer untuk update posisi matahari
    glutTimerFunc(25, updateSunPosition, 0);
   	 
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(mouseMove);

    glutMainLoop();
    return 0;
}  