#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include "imageloader.h"
#include "vec3f.h"
#endif



static GLfloat spin, spin2 = 0.0;
float angle = 0;
using namespace std;

float lastx, lasty;
GLint stencilBits;
static int viewx = 0;
static int viewy = 100;
static int viewz = 150;

float rot = 0;






















GLuint texture[2]; //array untuk texture

//GLint slices = 16;
//GLint stacks = 16;

struct Images {
	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
};
typedef struct Images Images;











//class untuk terain 2D
class Terrain {
private:
	int w; //Width
	int l; //Length
	float** hs; //Heights
	Vec3f** normals;
	bool computedNormals; //Whether normals is up-to-date
public:
	Terrain(int w2, int l2) {
		w = w2;
		l = l2;

		hs = new float*[l];
		for (int i = 0; i < l; i++) {
			hs[i] = new float[w];
		}

		normals = new Vec3f*[l];
		for (int i = 0; i < l; i++) {
			normals[i] = new Vec3f[w];
		}

		computedNormals = false;
	}

	~Terrain() {
		for (int i = 0; i < l; i++) {
			delete[] hs[i];
		}
		delete[] hs;

		for (int i = 0; i < l; i++) {
			delete[] normals[i];
		}
		delete[] normals;
	}

	int width() {
		return w;
	}

	int length() {
		return l;
	}

	//Sets the height at (x, z) to y
	void setHeight(int x, int z, float y) {
		hs[z][x] = y;
		computedNormals = false;
	}

	//Returns the height at (x, z)
	float getHeight(int x, int z) {
		return hs[z][x];
	}

	//Computes the normals, if they haven't been computed yet
	void computeNormals() {
		if (computedNormals) {
			return;
		}

		//Compute the rough version of the normals
		Vec3f** normals2 = new Vec3f*[l];
		for (int i = 0; i < l; i++) {
			normals2[i] = new Vec3f[w];
		}

		for (int z = 0; z < l; z++) {
			for (int x = 0; x < w; x++) {
				Vec3f sum(0.0f, 0.0f, 0.0f);

				Vec3f out;
				if (z > 0) {
					out = Vec3f(0.0f, hs[z - 1][x] - hs[z][x], -1.0f);
				}
				Vec3f in;
				if (z < l - 1) {
					in = Vec3f(0.0f, hs[z + 1][x] - hs[z][x], 1.0f);
				}
				Vec3f left;
				if (x > 0) {
					left = Vec3f(-1.0f, hs[z][x - 1] - hs[z][x], 0.0f);
				}
				Vec3f right;
				if (x < w - 1) {
					right = Vec3f(1.0f, hs[z][x + 1] - hs[z][x], 0.0f);
				}

				if (x > 0 && z > 0) {
					sum += out.cross(left).normalize();
				}
				if (x > 0 && z < l - 1) {
					sum += left.cross(in).normalize();
				}
				if (x < w - 1 && z < l - 1) {
					sum += in.cross(right).normalize();
				}
				if (x < w - 1 && z > 0) {
					sum += right.cross(out).normalize();
				}

				normals2[z][x] = sum;
			}
		}

		//Smooth out the normals
		const float FALLOUT_RATIO = 0.5f;
		for (int z = 0; z < l; z++) {
			for (int x = 0; x < w; x++) {
				Vec3f sum = normals2[z][x];

				if (x > 0) {
					sum += normals2[z][x - 1] * FALLOUT_RATIO;
				}
				if (x < w - 1) {
					sum += normals2[z][x + 1] * FALLOUT_RATIO;
				}
				if (z > 0) {
					sum += normals2[z - 1][x] * FALLOUT_RATIO;
				}
				if (z < l - 1) {
					sum += normals2[z + 1][x] * FALLOUT_RATIO;
				}

				if (sum.magnitude() == 0) {
					sum = Vec3f(0.0f, 1.0f, 0.0f);
				}
				normals[z][x] = sum;
			}
		}

		for (int i = 0; i < l; i++) {
			delete[] normals2[i];
		}
		delete[] normals2;

		computedNormals = true;
	}

