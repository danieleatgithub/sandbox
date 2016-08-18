/*
 * HomerMenu.hpp
 *
 *  Created on: 16/ago/2016
 *      Author: daniele
 */

#ifndef HOMERMENU_HPP_
#define HOMERMENU_HPP_
#include <Menu.hpp>
#include <mutex>

namespace homerio {




/**
 * Root Menu
 *
 */
class HomerMenu {
private:
	SubMenu root{SubMenu(string("root"))};
    Task timedHome;
    MenuComponent *active_element;

	DisplayVisitor dw;
	LoggerVisitor lw;
	MoveVisitor mv;

	KeyButton nokey;
	std::mutex mutex;

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
		active_element->exe_leave(dw,k);
		active_element->exe_leave(lw,k);
	}
	void enter(KeyButton& k) {
		active_element->exe_enter(dw,k);
		active_element->exe_enter(lw,k);
	}
	void click(KeyButton& k) {
		active_element->exe_click(dw,k);
		active_element->exe_click(lw,k);

	}
public:
    HomerMenu() {

		root.add(&welcome);
		root.add(&ciao);
		sensors.add(&temperature);
		sensors.add(&pressure);
		system.add(&network);
		system.add(&cpu);
		system.add(&sensors);

		root.add(&system);
		root.add(&dany);

    	active_element = root.get_active_element();

    	root.home();
    	mv.init(&root);

		timedHome.setCallback([&] () {
			cout << "Timed Home" << endl;
			leave(nokey);
			mutex.lock();
			active_element = mv.home();
			mutex.unlock();
			enter(nokey);
		});
	}
	virtual ~HomerMenu() {}
	void start(KeyPanel& key_panel, Scheduler& sch) {
		key_panel.key_attach([&] ( KeyButton& k ) {
				 sch.ScheduleCancel(timedHome);
				 if(k.get_key() == BUTTON_ENTER) {
					 click(k);
				 } else {
					 leave(k);
					 mutex.lock();
					 active_element = active_element->exe_move(mv,k);
					 mutex.unlock();
					 enter(k);
				 }
			 sch.ScheduleAfter(std::chrono::seconds(30),timedHome);
		});
	}

};


} /* namespace homerio */



#endif /* HOMERMENU_HPP_ */
