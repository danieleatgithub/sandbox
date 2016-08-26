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
#include <WinstarEmulator.hpp>

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

	BoardEmulated		acquaA5Emulated;
	Scheduler			scheduler;
	KeyPanel			keyPanel;

	// Create Big Brother
	HomerEmulator emu(argc,argv,scheduler,keyPanel,acquaA5Emulated);

	Display& display = emu.getDisplay();

	HomerMenu			menu(keyPanel,scheduler,display);

	emu.start(); 		// Start Homer Emulator
	keyPanel.set_event_filename(emu.getKeyEmulator().getEvent().c_str());
	keyPanel.start();  // Start keyPanel

    display.dpy_open();
    display.set_backlight(true);

    emu.mainLoop();

    display.dpy_close();
    keyPanel.stop();


	sleep(1);

	return 0;
}