	//Returns the normal at (x, z)
	Vec3f getNormal(int x, int z) {
		if (!computedNormals) {
			computeNormals();
		}
		return normals[z][x];
	}
};
//end class






//Loads a terrain from a heightmap.  The heights of the terrain range from
//-height / 2 to height / 2.
//load terain di procedure inisialisasi
Terrain* loadTerrain(const char* filename, float height) {
	Image* image = loadBMP(filename);
	Terrain* t = new Terrain(image->width, image->height);
	for (int y = 0; y < image->height; y++) {
		for (int x = 0; x < image->width; x++) {
			unsigned char color = (unsigned char) image->pixels[3 * (y
					* image->width + x)];
			float h = height * ((color / 255.0f) - 0.5f);
			t->setHeight(x, y, h);
		}
	}

	delete image;
	t->computeNormals();
	return t;
}

float _angle = 60.0f;
//buat tipe data terain
Terrain* _terrain;
Terrain* _terrainTanah;
Terrain* _terrainAir;


void cleanup() {
	delete _terrain;
	delete _terrainTanah;
	delete _terrainAir;
}









//mengambil gambar BMP
int ImageLoad(char *filename, Images *image) {
	FILE *file;
	unsigned long size; // ukuran image dalam bytes
	unsigned long i; // standard counter.
	unsigned short int plane; // number of planes in image

	unsigned short int bpp; // jumlah bits per pixel
	char temp; // temporary color storage for var warna sementara untuk memastikan filenya ada


	if ((file = fopen(filename, "rb")) == NULL) {
		printf("File Not Found : %s\n", filename);
		return 0;
	}
	// mencari file header bmp
	fseek(file, 18, SEEK_CUR);
	// read the width
	if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
		printf("Error reading width from %s.\n", filename);
		return 0;
	}
	//printf("Width of %s: %lu\n", filename, image->sizeX);
	// membaca nilai height
	if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
		printf("Error reading height from %s.\n", filename);
		return 0;
	}
	//printf("Height of %s: %lu\n", filename, image->sizeY);
	//menghitung ukuran image(asumsi 24 bits or 3 bytes per pixel).

	size = image->sizeX * image->sizeY * 3;
	// read the planes
	if ((fread(&plane, 2, 1, file)) != 1) {
		printf("Error reading planes from %s.\n", filename);
		return 0;
	}
	if (plane != 1) {
		printf("Planes from %s is not 1: %u\n", filename, plane);
		return 0;
	}
	// read the bitsperpixel
	if ((i = fread(&bpp, 2, 1, file)) != 1) {
		printf("Error reading bpp from %s.\n", filename);

		return 0;
	}
	if (bpp != 24) {
		printf("Bpp from %s is not 24: %u\n", filename, bpp);
		return 0;
	}
	// seek past the rest of the bitmap header.
	fseek(file, 24, SEEK_CUR);
	// read the data.
	image->data = (char *) malloc(size);
	if (image->data == NULL) {
		printf("Error allocating memory for color-corrected image data");
		return 0;
	}
	if ((i = fread(image->data, size, 1, file)) != 1) {
		printf("Error reading image data from %s.\n", filename);
		return 0;
	}
	for (i = 0; i < size; i += 3) { // membalikan semuan nilai warna (gbr - > rgb)
		temp = image->data[i];
		image->data[i] = image->data[i + 2];
		image->data[i + 2] = temp;
	}
	// we're done.
	return 1;
}




//mengambil tekstur
Images * loadTexture() {
	Images *image1;
	// alokasi memmory untuk tekstur
	image1 = (Images *) malloc(sizeof(Images));
	if (image1 == NULL) {
		printf("Error allocating space for image");
		exit(0);
	}
	//pic.bmp is a 64x64 picture
	if (!ImageLoad("grass.bmp", image1)) {
		exit(1);
	}
	return image1;
}


