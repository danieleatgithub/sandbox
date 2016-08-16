/*
 * DisplayVisitor.hpp
 *
 *  Created on: 16/ago/2016
 *      Author: daniele
 */

#ifndef LOGGERVISITOR_HPP_
#define LOGGERVISITOR_HPP_

#include <MenuComponentVisitor.hpp>
#include <Menu.hpp>

namespace homerio {

class LoggerVisitor : public MenuComponentVisitor {
public:
	LoggerVisitor() {};
	virtual ~LoggerVisitor() {};

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


#endif /* LOGGERVISITOR_HPP_ */
