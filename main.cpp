#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include <windows.h>
#include <math.h>
#include "camera.h"

int windowHeight = 900, windowWidth = 900;
int num_texture = 1;
int id_sun, id_earth, id_jupiter, id_mercury, id_venus, id_mars, id_saturn, id_uranus, id_neptune, id_pluto, id_galaxy;
float ySun=0.0f, yMercury=0.0f, yVenus=0.0f, yEarth = 0.0f, yMars=0.0f, yJupiter = 0.0f, ySaturn=0.0f, yUranus=0.0f, yNeptune=0.0f, yPluto=0.0f;
float mercuryVelocity=0.0f, venusVelocity=0.0f, earthVelocity=0.0f, marsVelocity=0.0f, jupiterVelocity=0.0f, saturnVelocity=0.0f, uranusVelocity=0.0f, neptuneVelocity=0.0f, plutoVelocity=0.0f;
float earthRadius = 0.3959, sunRadius=earthRadius*110;

float rotateX, rotateY, translateX = 0, translateY = 0, translateZ = 0;
float orthoX=400, orthoY=400;

/*const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 0.1f };

const GLfloat mat_ambient[]    = { 0.247725f, 0.1995f, 0.0745f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.75164f, 0.60648f, 0.22648f, 1.0f };
const GLfloat mat_specular[]   = { 0.628281f, 0.555802f, 0.366065f, 1.0f };
const GLfloat high_shininess[] = { 100.f };*/
const GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 0.1f };

Camera camera;

struct ControlStates {
	bool forward, backward, left, right, yawLeft, yawRight, pitchUp, pitchDown, rollLeft, rollRight;
} controls;

int LoadBitmap(char *filename) {
    int i, j = 0;
    FILE *l_file;
    BYTE *l_texture;

    BITMAPFILEHEADER fileheader;
    BITMAPINFOHEADER infoheader;
    RGBTRIPLE rgb;

    num_texture++;

    if((l_file = fopen(filename, "rb")) == NULL)
        return(-1);

    fread(&fileheader, sizeof(fileheader), 1, l_file);

    fseek(l_file, sizeof(fileheader), SEEK_SET);
    fread(&infoheader, sizeof(infoheader), 1, l_file);

    l_texture = (BYTE*)malloc(infoheader.biWidth * infoheader.biHeight * 4);

    memset(l_texture, 0, infoheader.biWidth * infoheader.biHeight * 4);

    for(i=0; i < infoheader.biWidth * infoheader.biHeight; i++) {
        fread(&rgb, sizeof(rgb), 1, l_file);
        l_texture[j+0] = rgb.rgbtRed;
        l_texture[j+1] = rgb.rgbtGreen;
        l_texture[j+2] = rgb.rgbtBlue;
        l_texture[j+3] = 255;
        j += 4;
    }

    fclose(l_file);

    glBindTexture(GL_TEXTURE_2D, num_texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_REPLACE);

    glTexImage2D(GL_TEXTURE_2D, 0, 4, infoheader.biWidth, infoheader.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);

    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, infoheader.biWidth, infoheader.biHeight, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);

    free(l_texture);
    return(num_texture);
}