Images * loadTextureSatu() {
	Images *image1;
	// alokasi memmory untuk tekstur
	image1 = (Images *) malloc(sizeof(Images));
	if (image1 == NULL) {
		printf("Error allocating space for image");
		exit(0);
	}
	//pic.bmp is a 64x64 picture
	if (!ImageLoad("balon.bmp", image1)) {
		exit(1);
	}
	return image1;
}

Images * loadTextureDua() {
	Images *image1;
	// alokasi memmory untuk tekstur
	image1 = (Images *) malloc(sizeof(Images));
	if (image1 == NULL) {
		printf("Error allocating space for image");
		exit(0);
	}
	//pic.bmp is a 64x64 picture
	if (!ImageLoad("kotak.bmp", image1)) {
		exit(1);
	}
	return image1;
}

















void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
}







void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*
	 glMatrixMode(GL_MODELVIEW);
	 glLoadIdentity();
	 glTranslatef(0.0f, 0.0f, -10.0f);
	 glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
	 glRotatef(-_angle, 0.0f, 1.0f, 0.0f);

	 GLfloat ambientColor[] = {0.4f, 0.4f, 0.4f, 1.0f};
	 glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	 GLfloat lightColor0[] = {0.6f, 0.6f, 0.6f, 1.0f};
	 GLfloat lightPos0[] = {-0.5f, 0.8f, 0.1f, 0.0f};
	 glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	 glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	 */
	float scale = 500.0f / max(_terrain->width() - 1, _terrain->length() - 1);
	glScalef(scale, scale, scale);
	glTranslatef(-(float) (_terrain->width() - 1) / 2, 0.0f,
			-(float) (_terrain->length() - 1) / 2);

	glColor3f(0.3f, 0.9f, 0.0f);
	for (int z = 0; z < _terrain->length() - 1; z++) {
		//Makes OpenGL draw a triangle at every three consecutive vertices
		glBegin(GL_TRIANGLE_STRIP);
		for (int x = 0; x < _terrain->width(); x++) {
			Vec3f normal = _terrain->getNormal(x, z);
			glNormal3f(normal[0], normal[1], normal[2]);
			glVertex3f(x, _terrain->getHeight(x, z), z);
			normal = _terrain->getNormal(x, z + 1);
			glNormal3f(normal[0], normal[1], normal[2]);
			glVertex3f(x, _terrain->getHeight(x, z + 1), z + 1);
		}
		glEnd();
	}

}


//untuk di display
void drawSceneTanah(Terrain *terrain, GLfloat r, GLfloat g, GLfloat b) {
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*
	 glMatrixMode(GL_MODELVIEW);
	 glLoadIdentity();
	 glTranslatef(0.0f, 0.0f, -10.0f);
	 glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
	 glRotatef(-_angle, 0.0f, 1.0f, 0.0f);

	 GLfloat ambientColor[] = {0.4f, 0.4f, 0.4f, 1.0f};
	 glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	 GLfloat lightColor0[] = {0.6f, 0.6f, 0.6f, 1.0f};
	 GLfloat lightPos0[] = {-0.5f, 0.8f, 0.1f, 0.0f};
	 glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	 glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	 */
	float scale = 500.0f / max(terrain->width() - 1, terrain->length() - 1);
	glScalef(scale, scale, scale);
	glTranslatef(-(float) (terrain->width() - 1) / 2, 0.0f,
			-(float) (terrain->length() - 1) / 2);

	glColor3f(r, g, b);
	for (int z = 0; z < terrain->length() - 1; z++) {
		//Makes OpenGL draw a triangle at every three consecutive vertices
		glBegin(GL_TRIANGLE_STRIP);
		for (int x = 0; x < terrain->width(); x++) {
			Vec3f normal = terrain->getNormal(x, z);
			glNormal3f(normal[0], normal[1], normal[2]);
			glVertex3f(x, terrain->getHeight(x, z), z);
			normal = terrain->getNormal(x, z + 1);
			glNormal3f(normal[0], normal[1], normal[2]);
			glVertex3f(x, terrain->getHeight(x, z + 1), z + 1);
		}
		glEnd();
	}

}








