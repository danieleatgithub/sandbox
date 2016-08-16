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

	MenuLeaf welcome{MenuLeaf(string("welcome"))};
	MenuLeaf ciao{MenuLeaf(string("ciao"))};
	MenuLeaf dany{MenuLeaf(string("dany"))};

	SubMenu system{SubMenu(string("system"))};
	MenuLeaf network{MenuLeaf(string("network"))};
	MenuLeaf cpu{MenuLeaf(string("cpu"))};
	SubMenu sensors{SubMenu(string("sensors"))};
	MenuLeaf temperature{MenuLeaf(string("temperature"))};
	MenuLeaf pressure{MenuLeaf(string("pressure"))};

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
			active = root.home();
		});
	}
	virtual ~HomerMenu() {}
	void start(KeyPanel& key_panel, Scheduler& sch) {
		active = root.home();
		key_panel.key_attach([&] ( KeyButton& k ) {
				 sch.ScheduleCancel(timedHome);
				 if(k.get_key() == BUTTON_ENTER) {
					 active->exe_click(dw,k);
					 active->exe_click(lw,k);
				 } else {
					 active->exe_leave(dw,k);
					 active->exe_leave(lw,k);
					 active = active->move(k);
					 active->exe_enter(dw,k);
					 active->exe_enter(lw,k);
				 }
			 sch.ScheduleAfter(std::chrono::seconds(30),timedHome);
		});
	}

};


} /* namespace homerio */



#endif /* HOMERMENU_HPP_ */