void SetupRC(){
    /*glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CW);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);*/
    glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();
	GLfloat matSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat matAmbience[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat matShininess[] = { 20.0 };
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbience);

	GLfloat lightAmbient[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat lightDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
}

void timer(int t) {
    yEarth += 3.6f;
    ySun = yEarth/27.0f;
    yMercury = yEarth/58.6;
    yVenus = yEarth/243.0;
    yMars = yEarth/1.03;
    yJupiter = yEarth/0.41f;
    ySaturn = yEarth/0.45f;
    yUranus = yEarth/0.72f;
    yNeptune = yEarth/0.67f;
    yPluto = yEarth/6.39f;

    mercuryVelocity = earthVelocity/0.25;
    venusVelocity = earthVelocity/0.58;
    earthVelocity += 0.0986f;
    marsVelocity = earthVelocity/2;
    jupiterVelocity = earthVelocity/12;
    saturnVelocity = earthVelocity/29.5;
    uranusVelocity = earthVelocity/84;
    neptuneVelocity = earthVelocity/165;
    plutoVelocity = earthVelocity/248;

    glutTimerFunc(10, timer, 0);
    glutPostRedisplay();
}

void drawOrbit(float radius) {
    glDisable(GL_LIGHTING);
    glColor3ub(255, 255, 255);
    glBegin(GL_LINE_STRIP);
        for(float angle = 0.0f; angle < 6.283185307f; angle += 0.05f) {
            glVertex3f(sin(angle) * radius, cos(angle) * radius, 0.0f);
        }
        glVertex3f(0.0f, radius, 0.0f);
    glEnd();
    glEnable(GL_LIGHTING);
}

void drawGalaxy(void) {
    GLUquadricObj* galaxy = gluNewQuadric();
	gluQuadricTexture(galaxy, true);
	gluQuadricNormals(galaxy, GLU_SMOOTH);
	gluSphere(galaxy, 100, 30, 30);
}

static void display(void) {
    if(controls.forward) camera.forward();		if(controls.backward) camera.backward();
	if(controls.left) camera.left();			if(controls.right) camera.right();
	if(controls.yawLeft) camera.yawLeft();		if(controls.yawRight) camera.yawRight();
	if(controls.rollLeft) camera.rollLeft();	if(controls.rollRight) camera.rollRight();
	if(controls.pitchUp) camera.pitchUp();		if(controls.pitchDown) camera.pitchDown();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_COLOR_MATERIAL);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // 3d orchinoo render hiihed zoriulj perspective matrixaa tohiruulah
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0f, (float)windowWidth/(float)windowHeight, 0.001f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    camera.transformOrientation();
    glBindTexture(GL_TEXTURE_2D, id_galaxy);
	drawGalaxy();
    camera.transformTranslation();

    glPushMatrix();
        glRotatef(90, 1, 0, 0);

        GLUquadricObj *sun, *mercury, *venus, *earth, *mars, *jupiter, *saturn, *uranus, *neptune, *pluto;
    /////////////////////////////////////////////////////////////////////////////////////////
        glPushMatrix();
            glRotatef(mercuryVelocity, 0.0f, 1.0f, 0.0f);

            glTranslatef(3.5, 0, 0);
            glRotatef(90, 1, 0, 0);

            glRotatef(yMercury, 0, 0, -1);

            glBindTexture(GL_TEXTURE_2D, id_mercury);
            mercury = gluNewQuadric();
            gluQuadricTexture(mercury, true);
            gluQuadricNormals(mercury, GLU_SMOOTH);
            gluSphere(mercury, earthRadius/3, 30, 30);
        glPopMatrix();
    /////////////////////////////////////////////////////////////////////////////////////////
        glPushMatrix();
            glRotatef(venusVelocity, 0.0f, 1.0f, 0.0f);

            glTranslatef(6.7, 0, 0);
            glRotatef(90, 1, 0, 0);
            glRotatef(yVenus, 0, 0, -1);

            glBindTexture(GL_TEXTURE_2D, id_venus);
            venus = gluNewQuadric();
            gluQuadricTexture(venus, true);
            gluQuadricNormals(venus, GLU_SMOOTH);
            gluSphere(venus, earthRadius*0.9, 30, 30);
        glPopMatrix();
    /////////////////////////////////////////////////////////////////////////////////////////
        glPushMatrix();
            glRotatef(earthVelocity, 0.0f, 1.0f, 0.0f);

            glTranslatef(9.3, 0, 0);
            glRotatef(90, 1, 0, 0);
            glRotatef(yEarth, 0, 0, -1);

            glBindTexture(GL_TEXTURE_2D, id_earth);
            earth = gluNewQuadric();
            gluQuadricTexture(earth, true);
            gluQuadricNormals(earth, GLU_SMOOTH);
            gluSphere(earth, earthRadius, 30, 30);
        glPopMatrix();
    /////////////////////////////////////////////////////////////////////////////////////////
        glPushMatrix();
            glRotatef(marsVelocity, 0.0f, 1.0f, 0.0f);

            glTranslatef(14.2, 0, 0);
            glRotatef(90, 1, 0, 0);

            glRotatef(yMars, 0, 0, -1);

            glBindTexture(GL_TEXTURE_2D, id_mars);
            mars = gluNewQuadric();
            gluQuadricTexture(mars, true);
            gluQuadricNormals(mars, GLU_SMOOTH);
            gluSphere(mars, earthRadius/2, 30, 30);
        glPopMatrix();
    /////////////////////////////////////////////////////////////////////////////////////////
        glPushMatrix();
            glRotatef(jupiterVelocity, 0.0f, 1.0f, 0.0f);

            glTranslatef(48.4, 0, 0);
            glRotatef(90, 1, 0, 0);

            glRotatef(yJupiter, 0, 0, -1);

            glBindTexture(GL_TEXTURE_2D, id_jupiter);
            jupiter = gluNewQuadric();
            gluQuadricTexture(jupiter, true);
            gluQuadricNormals(jupiter, GLU_SMOOTH);
            gluSphere(jupiter, earthRadius*11, 30, 30);
        glPopMatrix();
    /////////////////////////////////////////////////////////////////////////////////////////
        glPushMatrix();
            glRotatef(saturnVelocity, 0.0f, 1.0f, 0.0f);

            glTranslatef(88.9, 0, 0);
            glRotatef(90, 1, 0, 0);

            glRotatef(ySaturn, 0, 0, -1);

            glBindTexture(GL_TEXTURE_2D, id_saturn);
            saturn = gluNewQuadric();
            gluQuadricTexture(saturn, true);
            gluQuadricNormals(saturn, GLU_SMOOTH);
            gluSphere(saturn, earthRadius*9, 30, 30);
        glPopMatrix();
    /////////////////////////////////////////////////////////////////////////////////////////
        glPushMatrix();
            glRotatef(uranusVelocity, 0.0f, 1.0f, 0.0f);

            glTranslatef(179, 0, 0);
            glRotatef(90, 1, 0, 0);

            glRotatef(yUranus, 0, 0, -1);

            glBindTexture(GL_TEXTURE_2D, id_uranus);
            uranus = gluNewQuadric();
            gluQuadricTexture(uranus, true);
            gluQuadricNormals(uranus, GLU_SMOOTH);
            gluSphere(uranus, earthRadius*4, 30, 30);
        glPopMatrix();
    /////////////////////////////////////////////////////////////////////////////////////////
        glPushMatrix();
            glRotatef(neptuneVelocity, 0.0f, 1.0f, 0.0f);

            glTranslatef(288, 0, 0);
            glRotatef(90, 1, 0, 0);

            glRotatef(yNeptune, 0, 0, -1);

            glBindTexture(GL_TEXTURE_2D, id_neptune);
            neptune = gluNewQuadric();
            gluQuadricTexture(neptune, true);
            gluQuadricNormals(neptune, GLU_SMOOTH);
            gluSphere(neptune, earthRadius*4.1, 30, 30);
        glPopMatrix();
    /////////////////////////////////////////////////////////////////////////////////////////
        glPushMatrix();
            glRotatef(plutoVelocity, 0.0f, 1.0f, 0.0f);

            glTranslatef(367, 0, 0);
            glRotatef(90, 1, 0, 0);

            glRotatef(yPluto, 0, 0, -1);

            glBindTexture(GL_TEXTURE_2D, id_pluto);
            pluto = gluNewQuadric();
            gluQuadricTexture(pluto, true);
            gluQuadricNormals(pluto, GLU_SMOOTH);
            gluSphere(pluto, earthRadius/6, 30, 30);
        glPopMatrix();
    /////////////////////////////////////////////////////////////////////////////////////////
        glPushMatrix();
            glTranslatef(0, 0, 0);
            glRotatef(90, 1, 0, 0);

            drawOrbit(3.5);
            drawOrbit(6.7);
            drawOrbit(9.3);
            drawOrbit(14.2);
            drawOrbit(48.4);
            drawOrbit(88.9);
            drawOrbit(179);
            drawOrbit(288);
            drawOrbit(367);

            glRotatef(ySun, 0, 0, -1);

            glBindTexture(GL_TEXTURE_2D, id_sun);

            sun = gluNewQuadric();
            gluQuadricTexture(sun, true);
            gluQuadricNormals(sun, GLU_SMOOTH);

            glDisable(GL_LIGHTING);
            if(sunRadius < 5){
                gluSphere(sun, sunRadius, 30, 30);
            }
            else {
                gluSphere(sun, 2, 30, 30);
            }
            glEnable(GL_LIGHTING);
        glPopMatrix();
    /////////////////////////////////////////////////////////////////////////////////////////
    glPopMatrix();

    /*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)windowWidth, (GLdouble)windowHeight, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();*/

    glutSwapBuffers();
}

