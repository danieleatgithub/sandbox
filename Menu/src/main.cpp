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
#include <LoggerVisitor.hpp>
#include <HomerMenu.hpp>

using namespace std;
using namespace homerio;
using namespace shd;
using namespace log4cplus;

struct termios t;


int main() {
	string name;

	initialize();
    PropertyConfigurator config("/wks/workspace/sandbox/Menu/src/homerd.properties");
    config.configure();

	Logger logger = Logger::getRoot();
	HomerMenu homerMenu;
	KeyPanel key_panel;
	Scheduler sch;
	KeyEmulator key_emulator;

	key_emulator.start(); // Start Emulator
	sleep(2); // FIXME fare start sincrono

	key_panel.set_event_filename(key_emulator.get_event().c_str());
	sleep(2); // FIXME fare start sincrono
	cout << "READY" << endl;
	key_panel.start();  // Start keyPanel

	homerMenu.start(key_panel,sch); // Start Menu

	sleep(120);
	key_emulator.stop();


	return 0;
}