void update(int value) {

	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}


void freetexture(GLuint texture) {
	glDeleteTextures(2, &texture);
}





























const GLfloat light_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
const GLfloat light_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 1.0f };

const GLfloat light_ambient2[] = { 0.3f, 0.3f, 0.3f, 0.0f };
const GLfloat light_diffuse2[] = { 0.3f, 0.3f, 0.3f, 0.0f };

const GLfloat mat_ambient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/*
unsigned int texture_balon;
unsigned int texture_kotak;
unsigned int LoadTextureFromBmpFile(char *filename);
*/


void matahari(void){
glPushMatrix();
glTranslatef(150,150,-150);  
glColor3ub(255, 253, 116);
glutSolidSphere(10, 50, 50);
glPopMatrix();
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
}


void balonatas(void){
glPushMatrix();
glTranslatef(0,102,0);
glutSolidSphere(20,20,50);
glPopMatrix();
}



void balonbawah(void){
GLUquadricObj *pObj;
pObj =gluNewQuadric();
gluQuadricNormals(pObj, GLU_SMOOTH);    

glPushMatrix();
glTranslatef(0,92,0);  
glRotatef(90,1,0,0);
gluCylinder(pObj, 17, 6, 22, 25, 25);
glPopMatrix();
}



void kotakatas(void){
glPushMatrix();
glTranslatef(0,65.2,0);
glScalef(1, 0.1, 1);
glutSolidCube(8);
glPopMatrix();
}

void kotak1(void){
glPushMatrix();
glTranslatef(0,58,0);
glScalef(1, 0.05, 1);
glutSolidCube(8);
glPopMatrix();
}

void kotak2(void){
glPushMatrix();
glTranslatef(3.7,60,0);
glScalef(0.05, 0.5, 1);
glutSolidCube(8);
glPopMatrix();
}

void kotak3(void){
glPushMatrix();
glTranslatef(-3.7,60,0);
glScalef(0.05, 0.5, 1);
glutSolidCube(8);
glPopMatrix();
}

void kotak4(void){
glPushMatrix();
glTranslatef(0,60,3.7);
glScalef(1, 0.5, 0.05);
glutSolidCube(8);
glPopMatrix();
}

void kotak5(void){
glPushMatrix();
glTranslatef(0,60,-3.7);
glScalef(1, 0.5, 0.05);
glutSolidCube(8);
glPopMatrix();
}


void tiang1(void){
GLUquadricObj *pObj;
pObj =gluNewQuadric();
gluQuadricNormals(pObj, GLU_SMOOTH); 
glPushMatrix();
glTranslatef(3.5,65,3.5);
glRotatef(90,1,0,0);
gluCylinder(pObj, 0.3, 0.3, 3, 25, 25);
glPopMatrix();
}


void tiang2(void){
GLUquadricObj *pObj;
pObj =gluNewQuadric();
gluQuadricNormals(pObj, GLU_SMOOTH); 
glPushMatrix();
glTranslatef(3.5,65,-3.5);
glRotatef(90,1,0,0);
gluCylinder(pObj, 0.3, 0.3, 3, 25, 25);
glPopMatrix();
}

void tiang3(void){
GLUquadricObj *pObj;
pObj =gluNewQuadric();
gluQuadricNormals(pObj, GLU_SMOOTH); 
glPushMatrix();
glTranslatef(-3.5,65,3.5);
glRotatef(90,1,0,0);
gluCylinder(pObj, 0.3, 0.3, 3, 25, 25);
glPopMatrix();
}


