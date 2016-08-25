/*
 * emulator.hpp
 *
 *  Created on: 22/ago/2016
 *      Author: daniele
 */

#ifndef HOMEREMULATOR_HPP_
#define HOMEREMULATOR_HPP_
#include <GL/glut.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <KeyEmulator.hpp>
#include <HwEmulated.hpp>
#include <Scheduler.hpp>
#include <WinstarEmulator.hpp>

#define HEMUL_REFRESHRATE 250 // milliseconds

using namespace homerio;

namespace homeremulator {
class HomerEmulator;

struct GL_callbacks {
	static void  reshape(int w, int h);
	static void  keypress(unsigned char key, int x, int y);
	static void  keyrelease(unsigned char key, int x, int y);
	static void  display();
	static void  timer(int value);
	static void  idle();

	GL_callbacks () {}
	static void setHomerEmulator(HomerEmulator* ptr) {
		homerEmulator = ptr;
	}
	static HomerEmulator *homerEmulator;
};


class HomerEmulator {
private:
	int gl_argc;
	char **gl_argv;

	// Emulated stuff
	Scheduler&			scheduler;
	KeyPanel&			keyPanel;
	BoardEmulated&   	acquaA5;

	KeyEmulator 		key_emulator;
	WinstarEmulator		display;

	unsigned int		refreshRate;
	void makeRasterFont(void);

public:

	HomerEmulator(int gl_argc, char** gl_argv,Scheduler& shd,KeyPanel& kp, BoardEmulated& board);
	virtual ~HomerEmulator() {
		refreshRate = HEMUL_REFRESHRATE;
	};
	int start();

	KeyEmulator& getKeyEmulator() {
		return key_emulator;
	}

	 WinstarEmulator& getDisplay()  {
		return display;
	}


	void mainLoop();

	unsigned int getRefreshRate() const {
		return refreshRate;
	}

	void setRefreshRate(unsigned int refreshRate) {
		this->refreshRate = refreshRate;
	}
};

}


#endif /* HOMEREMULATOR_HPP_ */
