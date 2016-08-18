/*
 * DisplayVisitor.hpp
 *
 *  Created on: 16/ago/2016
 *      Author: daniele
 */

#ifndef DISPLAYVISITOR_HPP_
#define DISPLAYVISITOR_HPP_

#include <Menu.hpp>
#include <MenuVisitors.hpp>

namespace homerio {

class DisplayVisitor : public MenuActionVisitor {
public:
	DisplayVisitor() {};
	virtual ~DisplayVisitor() {};

	virtual void enter(MenuLeaf& l,KeyButton& k) 	{
		cout << __PRETTY_FUNCTION__ << l.get_label() << "" << endl;
	}
	virtual void leave(MenuLeaf& l,KeyButton& k) {
		cout << __PRETTY_FUNCTION__ <<l.get_label() << "" << endl;
	}
	virtual void click(MenuLeaf& l,KeyButton& k) {
		cout << __PRETTY_FUNCTION__ << l.get_label() << "" << endl;
	}


	virtual void enter(SubMenu& l,KeyButton& k) 	{
		cout << __PRETTY_FUNCTION__ << l.get_label() << ">" << endl;
	}
	virtual void leave(SubMenu& l,KeyButton& k) {
		cout << __PRETTY_FUNCTION__ << l.get_label() << ">" << endl;
	}
	virtual void click(SubMenu& l,KeyButton& k) {
		cout << __PRETTY_FUNCTION__ << l.get_label() << ">" << endl;
	}

};

}


#endif /* DISPLAYVISITOR_HPP_ */