void tiang4(void){
GLUquadricObj *pObj;
pObj =gluNewQuadric();
gluQuadricNormals(pObj, GLU_SMOOTH); 
glPushMatrix();
glTranslatef(-3.5,65,-3.5);
glRotatef(90,1,0,0);
gluCylinder(pObj, 0.3, 0.3, 3, 25, 25);
glPopMatrix();
}


void tiang5(void){
GLUquadricObj *pObj;
pObj =gluNewQuadric();
gluQuadricNormals(pObj, GLU_SMOOTH); 
glPushMatrix();
glTranslatef(3.5,64.8,-3.5);
glRotatef(-90,0,1,0);
gluCylinder(pObj, 0.3, 0.3, 7.2, 25, 25);
glPopMatrix();
}


void tiang6(void){
GLUquadricObj *pObj;
pObj =gluNewQuadric();
gluQuadricNormals(pObj, GLU_SMOOTH); 
glPushMatrix();
glTranslatef(3.5,64.8,-3.5);
glRotatef(0,1,0,0);
gluCylinder(pObj, 0.3, 0.3, 7.2, 25, 25);
glPopMatrix();
}


void tiang7(void){
GLUquadricObj *pObj;
pObj =gluNewQuadric();
gluQuadricNormals(pObj, GLU_SMOOTH); 
glPushMatrix();
glTranslatef(-3.5,64.8,3.5);
glRotatef(90,0,1,0);
gluCylinder(pObj, 0.3, 0.3, 7.2, 25, 25);
glPopMatrix();
}

void tiang8(void){
GLUquadricObj *pObj;
pObj =gluNewQuadric();
gluQuadricNormals(pObj, GLU_SMOOTH); 
glPushMatrix();
glTranslatef(-3.5,64.8,-3.5);
glRotatef(0,1,0,0);
gluCylinder(pObj, 0.3, 0.3, 7.2, 25, 25);
glPopMatrix();
}





void tali1(void){
GLUquadricObj *pObj;
pObj =gluNewQuadric();
gluQuadricNormals(pObj, GLU_SMOOTH); 
glPushMatrix();
glTranslatef(4.7,70.1,3.8);
glRotatef(93,1,0,0);
glRotatef(-15,0,1,0);
gluCylinder(pObj, 0.05, 0.05, 6, 25, 25);
glPopMatrix();
}

void tali2(void){
GLUquadricObj *pObj;
pObj =gluNewQuadric();
gluQuadricNormals(pObj, GLU_SMOOTH); 
glPushMatrix();
glTranslatef(2,70.1,5.5);
glRotatef(93,1,0,0);
glRotatef(15,0,1,0);
glRotatef(15,1,0,0);
gluCylinder(pObj, 0.05, 0.05, 6, 25, 25);
glPopMatrix();
}

void tali3(void){
GLUquadricObj *pObj;
pObj =gluNewQuadric();
gluQuadricNormals(pObj, GLU_SMOOTH); 
glPushMatrix();
glTranslatef(5,70.1,5.3);
glRotatef(93,1,0,0);
glRotatef(-15,0,1,0);
glRotatef(15,1,0,0);
gluCylinder(pObj, 0.05, 0.05, 6, 25, 25);
glPopMatrix();
}

void tali4(void){
GLUquadricObj *pObj;
pObj =gluNewQuadric();
gluQuadricNormals(pObj, GLU_SMOOTH); 
glPushMatrix();
glTranslatef(2,70.1,3.3);
glRotatef(70,1,0,0);
glRotatef(15,0,1,0);
glRotatef(15,1,0,0);
gluCylinder(pObj, 0.05, 0.05, 6, 25, 25);
glPopMatrix();
}

void tali5(void){
GLUquadricObj *pObj;
pObj =gluNewQuadric();
gluQuadricNormals(pObj, GLU_SMOOTH); 
glPushMatrix();
glTranslatef(5,70.1,5.3);
glRotatef(93,1,0,0);
glRotatef(-15,0,1,0);
glRotatef(15,1,0,0);
gluCylinder(pObj, 0.05, 0.05, 6, 25, 25);
glPopMatrix();
}

