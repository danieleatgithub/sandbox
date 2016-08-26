/*
 * WinstarEmulation.hpp
 *
 *  Created on: 24/ago/2016
 *      Author: daniele
 */

#ifndef WINSTAREMULATOR_HPP_
#define WINSTAREMULATOR_HPP_

#include <Winstar.h>
#include <EmuGlobals.h>
#include <Observer.h>


using namespace homeremulator;

namespace homerio {

class WinstarEmulator: public Winstar {
	GLfloat *bg,*fg;
	BoardEmulated& 		board;
	Registration		light_reg;
	char line1[18];
	char line2[18];

	void printString(char *s)
	{
	   glPushAttrib (GL_LIST_BIT);
	   glListBase(fontOffset);
	   glCallLists((GLsizei) strlen(s), GL_UNSIGNED_BYTE, (GLubyte *) s);
	   glPopAttrib ();
	}
public:

	WinstarEmulator(KeyPanel &kpnl, Scheduler &shd, BoardEmulated& board) :
		Winstar(kpnl,shd,board.getI2c0(), board.getLcdReset(), board.getLcdBacklight()), board(board) {
		bg = grey;
		fg = black;
		backLight_register();
//		sprintf(line1,"ABCDEFGHILMNOPQR");
//		sprintf(line2,"1234567890123456");
		sprintf(line1,"Homer Emulator");
		sprintf(line2,"1234567890123456");
	}

	virtual ~WinstarEmulator() {};

	GpioPortEmulated& getLcdReset() {
		return(board.getLcdReset());
	}
	GpioPortEmulated& getLcdBacklight() {
		return(board.getLcdBacklight());
	}

	void backLight_register() {
		board.getLcdBacklight().reg_write(light_reg, [&] ( int fd, const void *buffer, size_t size ) {
		   	   const unsigned char *p = (const unsigned char *)buffer;
		   	   if(p[0] == 0x30) {
		   		   cerr << "light off" << endl;
		   		   this->setColor(grey,black);
		   	   } else {
		   		   cerr << "light on" << endl;
		   		this->setColor(white,black);
		   	   }
		   	   glutPostRedisplay();

		});
	}

	void draw() {
		   glColor3fv(bg);
		   glRects(10.0f,140.0f, 190.0f, 190.0f);
		   glColor3fv(fg);
		   glRasterPos2i(20, 170);
		   printString(line1);
		   glRasterPos2i(20, 150);
		   printString(line2);
	}

	void setColor(GLfloat *background,GLfloat *foreground) {
			   bg = background;
			   fg = foreground;
	}



};


} /* namespace homerio */

#endif /* WINSTAREMULATOR_HPP_ */
