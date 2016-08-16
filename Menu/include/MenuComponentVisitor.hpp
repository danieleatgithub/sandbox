/*
 * MenuComponentVisitor.hpp
 *
 *  Created on: 16/ago/2016
 *      Author: daniele
 */

#ifndef MENUCOMPONENTVISITOR_HPP_
#define MENUCOMPONENTVISITOR_HPP_

#include <Menu.hpp>
#include <KeyPanel.hpp>



namespace homerio {


class MenuLeaf;
class SubMenu;

class MenuComponentVisitor
{
public:
	virtual ~MenuComponentVisitor() {};
	virtual void enter(MenuLeaf& l,KeyButton& k) 	= 0;
	virtual void leave(MenuLeaf& l,KeyButton& k) = 0;
	virtual void click(MenuLeaf& l,KeyButton& k) = 0;

	virtual void enter(SubMenu& l,KeyButton& k) 	= 0;
	virtual void leave(SubMenu& l,KeyButton& k) = 0;
	virtual void click(SubMenu& l,KeyButton& k) = 0;
};

}

#endif /* MENUCOMPONENTVISITOR_HPP_ */
