/*
 * DisplayVisitor.hpp
 *
 *  Created on: 16/ago/2016
 *      Author: daniele
 */

#ifndef MOVEVISITOR_HPP_
#define MOVEVISITOR_HPP_
#include <Menu.hpp>
#include <MenuVisitors.hpp>
#include <stack>

namespace homerio {


class MoveVisitor : public MenuNavigatorVisitor {
	stack<SubMenu*> history;

public:
	MoveVisitor() {	};
	virtual ~MoveVisitor() {};
	void init(SubMenu *m) { history.push(m); }
	MenuComponent* home() {
		while(history.size() > 1) {
			 history.top()->home();
			 history.pop();
		}
		history.top()->home();
		return(history.top()->get_active_element());
	}

	virtual MenuComponent* move(MenuLeaf& m,KeyButton& k) 	{
		 switch(k.get_key()) {
			 case BUTTON_UP:
				 return(history.top()->get_previous_element());
			 case BUTTON_DOWN:
				 return(history.top()->get_next_element());
			 case BUTTON_LEFT:
				 if(history.size() > 1) {
					 history.top()->home();
					 history.pop();
				 }
				 return(history.top()->get_active_element());
			 case BUTTON_RIGHT:
				 return(&m);
			 default:
					throw std::runtime_error((string("Unexpected key ")+ string(k)).c_str());
				 break;
		 }
	}

	virtual MenuComponent* move(SubMenu& m,KeyButton& k) 	{
		 switch(k.get_key()) {
			 case BUTTON_UP:
				 return(history.top()->get_previous_element());
			 case BUTTON_DOWN:
				 return(history.top()->get_next_element());
			 case BUTTON_LEFT:
				 if(history.size() > 1) {
					 history.top()->home();
					 history.pop();
				 }
				 return(history.top()->get_active_element());
			 case BUTTON_RIGHT:
				 history.push(&m);
				 return(history.top()->get_active_element());
			 default:
					throw std::runtime_error((string("Unexpected key ")+ string(k)).c_str());
				 break;
		 }
	}
};

}


#endif /* MOVEVISITOR_HPP_ */