void tali6(void){
GLUquadricObj *pObj;
pObj =gluNewQuadric();
gluQuadricNormals(pObj, GLU_SMOOTH); 
glPushMatrix();
glTranslatef(2,70.1,1.3);
glRotatef(83,1,0,0);
glRotatef(15,0,1,0);
glRotatef(-15,1,0,0);
gluCylinder(pObj, 0.05, 0.05, 6, 25, 25);
glPopMatrix();
}

void tali7(void){
GLUquadricObj *pObj;
pObj =gluNewQuadric();
gluQuadricNormals(pObj, GLU_SMOOTH); 
glPushMatrix();
glTranslatef(5,70.1,1.7);
glRotatef(70,1,0,0);
glRotatef(-15,0,1,0);
gluCylinder(pObj, 0.05, 0.05, 6, 25, 25);
glPopMatrix();
}

void tali8(void){
GLUquadricObj *pObj;
pObj =gluNewQuadric();
gluQuadricNormals(pObj, GLU_SMOOTH); 
glPushMatrix();
glTranslatef(2.2,70.1,3.4);
glRotatef(75,1,0,0);
glRotatef(15,0,1,0);
glRotatef(15,1,0,0);
gluCylinder(pObj, 0.05, 0.05, 6, 25, 25);
glPopMatrix();
}



void pohon(void){
//batang
GLUquadricObj *pObj;
pObj =gluNewQuadric();
gluQuadricNormals(pObj, GLU_SMOOTH);    

glPushMatrix();
glColor3ub(104,70,14);
glRotatef(270,1,0,0);
gluCylinder(pObj, 4, 0.7, 30, 25, 25);
glPopMatrix();
}

//ranting  
void ranting(void){
GLUquadricObj *pObj;
pObj =gluNewQuadric();
gluQuadricNormals(pObj, GLU_SMOOTH); 
glPushMatrix();
glColor3ub(104,70,14);
glTranslatef(0,27,0);
glRotatef(330,1,0,0);
gluCylinder(pObj, 0.6, 0.1, 15, 25, 25);
glPopMatrix();

//daun
glPushMatrix();
glColor3ub(18,118,13);
glScaled(5, 5, 5);
glTranslatef(0,7,3);
glutSolidDodecahedron();
glPopMatrix();
}


//api
void tempatapi(void){
GLUquadricObj *pObj;
pObj =gluNewQuadric();
gluQuadricNormals(pObj, GLU_SMOOTH);

glPushMatrix();
glTranslatef(0,66.5,0);
glutSolidCube(2);
glPopMatrix();

glPushMatrix();
glTranslatef(0,68,0);
glutSolidCube(1);
glPopMatrix();

glPushMatrix();
glTranslatef(0,69.5,0);
glRotatef(90,1,0,0);
gluCylinder(pObj, 0.3, 0.3, 1, 25, 25);
glPopMatrix();
}

void api(void){

glPushMatrix();
glColor3ub(241,114,11);
glTranslatef(0,69.5,0);
glutSolidSphere(0.4, 20, 20);
glPopMatrix();

glPushMatrix();
glColor3ub(241,170,11);
glTranslatef(0,69.5,0);
glRotatef(-90,1,0,0);
glutSolidCone(0.4, 1, 20, 20);
glPopMatrix();

}


