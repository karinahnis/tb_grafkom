#include <GL/glut.h>
#include <math.h>


//untuk di display
void drawSceneTanah(Terrain *terrain, GLfloat r, GLfloat g, GLfloat b) {
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






void display(void){
//    glutSwapBuffers();
	glClearStencil(0); //clear the stencil buffer
	glClearDepth(1.0f);

	glClearColor(0.0, 0.6, 0.8, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();
	gluLookAt(viewx, viewy, viewz, 0.0, 50.0, 0.0, 0.0, 1.0, 0.0);
//gluLookAt(0.0,10.0,3.0,0.0,0.0,0.0,0.0,1.0,0.0);


    glPushMatrix();
    drawScene();
    glPopMatrix();
    
    glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	drawSceneTanah(_terrain, 0.3f, 0.9f, 0.0f);
	glPopMatrix();

	
	glPushMatrix();
	drawSceneTanah(_terrainAir, 0.0f, 0.2f, 0.5f);
	glPopMatrix();


	glPushMatrix();
	drawSceneTanah(_terrainTanah, 0.7f, 0.2f, 0.1f);
	glPopMatrix();





//glEnable(GL_COLOR_MATERIAL);
//glColorMaterial(GL_FRONT,GL_SPECULAR);

//matahari
matahari();

//awan
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



//pohon1
glPushMatrix();
glTranslatef(80,-13,5);    
glRotatef(30,0,1,0);

pohon();
//ranting1
ranting();

//ranting2
glPushMatrix();
glScalef(1.5, 1.5, 1.5);
glTranslatef(0,25,25);   
glRotatef(250,1,0,0);
ranting();
glPopMatrix();

//ranting3
glPushMatrix();
glScalef(1.8, 1.8, 1.8);
glTranslatef(0,-6,21.5);   
glRotatef(-55,1,0,0);
ranting();
glPopMatrix();

glPopMatrix();



//pohon2
glPushMatrix();
glTranslatef(-180,10,-150);    
glScalef(1.5, 1.5, 1.5);
glRotatef(90,0,1,0);
pohon();

//ranting1
ranting();

//ranting2
glPushMatrix();
glScalef(1.5, 1.5, 1.5);
glTranslatef(0,25,25);   
glRotatef(250,1,0,0);
ranting();
glPopMatrix();

//ranting3
glPushMatrix();
glScalef(1.8, 1.8, 1.8);
glTranslatef(0,-6,21.5);   
glRotatef(-55,1,0,0);
ranting();
glPopMatrix();

glPopMatrix();


//pohon3
glPushMatrix();
glTranslatef(-100,-13,100);   
glScalef(0.7, 0.7, 0.7); 
glRotatef(120,0,1,0);

pohon();
//ranting1
ranting();

//ranting2
glPushMatrix();
glScalef(1.5, 1.5, 1.5);
glTranslatef(0,25,25);   
glRotatef(250,1,0,0);
ranting();
glPopMatrix();

//ranting3
glPushMatrix();
glScalef(1.8, 1.8, 1.8);
glTranslatef(0,-6,21.5);   
glRotatef(-55,1,0,0);
ranting();
glPopMatrix();

glPopMatrix();




//pohon4
glPushMatrix();
glTranslatef(150,-13,-100);   
glScalef(0.8, 1, 0.8); 
glRotatef(60,0,1,0);

pohon();
//ranting1
ranting();

//ranting2
glPushMatrix();
glScalef(1.5, 1.5, 1.5);
glTranslatef(0,25,25);   
glRotatef(250,1,0,0);
ranting();
glPopMatrix();

//ranting3
glPushMatrix();
glScalef(1.8, 1.8, 1.8);
glTranslatef(0,-6,21.5);   
glRotatef(-55,1,0,0);
ranting();
glPopMatrix();

glPopMatrix();



//pohon5
glPushMatrix();
glTranslatef(160,-13,100);   
glScalef(1.2, 1.2, 1.0); 
glRotatef(100,0,1,0);

pohon();
//ranting1
ranting();

//ranting2
glPushMatrix();
glScalef(1.5, 1.5, 1.5);
glTranslatef(0,25,25);   
glRotatef(250,1,0,0);
ranting();
glPopMatrix();

//ranting3
glPushMatrix();
glScalef(1.8, 1.8, 1.8);
glTranslatef(0,-6,21.5);   
glRotatef(-55,1,0,0);
ranting();
glPopMatrix();

glPopMatrix();




//burung1

glPushMatrix();
glScalef(0.2, 0.2, 0.2);
glTranslatef(0,200,-80); 
static float i = 0;
i+=0.5;
glRotatef(i,0,1,0);
glRotatef(i,0,1,1);
glTranslatef(0,0,i); 
burung();
glPopMatrix();

//burung2

glPushMatrix();
glRotatef(-180,0,1,0);
glScalef(0.2, 0.2, 0.2);
glTranslatef(0,200,-80);
glRotatef(i,0,1,0);
glRotatef(i,0,1,1);
glTranslatef(0,0,i); 
burung();
glPopMatrix();

//burung3

glPushMatrix();
glScalef(0.2, 0.2, 0.2);
glTranslatef(500,200,-300); 

glRotatef(i,0,1,0);
glRotatef(i,0,1,1);
glTranslatef(0,0,i); 
burung();
glPopMatrix();

//burung4

glPushMatrix();
glRotatef(-180,0,1,0);
glScalef(0.2, 0.2, 0.2);
glTranslatef(500,200,300);
glRotatef(i,0,1,0);
glRotatef(i,0,1,1);
glTranslatef(0,0,i); 
burung();
glPopMatrix();



glPushMatrix();
glScalef(2, 2, 2);
glTranslatef(0,-30,0); 

//manusia
manusia();


//api

api();

//Balon

glBindTexture(GL_TEXTURE_2D, texture[1]);
balonatas();
balonbawah();


//Kotak dibawah balon
glBindTexture(GL_TEXTURE_2D, texture[2]);
tempatapi();

kotakatas();
kotak1();
kotak2();
kotak3();
kotak4();
kotak5();

//tiang
tiang1();
tiang2();
tiang3();
tiang4();
tiang5();
tiang6();
tiang7();
tiang8();

tali1();
tali2();

glPushMatrix();
glTranslatef(-7,0,0);
tali3();
tali4();
glPopMatrix();

glPushMatrix();
glTranslatef(0,0,-7);
tali5();
tali6();
glPopMatrix();

glPushMatrix();
glTranslatef(-7,0,-7);
tali7();
tali8();
glPopMatrix();

glPopMatrix();



    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); //disable the color mask
	glDepthMask(GL_FALSE); //disable the depth mask

	glEnable(GL_STENCIL_TEST); //enable the stencil testing

	glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE); //set the stencil buffer to replace our next lot of data

	//ground
	//tanah(); //set the data plane to be replaced
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); //enable the color mask
	glDepthMask(GL_TRUE); //enable the depth mask

	glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //set the stencil buffer to keep our next lot of data

	glDisable(GL_DEPTH_TEST); //disable depth testing of the reflection

	// glPopMatrix();  
	glEnable(GL_DEPTH_TEST); //enable the depth testing
	glDisable(GL_STENCIL_TEST); //disable the stencil testing
	//end of ground
	glEnable(GL_BLEND); //enable alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //set the blending function
	glRotated(1, 0, 0, 0);
	
	glDisable(GL_BLEND);



    glutSwapBuffers();
	glFlush();
	rot++;
	angle++;




