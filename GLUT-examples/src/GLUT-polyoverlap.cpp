/*
 * GLUT-texture.cpp
 *
 *  Created on: 07/nov/2016
 *      Author: daniele
 */

#include <GL/freeglut.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include "GLUTUtilities.h"

using namespace std;

//Called when a key is pressed
void GLUT_polyoverlap_keypress(unsigned char key, int x, int y) {

	switch (key) {
	case 'q': //Escape key
		glutLeaveMainLoop();
	}
}

void GLUT_polyoverlap_display()

{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTranslatef(0.0, 0.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	GLfloat left;
	GLfloat right;
	GLfloat up;
	GLfloat down;

	glBegin (GL_POLYGON);
	left = -1.0f;
	right = 1.0f;
	up = 1.0f;
	down = -1.0f;
	glTexCoord2f(0.0f, 0.0f); //Texture co-ordinate origin or lower left corner
	glVertex3f(left, down, 0.0f); // v1 low-sx
	glTexCoord2f(1.0f, 0.0f); //Texture co-ordinate for repeating image ten times form			      //origin to lower right corner
	glVertex3f(right, down, 0.0f); // v2 low-dx
	glTexCoord2f(1.0f, 1.0f); //repeat texture ten times form lower to top right corner.
	glVertex3f(right, up, 0.0f); // v3 up-dx
	glTexCoord2f(0.0f, 1.0f); //repeat texture ten time form top right to top left corner.
	glVertex3f(left, up, 0.0f); // up-sx
	glEnd();

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	left = -0.5f;
	right = 0.5f;
	up = 0.5f;
	down = -0.5f;
	glTexCoord2f(0.0f, 0.0f); //Texture co-ordinate origin or lower left corner
	glVertex3f(left, down, 0.0f); // v1 low-sx
	glTexCoord2f(1.0f, 0.0f); //Texture co-ordinate for repeating image ten times form			      //origin to lower right corner
	glVertex3f(right, down, 0.0f); // v2 low-dx
	glTexCoord2f(1.0f, 1.0f); //repeat texture ten times form lower to top right corner.
	glVertex3f(right, up, 0.0f); // v3 up-dx
	glTexCoord2f(0.0f, 1.0f); //repeat texture ten time form top right to top left corner.
	glVertex3f(left, up, 0.0f); // up-sx
	glEnd();

	glFlush();

}
void GLUT_polyoverlap_reshape(int w, int h) {

}

int GLUT_polyoverlap_main()

{

	// int     width, height;
	char *myargv[1];
	int myargc;
	myargv[0] = strdup("GLUT_texture");

	glutInit(&myargc, &myargv[0]);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 129);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("GLUT_polyoverlap");
	glShadeModel (GL_FLAT);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
			GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glEnable (GL_DEPTH_TEST);
	glutReshapeFunc(GLUT_polyoverlap_reshape);
	glutSetKeyRepeat (GLUT_KEY_REPEAT_OFF);
	glutKeyboardFunc(GLUT_polyoverlap_keypress);
	glutDisplayFunc(GLUT_polyoverlap_display);
	glutMainLoop();

	return 0;

}