//manusia
void manusia(void){
glPushMatrix();
glTranslatef(0,62.8,2);  
glScalef(0.4, 0.4, 0.4);

    GLUquadricObj *pObj; 
    glColor3ub(128, 128, 128);

    pObj =gluNewQuadric();
	gluQuadricNormals(pObj, GLU_SMOOTH);

	//kepala
	glColor3ub(205,133,63);
	gluSphere(pObj, 2.5, 26, 13);
	glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);
		glTranslatef(-1.0, -0.20, 2.4);
		gluSphere(pObj, 0.4, 26, 13);//mata kiri
		glTranslatef(2.0, 0.0, 0.0);
		gluSphere(pObj, 0.4, 26, 13);//mata kanan
        glTranslatef(-1.0, -1.0, 0.0);
		gluSphere(pObj, 0.2, 26, 13);//mulut	
	glPopMatrix();
	
	glPushMatrix();
		glColor3ub(205,133,63);
		glTranslatef(-2.2, -0.50, 0.30);
		gluSphere(pObj, 0.6, 26, 13);
		glTranslatef(4.4, 0.0f, 0.0);
		gluSphere(pObj, 0.6, 26, 13);
	glPopMatrix();
	
	glColor3ub(219,219,112);
	glPushMatrix();
		glRotatef(240.0f, 1.0, 0.0, 0.0);
		glDisable(GL_CULL_FACE);
		gluCylinder(pObj, 3.0, 0.0, 6.0, 26, 1);
		gluCylinder(pObj, 3.0, 4.0, 0.5, 26, 1);
		glEnable(GL_CULL_FACE);
	glPopMatrix();

    //tangan
	//tangan kiri
	glColor3ub(205,133,63);
	glPushMatrix();
		glTranslatef(0.0, -3.6, 0.0);
		glRotatef(160.0, 1.0, 0, 1.0);
		gluCylinder(pObj, 0.7, 0.3, 9.0, 26, 13);
	glPopMatrix();
	//tangan kanan
	glColor3ub(205,133,63);
	glPushMatrix();
		glTranslatef(0.0, -3.6, 0.0);
		glRotatef(160.0, 1.0, 0.0, -1.0);
		gluCylinder(pObj, 0.7, 0.3, 9.0, 26, 13);
	glPopMatrix();
	
	
	glPushMatrix();
		glTranslatef(-3.2, -4.40, 0.0);
		gluSphere(pObj,1.10, 26, 13);
		glRotatef(90.f, 1.0,  -1.80, 0.0);
		gluCylinder(pObj, 1.1, 1.4, 1.0, 26, 13); 
	glPopMatrix();
	glPushMatrix();
		glTranslatef(3.2, -4.40, 0.0);
		gluSphere(pObj,1.10, 26, 13);
		glRotatef(90.0, 1.0,  1.80, 0.0);
		gluCylinder(pObj, 1.1, 1.4, 1.0, 26, 13); 
	glPopMatrix();

	glColor3ub(92,51,23);
	glPushMatrix();
		glTranslatef(0.0, -2.0, 0.0);
		glRotatef(90, 1.0, 0.0, 0.0);
		gluCylinder(pObj, 1, 3.0, 10.0, 26, 13);
	glPopMatrix();
	
glPopMatrix();
}