//glDisable(GL_COLOR_MATERIAL);
}




void reshape(int w, int h){
glViewport(0, 0 , (GLsizei) w,(GLsizei)h);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();

//glFrustum(-1.0,1.0,-1.0,1.0,1.5,20.0);
gluPerspective(60, (GLfloat) w / (GLfloat) h, 0.1, 1000.0);
glMatrixMode(GL_MODELVIEW);
//glLoadIdentity();
//gluLookAt(100.0,0.0,5.0,0.0,0.0,0.0,0.0,1.0,0.0);
}








static void kibor(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_HOME:
		viewy++;
		break;
	case GLUT_KEY_END:
		viewy--;
		break;
	case GLUT_KEY_UP:
		viewz--;
		break;
	case GLUT_KEY_DOWN:
		viewz++;
		break;

	case GLUT_KEY_RIGHT:
		viewx++;
		break;
	case GLUT_KEY_LEFT:
		viewx--;
		break;

	case GLUT_KEY_F1: {
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	}
		;
		break;
	case GLUT_KEY_F2: {
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient2);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse2);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	}
		;
		break;
	default:
		break;
	}
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 'd') {

		spin = spin - 1;
		if (spin > 360.0)
			spin = spin - 360.0;
	}
	if (key == 'a') {
		spin = spin + 1;
		if (spin > 360.0)
			spin = spin - 360.0;
	}
	if (key == 'q') {
		viewz++;
	}
	if (key == 'e') {
		viewz--;
	}
	if (key == 's') {
		viewy--;
	}
	if (key == 'w') {
		viewy++;
	}
}

int main(int argc, char** argv){
glutInit(&argc, argv);
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL | GLUT_DEPTH); //add a stencil buffer to the window
glutInitWindowSize(800,600);
glutInitWindowPosition(100,100);
glutCreateWindow("Balon Udara Kelompok 9");
init();

glutDisplayFunc(display);
glutIdleFunc(display);
glutReshapeFunc(reshape);

glutKeyboardFunc (keyboard);
glutSpecialFunc(kibor);

	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);

glutMainLoop();
return 0;
}
