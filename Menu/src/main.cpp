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
#include <MoveVisitor.hpp>
#include <HomerMenu.hpp>

using namespace std;
using namespace homerio;
using namespace shd;
using namespace log4cplus;

struct termios t;


int main() {
	string name;

	// Poperties load and init
	initialize();
    PropertyConfigurator config("/wks/workspace/sandbox/Menu/src/homerd.properties");
    config.configure();

	Logger logger = Logger::getRoot();


	Scheduler 	sch;
	HomerMenu 	homerMenu;
	KeyEmulator key_emulator;
	KeyPanel 	key_panel;

	key_emulator.start(); // Start keyboard Emulator
	key_panel.set_event_filename(key_emulator.getEvent().c_str());
	key_panel.start();  // Start keyPanel
	homerMenu.start(key_panel,sch); // Start Menu

	key_emulator.join();


	return 0;
}