void burung(void) {

	GLfloat xS = 20;
	GLfloat yS = 12;
	GLfloat zS = 50;
	//kepala
	glColor3f(0.88, 0.88, 0.1);

	glPushMatrix();

	glTranslatef(xS, yS -5, zS -1);
	glScalef(3 * 4, 3 * 4, 3.5 * 4);

	glutSolidSphere(0.5, 20, 30);
	glPopMatrix();


	glColor3f(0.8, 0.3, 0.4);

	glPushMatrix();
	glTranslatef(xS, yS -4, zS -1);
	glScalef(3 * 4, 3 * 4, 0.5 * 4);

	glutSolidSphere(0.5, 20, 30);
	glPopMatrix();

	GLfloat x = -48;
	GLfloat y = 8;
	GLfloat z = 23.5;

	glColor3f(0.7, 0.6, 0.4);
	glPushMatrix();
	glRotatef(95, 0, 1, 0);
	glTranslatef(x - 5.4, y + 1, z - 4);
	glScalef(1.5 * 4, 1.5 * 4, 1.5 * 4);

	glutSolidSphere(0.3, 20, 30);

	glPopMatrix();

	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();
	glRotatef(95, 0, 1, 0);
	glTranslatef(x - 5.4, y + 1, z - 2.75);
	glScalef(1.5 * 3, 1.5 * 3, 1.5 * 3);
	glutSolidSphere(0.18, 20, 30);

	glPopMatrix();

	glPushMatrix();
	glColor3f(0.7, 0.6, 0.4);
	glRotatef(83, 0, 1, 0);
	glTranslatef(x + 4.9, y + 1, z + 5.8);
	glScalef(1.5 * 4, 1.5 * 4, 1.5 * 4);
	glutSolidSphere(0.3, 20, 30);

	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glRotatef(83, 0, 1, 0);
	glTranslatef(x + 4.9, y + 1, z + 6.65);
	glScalef(1.5 * 4, 1.5 * 4, 1.5 * 4);
	glutSolidSphere(0.18, 20, 30);

	glPopMatrix();


	//mulut

    
    
	glPushMatrix();
	glTranslatef(xS + 5, yS -7 + 2, zS -1);
	glRotatef(90, -0.5, 1, 0);
	glutSolidCone(1.5, 3, 20, 30);
	glPopMatrix();
	
	
    glPushMatrix();

	glTranslatef(xS + 5, yS -7 + 2, zS -1);
	glRotatef(90, 0.5, 1, 0);
	glutSolidCone(1.5, 3, 20, 30);
	glPopMatrix();
	
	//badan utama
	glPushMatrix();
	
	glColor3f(0.88, 0.88, 0.1);
	glTranslatef(xS - 7, yS - 10, zS - 0.3);
	glScalef(5 * 4, 4 * 3, 4 * 4);

	glutSolidSphere(0.5, 20, 30);
	glPopMatrix();


	int sudutSayap = 90;
	//sayap
	for (int i = 0; i < 3; i++) {
		//kanan
		glPushMatrix();
		glTranslatef(xS - 6, yS - 7 - i, zS + 7);
		glRotatef(sudutSayap, 1, 10, 0);
		glRotatef(-15 + (i * 8), 0, 1, 0);
		glRotatef(-8.5, 0, 0, 1);
		glScalef(4 * 4.5 - i, 0.6 * 5, 2.5 * 3 - 1);

		glutSolidSphere(0.5, 20, 30);
		glPopMatrix();
		//kiri
		glPushMatrix();
		glTranslatef(xS - 6, yS - 7 - i, zS -9 );
		glRotatef(sudutSayap, 1, 10, 0);
		glRotatef(-15 + (i * 8), 0, 1, 0);
		glRotatef(-8.5, 0, 0, 1);
		glScalef(4 * 4.5 - i, 0.6 * 5, 2.5 * 3 - 1);

		glutSolidSphere(0.5, 20, 30);
		glPopMatrix();

	}

	//buntut
	int sudut = 145;
	for (int i = 0; i < 4; i++) {
		glPushMatrix();

		glTranslatef(xS - 12.5 + i, yS - 5 - i, zS);
		glRotatef(sudut, 0, 0, 1);
		glScalef(4 * 6.5, 0.6 * 6, (2.5 * 4) - i);

		glutSolidSphere(0.5, 20, 30);
		glPopMatrix();
		sudut += 10;
	}
	
	//kaki
	     GLUquadricObj *pObj;
         pObj =gluNewQuadric();
         gluQuadricNormals(pObj, GLU_SMOOTH);    
         glColor3ub(92, 51, 23);
         
         glPushMatrix();
		 glTranslatef(xS - 5, yS - 15, zS +2);
         glRotatef(90,1,-1,0);
         gluCylinder(pObj, 0.3, 0.3, 4, 25, 25);
         glPopMatrix();
         
         glPushMatrix();
		 glTranslatef(xS - 5, yS - 15, zS -2);
         glRotatef(90,1,-1,0);
         gluCylinder(pObj, 0.3, 0.3, 4, 25, 25);
         glPopMatrix();

	glPopMatrix();
}