void keyDown(unsigned char key, int x, int y) {
	switch (key) {
        case 'w':
            controls.forward = true;
            break;
        case 's':
            controls.backward = true;
            break;
        case 'a':
            controls.left = true;
            break;
        case 'd':
            controls.right = true;
            break;
        case 'l':
            controls.yawRight = true;
            //controls.rollRight = true;
            break;
        case 'j':
            controls.yawLeft = true;
            //controls.rollLeft = true;
            break;
        case 'i':
            controls.pitchDown = true;
            break;
        case 'k':
            controls.pitchUp = true;
            break;
        /*case 'q':
            controls.yawLeft = true;
            break;
        case 'e':
            controls.yawRight = true;
            break;*/
        }
}

void keyUp(unsigned char key, int x, int y) {
	switch (key) {
        case 'w':
            controls.forward = false;
            break;
        case 's':
            controls.backward = false;
            break;
        case 'a':
            controls.left = false;
            break;
        case 'd':
            controls.right = false;
            break;
        case 'l':
            controls.yawRight = false;
            //controls.rollRight = false;
            break;
        case 'j':
            controls.yawLeft = false;
            //controls.rollLeft = false;
            break;
        case 'i':
            controls.pitchDown = false;
            break;
        case 'k':
            controls.pitchUp = false;
            break;
        /*case 'q':
            controls.yawLeft = false;
            break;
        case 'e':
            controls.yawRight = false;
            break;*/
	}
}

