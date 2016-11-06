//============================================================================
// Name        : GLUT-examples.cpp
// Author      : Daniele Colombo
// Version     :
// Copyright   : GPL 2
// Description : Hello World in C++, Ansi-style
//============================================================================

//include header file for glfw library so that we can use OpenGL

#include <GL/glfw.h>
#include <stdlib.h>  //needed for exit function
#include <iostream>

using namespace std;

//Initializes 3D rendering
void
initializeRendering ()

{
  glfwInit ();
  //Makes 3D drawing work when something is in front of something else
  glEnable (GL_DEPTH_TEST);
  //glEnable( GL_TEXTURE_2D );
}

//Called when a key is pressed
void GLFWCALL handleKeypress(int key,int press)//The key that was pressed
  {

    switch (key)
      {
	case GLFW_KEY_ESC: //Escape key
	exit(0);//Exit the program
      }
  }

GLuint
LoadTexture (const char* TextureName)

{

  GLuint Texture;  //variable for texture
  glGenTextures (1, &Texture); //allocate the memory for texture
  glBindTexture (GL_TEXTURE_2D, Texture); //Binding the texture
  if (glfwLoadTexture2D (TextureName, GLFW_BUILD_MIPMAPS_BIT))
    {
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      return Texture;
    }
  else
    return -1;

}

void
display ()

{
  glClearColor (0.0f, 0.0f, 0.0f, 0.0f); //clear background screen to black
  //Clear information from last draw
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode (GL_MODELVIEW); //Switch to the drawing perspective
  glLoadIdentity (); //Reset the drawing perspective
  glTranslatef (0.0f, 0.0f, 0.0f); //Translate whole scene to -ve z-axis by -35 unit
  GLuint text2D;
  text2D = LoadTexture (
      "/wks/workspace/sandbox/GLUT-examples/resource/fpanel24.tga"); //loading image for texture

  glEnable (GL_TEXTURE_2D);
  glBindTexture (GL_TEXTURE_2D, text2D);
  glPushMatrix ();
  glBegin (GL_POLYGON);
  glNormal3f (0.0f, 0.0f, 1.0f);

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
  glPopMatrix ();
  glDisable (GL_TEXTURE_2D); //Disable the texture
  glfwSwapBuffers ();

}

int
main ()

{

  // int     width, height;

  bool running = true;

  initializeRendering ();
  glfwOpenWindowHint (GLFW_WINDOW_NO_RESIZE, GL_TRUE);
  if (!glfwOpenWindow (900, // width of window
      141, //height of window
      0,  //redbits
      0,  //greenbits
      0,  //bluebits
      0,  //alphabits
      0,  //depthbits
      0, //stencilbits
      GLFW_WINDOW) //mode
      )//return false if window is not created

    {

      glfwTerminate (); //terminating glfw window

      return 0;

    }

  glfwSetWindowTitle ("codeincodeblock.blogspot.com - Texture");
  glfwSetKeyCallback (handleKeypress); //callback function to handle keypress
  while (running) // infinite loop to draw object again and again

    {              // because once object is draw then window is terminated

      display ();
      running = glfwGetWindowParam (GLFW_OPENED); //when glfw window is opened then it return true
      //if closed then return false

    }

  return 0;

}

