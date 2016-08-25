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

using namespace homeremulator;

namespace homerio {

class WinstarEmulator: public Winstar {
	GLfloat *bg,*fg;
	BoardEmulated& board;

public:

	WinstarEmulator(KeyPanel &kpnl, Scheduler &shd, BoardEmulated& board) :
		Winstar(kpnl,shd,board.getI2c0(), board.getLcdReset(), board.getLcdBacklight()), board(board) {
		bg = grey;
		fg = black;
	}

	virtual ~WinstarEmulator() {};

	GpioPortEmulated& getLcdReset() {
		return(board.getLcdReset());
	}
	GpioPortEmulated& getLcdBacklight() {
		return(board.getLcdBacklight());
	}

	void draw() {
		   glColor3fv(bg);
		   glRects(10.0f,140.0f, 150.0f, 190.0f);
		   glColor3fv(fg);
		   glRasterPos2i(20, 170);
		   glRasterPos2i(20, 150);
	}
	void setColor(GLfloat *background,GLfloat *foreground) {
			   bg = background;
			   fg = foreground;
	}
};


} /* namespace homerio */

#endif /* WINSTAREMULATOR_HPP_ */
