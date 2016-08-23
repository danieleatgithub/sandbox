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

using namespace homerio;

namespace homeremulator {
class HomerEmulator;

struct GL_callbacks {
	static void  reshape(int w, int h);
	static void  keypress(unsigned char key, int x, int y);
	static void  keyrelease(unsigned char key, int x, int y);
	static void  display();

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
	KeyEmulator key_emulator;
	I2cBusEmulated&    i2cbus;
	GpioPortEmulated&  port_reset;
	GpioPortEmulated&  port_light;


	void makeRasterFont(void);
public:
	HomerEmulator(int gl_argc, char** gl_argv,I2cBusEmulated& i2c_0,GpioPortEmulated& reset,GpioPortEmulated& light);
	virtual ~HomerEmulator() {};
	int start();

	KeyEmulator& getKeyEmulator() {
		return key_emulator;
	}
	void mainLoop();

};

}


#endif /* HOMEREMULATOR_HPP_ */
