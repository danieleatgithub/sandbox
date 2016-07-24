//============================================================================
// Name        : Menu.cpp
// Author      : Daniele Colombo
// Version     :
// Copyright   : GPL 2
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <Menu.hpp>
#include <iostream>

#include <termios.h>
#include <unistd.h>
struct termios t;
void stdin_unbuffered(void)
{
	tcgetattr(STDIN_FILENO, &t); //get the current terminal I/O structure
	t.c_lflag &= ~ICANON; //Manipulate the flag bits to do what you want it to do
	tcsetattr(STDIN_FILENO, 0, &t); //Apply the new settings
}


void stdin_buffered(void)
{
	tcgetattr(STDIN_FILENO, &t); //get the current terminal I/O structure
	t.c_lflag |= ICANON; //Manipulate the flag bits to do what you want it to do
	tcsetattr(STDIN_FILENO, 0, &t); //Apply the new settings
}

using namespace std;


class TestMenu {
private:
	string name;
	Menu root{Menu(string("root"))};
	MenuLeaf welcome{MenuLeaf(string("welcome"))};
	MenuLeaf ciao{MenuLeaf(string("ciao"))};
	MenuLeaf dany{MenuLeaf(string("dany"))};

	Menu system{Menu(string("system"))};
	MenuLeaf network{MenuLeaf(string("network"))};
	MenuLeaf cpu{MenuLeaf(string("cpu"))};
	Menu sensors{Menu(string("sensors"))};
	MenuLeaf temperature{MenuLeaf(string("temperature"))};
	MenuLeaf pressure{MenuLeaf(string("pressure"))};

	MenuComponent *active;

public:
	TestMenu(string name) {
		this->name = name;
		root.set_parent(&root);
		root.add(&welcome);
		root.add(&ciao);
		sensors.add(&temperature);
		sensors.add(&pressure);
		system.add(&network);
		system.add(&cpu);
		system.add(&sensors);

		root.add(&system);
		root.add(&dany);

		active = root.get_active();
		active->show();
	}
	void down() {
		active = active->down();
		active->show();
	}
	void up() {
		active = active->up();
		active->show();
	}
	void left() {
		active = active->left();
		active->show();
	}
	void right() {
		active = active->right();
		active->show();
	}
};





int main() {
	char ch;
	TestMenu testMenu(string("testMenu"));

	stdin_unbuffered();
	while(ch != '0') {
		ch = cin.get();
		switch(ch){
						case '8':
							testMenu.up();
							break;
						case '2':
							testMenu.down();
							break;
						case '4':
							testMenu.left();
							break;
						case '6':
							testMenu.right();
							break;
		}

	}
	stdin_buffered();

	return 0;
}
