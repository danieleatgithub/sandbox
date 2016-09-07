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
#include <GL/freeglut.h>

using namespace homeremulator;

namespace homerio {

class WinstarEmulator: public Winstar {
	GLfloat *bg,*fg;
	BoardEmulated& 		board;
	Registration		light_reg,i2cwr_reg;
	bool ready;
	char line1[18];
	char line2[18];
	unsigned int cl1,cl2;
	void printString(char *s)
	{
	   glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char *)s);

	}
public:

	WinstarEmulator(KeyPanel &kpnl, Scheduler &shd, BoardEmulated& board) :
		Winstar(kpnl,shd,board.getI2c0(), board.getLcdReset(), board.getLcdBacklight()), board(board) {
		bg = grey;
		fg = black;
		cl1 = 0;
		cl2 = 0;
		ready = false;
		backLight_register();
		i2cbus_register();
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

	void i2cbus_register() {
		board.getI2c0().reg_write(i2cwr_reg, [&] (int fd, const void *buffer, size_t size) {
			const unsigned char *p = (const unsigned char *)buffer;
			// FIXME: change GL init dependency logic
			if(!ready) return;
			if(size == 2 && p[0] == 0 && p[1] == 1) {
				// dpy clear
				sprintf(line1,"                ");
				sprintf(line2,"                ");
				cl1 = 0;
				cl2 = 0;
				this->draw();
			}
			if(size == 2 && p[0] == 0x40) {
				// dpy clear
				line1[cl1++] = p[1];
				this->draw();
			}

			cerr << "SZ=" << size << " p[0]=" << hex << p[0] << endl;
		});
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
	void start() {
		ready = true;
	}
	void stop() {
		ready = false;
	}


};


} /* namespace homerio */

#endif /* WINSTAREMULATOR_HPP_ */
