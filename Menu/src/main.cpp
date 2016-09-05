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

	// Poperties load and init
	initialize();
    PropertyConfigurator config("/wks/workspace/sandbox/Menu/src/homerd.properties");
    config.configure();

	Logger logger = Logger::getRoot();

	// Real stuff
	Scheduler			*scheduler;
	KeyPanel			*keyPanel;
	HomerMenu			*menu;

	// Emulated stuff
	BoardEmulated   	*acquaA5;
	WinstarEmulator		*display;
	HomerEmulator       *emulator;


	// Real stuff
	keyPanel  = new KeyPanel();
	scheduler = new Scheduler();
	menu 	  = new HomerMenu(*keyPanel,*scheduler);

	// Emulated stuff
	acquaA5   = new BoardEmulated();
	display	  = new WinstarEmulator(*keyPanel,*scheduler,*acquaA5);
	emulator  = new HomerEmulator(display);

	emulator->start();
	keyPanel->set_event_filename(emulator->getKeyEventFilename().c_str());
	keyPanel->start();
    display->dpy_open();
    display->set_backlight(true);

    emulator->mainLoop();

    display->dpy_close();
    keyPanel->stop();
    emulator->stop();

    sleep(1);

    delete(emulator);
    delete(menu);
    delete(display);
    delete(acquaA5);
    delete(scheduler);
    delete(keyPanel);

	return 0;
}