void init() {
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glOrtho(-400, 400, -400, 400, -90000, 90000);
    glEnable(GL_TEXTURE_2D);
    id_sun = LoadBitmap("C:\\Users\\User\\OneDrive - National University of Mongolia\\Desktop\\MUIS\\3-r kurs\\1) Namriin uliral\\Computer graphics\\laboratories\\bmpTexture\\sun.bmp");
    id_mercury = LoadBitmap("C:\\Users\\User\\OneDrive - National University of Mongolia\\Desktop\\MUIS\\3-r kurs\\1) Namriin uliral\\Computer graphics\\laboratories\\bmpTexture\\mercury.bmp");
    id_venus = LoadBitmap("C:\\Users\\User\\OneDrive - National University of Mongolia\\Desktop\\MUIS\\3-r kurs\\1) Namriin uliral\\Computer graphics\\laboratories\\bmpTexture\\venus.bmp");
    id_earth = LoadBitmap("C:\\Users\\User\\OneDrive - National University of Mongolia\\Desktop\\MUIS\\3-r kurs\\1) Namriin uliral\\Computer graphics\\laboratories\\bmpTexture\\earth1.bmp");
    id_mars = LoadBitmap("C:\\Users\\User\\OneDrive - National University of Mongolia\\Desktop\\MUIS\\3-r kurs\\1) Namriin uliral\\Computer graphics\\laboratories\\bmpTexture\\mars.bmp");
    id_jupiter = LoadBitmap("C:\\Users\\User\\OneDrive - National University of Mongolia\\Desktop\\MUIS\\3-r kurs\\1) Namriin uliral\\Computer graphics\\laboratories\\bmpTexture\\jupiter.bmp");
    id_saturn = LoadBitmap("C:\\Users\\User\\OneDrive - National University of Mongolia\\Desktop\\MUIS\\3-r kurs\\1) Namriin uliral\\Computer graphics\\laboratories\\bmpTexture\\saturn.bmp");
    id_uranus = LoadBitmap("C:\\Users\\User\\OneDrive - National University of Mongolia\\Desktop\\MUIS\\3-r kurs\\1) Namriin uliral\\Computer graphics\\laboratories\\bmpTexture\\uranus.bmp");
    id_neptune = LoadBitmap("C:\\Users\\User\\OneDrive - National University of Mongolia\\Desktop\\MUIS\\3-r kurs\\1) Namriin uliral\\Computer graphics\\laboratories\\bmpTexture\\neptune.bmp");
    id_pluto = LoadBitmap("C:\\Users\\User\\OneDrive - National University of Mongolia\\Desktop\\MUIS\\3-r kurs\\1) Namriin uliral\\Computer graphics\\laboratories\\bmpTexture\\pluto.bmp");
    id_galaxy = LoadBitmap("C:\\Users\\User\\OneDrive - National University of Mongolia\\Desktop\\MUIS\\3-r kurs\\1) Namriin uliral\\Computer graphics\\laboratories\\bmpTexture\\galaxy.bmp");

    controls.forward = false;
	controls.backward = false;
	controls.left = false;
	controls.right = false;
	controls.rollRight = false;
	controls.rollLeft = false;
	controls.pitchDown = false;
	controls.pitchUp = false;
	controls.yawLeft = false;
	controls.yawRight = false;
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("BMP Texture Solar System");

    init();
    SetupRC();

    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    //glutKeyboardFunc(keyboard);

    glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);

    glutMainLoop();

    return EXIT_SUCCESS;
}
