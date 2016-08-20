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

#define TRACE_TMP do { \
	cout << __PRETTY_FUNCTION__ << l.get_label() << "" << endl; \
} while(0)

#define TRACE_TMP2 do { \
	cout << __PRETTY_FUNCTION__ << l.get_label() << ">" << endl; \
} while(0)

namespace homerio {

class DisplayVisitor : public MenuActionVisitor {
public:
	DisplayVisitor() {};
	virtual ~DisplayVisitor() {};

	virtual void enter(MenuLeaf&  l,KeyButton& k) 	{
		TRACE_TMP;
	}
	virtual void leave(MenuLeaf&  l,KeyButton& k) {
		TRACE_TMP;
	}
	virtual void click(MenuLeaf& l,KeyButton& k) {
		TRACE_TMP;
	}


	virtual void enter(SubMenu& l,KeyButton& k) 	{
		TRACE_TMP2;
	}
	virtual void leave(SubMenu& l,KeyButton& k) {
		TRACE_TMP2;
	}
	virtual void click(SubMenu& l,KeyButton& k) {
		TRACE_TMP2;
	}

};

}


#endif /* DISPLAYVISITOR_HPP_ */
