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
#include <memory>

namespace homerio {




/**
 * Homer Menu
 *
 */
class HomerMenu {
private:

    Task timedHome;

    shared_ptr<MenuComponent> active_element;


	KeyButton nokey;
	std::mutex mutex;

	shared_ptr<SubMenu> root = std::make_shared<SubMenu>(string("root"));
	shared_ptr<MenuLeaf> welcome = std::make_shared<MenuLeaf>(string("welcome"));
	shared_ptr<MenuLeaf> ciao = std::make_shared<MenuLeaf>(string("ciao"));
	shared_ptr<MenuLeaf> dany = std::make_shared<MenuLeaf>(string("dany"));
	shared_ptr<SubMenu> system = std::make_shared<SubMenu>(string("system"));
	shared_ptr<MenuLeaf> network = std::make_shared<MenuLeaf>(string("network"));
	shared_ptr<MenuLeaf> cpu = std::make_shared<MenuLeaf>(string("cpu"));
	shared_ptr<SubMenu> sensors = std::make_shared<SubMenu>(string("sensors"));
	shared_ptr<MenuLeaf> temperature = std::make_shared<MenuLeaf>(string("temperature"));
	shared_ptr<MenuLeaf> pressure = std::make_shared<MenuLeaf>(string("pressure"));

	DisplayVisitor  dw;
	MoveVisitor 	mv;


	void leave(KeyButton& k) {
		active_element->exe_leave(dw,k);
	}
	void enter(KeyButton& k) {
		active_element->exe_enter(dw,k);
	}
	void click(KeyButton& k) {
		active_element->exe_click(dw,k);
	}
public:
    HomerMenu(): mv(root) {

		root->add(welcome);
		root->add(ciao);
		sensors->add(temperature);
		sensors->add(pressure);
		system->add(network);
		system->add(cpu);
		system->add(sensors);
		root->add(system);
		root->add(dany);

		active_element = root->home();

		timedHome.setCallback([&] () {
			leave(nokey);
			mutex.lock();
			active_element = mv.home();
			mutex.unlock();
			enter(nokey);
		});
	}
	virtual ~HomerMenu() {}

	void start(KeyPanel& key_panel, Scheduler& sch)  {
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
