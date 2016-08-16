/*
 * HomerMenu.hpp
 *
 *  Created on: 16/ago/2016
 *      Author: daniele
 */

#ifndef HOMERMENU_HPP_
#define HOMERMENU_HPP_
#include <Menu.hpp>

namespace homerio {




/**
 * Root Menu
 *
 */
class HomerMenu {
private:
	SubMenu root{SubMenu(string("root"))};
    Task timedHome;
    MenuComponent* active;
	DisplayVisitor dw;
	LoggerVisitor lw;
	KeyButton nokey;

	MenuLeaf welcome{MenuLeaf(string("welcome"))};
	MenuLeaf ciao{MenuLeaf(string("ciao"))};
	MenuLeaf dany{MenuLeaf(string("dany"))};

	SubMenu system{SubMenu(string("system"))};
	MenuLeaf network{MenuLeaf(string("network"))};
	MenuLeaf cpu{MenuLeaf(string("cpu"))};
	SubMenu sensors{SubMenu(string("sensors"))};
	MenuLeaf temperature{MenuLeaf(string("temperature"))};
	MenuLeaf pressure{MenuLeaf(string("pressure"))};
	void leave(KeyButton& k) {
		 active->exe_leave(dw,k);
		 active->exe_leave(lw,k);
	}
	void enter(KeyButton& k) {
		 active->exe_enter(dw,k);
		 active->exe_enter(lw,k);
	}
	void click(KeyButton& k) {
		active->exe_click(dw,k);
		active->exe_click(lw,k);

	}
public:
    HomerMenu() {
		active = nullptr;

		root.add(&welcome);
		root.add(&ciao);
		sensors.add(&temperature);
		sensors.add(&pressure);
		system.add(&network);
		system.add(&cpu);
		system.add(&sensors);

		root.add(&system);
		root.add(&dany);
		timedHome.setCallback([&] () {
			cout << "Timed Home" << endl;
			leave(nokey);
			active = root.home();
			enter(nokey);
		});
	}
	virtual ~HomerMenu() {}
	void start(KeyPanel& key_panel, Scheduler& sch) {
		active = root.home();
		key_panel.key_attach([&] ( KeyButton& k ) {
				 sch.ScheduleCancel(timedHome);
				 if(k.get_key() == BUTTON_ENTER) {
					 click(k);
				 } else {
					 leave(k);
					 active = active->move(k);
					 enter(k);
				 }
			 sch.ScheduleAfter(std::chrono::seconds(30),timedHome);
		});
	}

};


} /* namespace homerio */



#endif /* HOMERMENU_HPP_ */
