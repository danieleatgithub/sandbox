//============================================================================
// Name        : Menu.cpp
// Author      : Daniele Colombo
// Version     :
// Copyright   : GPL 2
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <Menu.hpp>
#include <iostream>
#include <KeyEmulator.hpp>
#include <KeyPanel.hpp>
#include <termios.h>
#include <unistd.h>
#include "Scheduler.hpp"
#include <log4cplus/logger.h>
#include <log4cplus/config.hxx>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include <log4cplus/tchar.h>
#include <DisplayVisitor.hpp>
#include <HomerEmulator.hpp>
#include <MoveVisitor.hpp>
#include <HomerMenu.hpp>
#include <HwLayer.hpp>
#include <HwEmulated.hpp>
#include <Winstar.h>

using namespace std;
using namespace homerio;
using namespace shd;
using namespace log4cplus;
using namespace homeremulator;

struct termios t;


int main(int argc, char** argv) {
	string name;

	// Poperties load and init
	initialize();
    PropertyConfigurator config("/wks/workspace/sandbox/Menu/src/homerd.properties");
    config.configure();

	Logger logger = Logger::getRoot();
	GpioPortEmulated    reset(LCD_RESET_PIN);
	GpioPortEmulated    light(LCD_BACKLIGHT_PIN);
	I2cBusEmulated 		i2c_0(I2C_BUS);

	HomerEmulator emu(argc,argv,i2c_0,reset,light);

	Scheduler 	sch;
	HomerMenu 	homerMenu;
	KeyPanel 	key_panel;
//	Winstar		display(i2c_0,reset,light);


	emu.start(); // Start Homer Emulator
	key_panel.set_event_filename(emu.getKeyEmulator().getEvent().c_str());
	key_panel.start();  // Start keyPanel
	homerMenu.start(key_panel,sch); // Start Menu

//    display.dpy_open();
//    display.key_attach(key_panel,sch);
//    display.set_backlight(true);


	emu.mainLoop();

	sleep(1);

	return 0;
}
