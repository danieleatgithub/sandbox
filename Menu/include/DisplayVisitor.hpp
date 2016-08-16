/*
 * DisplayVisitor.hpp
 *
 *  Created on: 16/ago/2016
 *      Author: daniele
 */

#ifndef DISPLAYVISITOR_HPP_
#define DISPLAYVISITOR_HPP_

#include <MenuComponentVisitor.hpp>
#include <Menu.hpp>

namespace homerio {

class DisplayVisitor : public MenuComponentVisitor {
public:
	DisplayVisitor() {};
	virtual ~DisplayVisitor() {};

	virtual void enter(MenuLeaf& l,KeyButton& k) 	{
		cout << __PRETTY_FUNCTION__ << l.label << "" << endl;
	}
	virtual void leave(MenuLeaf& l,KeyButton& k) {
		cout << __PRETTY_FUNCTION__ << l.label << "" << endl;
	}
	virtual void click(MenuLeaf& l,KeyButton& k) {
		cout << __PRETTY_FUNCTION__ << l.label << "" << endl;
	}


	virtual void enter(SubMenu& l,KeyButton& k) 	{
		cout << __PRETTY_FUNCTION__ << l.label << ">" << endl;
	}
	virtual void leave(SubMenu& l,KeyButton& k) {
		cout << __PRETTY_FUNCTION__ << l.label << ">" << endl;
	}
	virtual void click(SubMenu& l,KeyButton& k) {
		cout << __PRETTY_FUNCTION__ << l.label << ">" << endl;
	}

};

}


#endif /* DISPLAYVISITOR_HPP_ */
