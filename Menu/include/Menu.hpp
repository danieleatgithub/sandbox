/*
 * Menu.hpp
 *
 * Reference:
 * 		http://stackoverflow.com/questions/13634189/best-way-to-construct-a-console-menu-class-hierachy
 * 		https://sourcemaking.com/design_patterns/composite
 * 		https://sourcemaking.com/design_patterns/visitor
 *
 *  Created on: 18/lug/2016
 *      Author: daniele
 */

#ifndef MENU_HPP_
#define MENU_HPP_


#include <iostream>
#include <vector>
#include <iterator>
#include <stack>
#include <Scheduler.hpp>
#include <KeyPanel.hpp>
#include <MenuVisitors.hpp>

using namespace std;
using namespace homerio;
using namespace shd;


namespace homerio {

class MenuActionVisitor;

/**
 * MenuComponent implements an abstract element in a menu
 *
 */
class MenuComponent {
	protected:
		string label;

	public:
		MenuComponent(string label) {
			this->label = label;
		}
		virtual ~MenuComponent() {};


		string get_label() {
			return label;
		}

		// Visitors interface
		virtual void exe_enter(MenuActionVisitor& m, KeyButton& k) 	= 0;
		virtual void exe_leave(MenuActionVisitor& m, KeyButton& k) 	= 0;
		virtual void exe_click(MenuActionVisitor& m, KeyButton& k) 	= 0;

		virtual MenuComponent* exe_move(MenuNavigatorVisitor& m,KeyButton& k) = 0;
};


/**
 *
 * SubMenu component.
 * container of MenuComponents
 *
 */
class SubMenu : public MenuComponent {
protected:
   vector<MenuComponent*> children;
   vector<MenuComponent*>::iterator cursor;


public:
   SubMenu(string label) : MenuComponent(label) { }
   /**
    *
    * @param c
    * MenuComponent to add to SubMenu
    */
   void add( MenuComponent* c ) {
	   children.push_back( c );
	   cursor = children.begin();
   }
   void home() {
	   cursor = children.begin();
   }
   /**
    *
    * @return
    * Return the active element in submenu
    */
   MenuComponent *get_active_element() {
	   return(*cursor);
   }

   MenuComponent *get_previous_element() {
	   if(cursor != children.begin()) cursor--;
	   return(*cursor);
   }
   MenuComponent *get_next_element() {
		 cursor++;
		 if(cursor == children.end()) cursor--;
	   return(*cursor);
   }

   // Visitor Interfaces
   virtual void exe_click(MenuActionVisitor& m,KeyButton& k) {
 	   m.click(*this,k);
   }
   virtual void exe_enter(MenuActionVisitor& m,KeyButton& k) {
 	   m.enter(*this,k);
   }
   virtual void exe_leave(MenuActionVisitor& m,KeyButton& k) {
 	   m.leave(*this,k);
   }

   virtual MenuComponent* exe_move(MenuNavigatorVisitor& m,KeyButton& k) {
 	   return(m.move(*this,k));
   }

};

/**
 * Terminal passive Menu element
 */
class MenuLeaf : public MenuComponent {


public:
   MenuLeaf(string label)  : MenuComponent(label) {

   }
   // Visitor Interfaces
   virtual void exe_click(MenuActionVisitor& m,KeyButton& k) {
	   m.click(*this,k);
  }
   virtual void exe_enter(MenuActionVisitor& m,KeyButton& k) {
	   m.enter(*this,k);
  }
   virtual void exe_leave(MenuActionVisitor& m,KeyButton& k) {
	   m.leave(*this,k);
  }
   virtual MenuComponent* exe_move(MenuNavigatorVisitor& m,KeyButton& k) {
 	   return(m.move(*this,k));
   }

};


}
#endif /* MENU_HPP_ */
