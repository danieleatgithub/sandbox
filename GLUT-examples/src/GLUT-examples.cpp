//============================================================================
// Name        : GLUT-examples.cpp
// Author      : Daniele Colombo
// Version     :
// Copyright   : GPL 2
// Description : Hello World in C++, Ansi-style
//============================================================================

//include header file for glfw library so that we can use OpenGL

#include <GL/freeglut.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

using namespace std;
char bitmap[800][129][3];
//Called when a key is pressed
void
keypress (unsigned char key, int x, int y)
{

  switch (key)
    {
    case 'q': //Escape key
      glutLeaveMainLoop ();
    }
}

void
display ()

{
  GLuint texture;
  FILE* fp = fopen (
      "/wks/workspace/sandbox/GLUT-examples/resource/fpanel800x129x24b.bmp",
      "r");
  fread (bitmap, 1, 54, fp);
  int xy = fread (bitmap, 3 * sizeof(char), 800 * 129, fp);
  if (xy != 800 * 129)
    {
      cerr << "Wrong size " << xy;
      exit (1);
    }
  fclose (fp);
  glGenTextures (1, &texture);
  glBindTexture (GL_TEXTURE_2D, texture);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, 800, 129, 0, GL_RGB, GL_UNSIGNED_BYTE,
		bitmap);

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable (GL_TEXTURE_2D);
  glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTranslatef (0.0, 0.0, 0.0);
  glColor3f (0.0, 0.0, 0.0);

  glBegin (GL_POLYGON);

  GLfloat left = -1.0f;
  GLfloat right = 1.0f;
  GLfloat up = 1.0f;
  GLfloat down = -1.0f;

  glTexCoord2f (0.0f, 0.0f); //Texture co-ordinate origin or lower left corner
  glVertex3f (left, down, 0.0f); // v1 low-sx
  glTexCoord2f (1.0f, 0.0f); //Texture co-ordinate for repeating image ten times form			      //origin to lower right corner
  glVertex3f (right, down, 0.0f); // v2 low-dx
  glTexCoord2f (1.0f, 1.0f); //repeat texture ten times form lower to top right corner.
  glVertex3f (right, up, 0.0f); // v3 up-dx
  glTexCoord2f (0.0f, 1.0f); //repeat texture ten time form top right to top left corner.
  glVertex3f (left, up, 0.0f); // up-sx

  glEnd ();
  glFlush ();
  glDisable (GL_TEXTURE_2D); //Disable the texture

}
void
reshape (int w, int h)
{

}

int
main ()

{

  // int     width, height;
  char *myargv[1];
  int myargc;
  myargv[0] = strdup ("GLUT examples");

  glutInit (&myargc, &myargv[0]);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize (800, 129);
  glutInitWindowPosition (100, 100);
  glutCreateWindow ("GLUT");
  glShadeModel (GL_FLAT);
  glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
  glEnable (GL_DEPTH_TEST);
  glutReshapeFunc (reshape);
  glutSetKeyRepeat (GLUT_KEY_REPEAT_OFF);
  glutKeyboardFunc (keypress);
  glutDisplayFunc (display);
  glutMainLoop ();

  return 0;

}

