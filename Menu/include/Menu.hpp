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
#include <MenuComponentVisitor.hpp>

using namespace std;
using namespace homerio;
using namespace shd;


namespace homerio {

class MenuComponentVisitor;

/**
 * MenuComponent implements an abstract element in a menu
 *
 */
class MenuComponent {
	protected:
		MenuComponent* parent;


	public:
		string label;
		MenuComponent(string label,MenuComponent* parent) {
			this->label = label;
			this->parent = parent;
		}
		MenuComponent(string label) {
			this->label = label;
			this->parent = nullptr;
		}
		virtual ~MenuComponent() {};
		// Tree movement
		virtual MenuComponent*  prev() { return(this); }
		virtual MenuComponent*  next() { return(this); }
		virtual MenuComponent*  back() { return(this); }

		// Navigation method
		virtual MenuComponent*  move(KeyButton& k) = 0;

		// Visitors interface
		virtual void exe_enter(MenuComponentVisitor& m, KeyButton& k) 	= 0;
		virtual void exe_leave(MenuComponentVisitor& m, KeyButton& k) 	= 0;
		virtual void exe_click(MenuComponentVisitor& m, KeyButton& k) 	= 0;

		/**
		 *
		 * @param p
		 */
		void set_parent(MenuComponent * p) {
			this->parent = p;
		}
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

   /**
    *
    * @return the previous element in submenu
    */
   MenuComponent* prev() {
	   if(cursor != children.begin()) cursor--;
	   return(*cursor);
   }
   /**
    *
    * @return the next element in submenu
    */
  MenuComponent* next() {
	   cursor++;
	   if(cursor == children.end()) {
		   cursor--;
	   }
	   return(*cursor);
   }

  MenuComponent* back() {
	   if(parent == nullptr) {
		   return(*cursor);
	   } else {
		   return(this);
	   }
   }


public:
   SubMenu(string label) : MenuComponent(label) {}
   /**
    *
    * @param c
    * MenuComponent to add to SubMenu
    */
   void add( MenuComponent* c ) {
	   children.push_back( c );
	   c->set_parent(this);
	   cursor = children.begin();
   }
   MenuComponent *home() {
	   cursor = children.begin();
	   return(*cursor);
   }
   /**
    *
    * @return
    * Return the active element in submenu
    */
   MenuComponent *get_active() {
	   return(*cursor);
   }

   virtual MenuComponent* move(KeyButton& k) {
		 switch(k.get_key()) {
			 case BUTTON_UP:
				 return(parent->prev());
			 case BUTTON_DOWN:
				 return(parent->next());
			 case BUTTON_LEFT:
				 return(parent->back());
			 case BUTTON_RIGHT:
				 return(*cursor);
			 default:
					throw std::runtime_error((string("Unexpected key ")+ string(k)).c_str());
				 break;
		 }
   }

   // Visitor Interfaces
   void exe_click(MenuComponentVisitor& m,KeyButton& k) {
	   m.click(*this,k);
   }
   void exe_enter(MenuComponentVisitor& m,KeyButton& k) {
	   m.enter(*this,k);
   }
   void exe_leave(MenuComponentVisitor& m,KeyButton& k) {
	   m.leave(*this,k);
   }

};

/**
 * Terminal passive Menu element
 */
class MenuLeaf : public MenuComponent {

public:
   MenuLeaf(string label)  : MenuComponent(label) {  }


   virtual MenuComponent* move(KeyButton& k) {
			 switch(k.get_key()) {
				 case BUTTON_UP:
					 return(parent->prev());
				 case BUTTON_DOWN:
					 return(parent->next());
				 case BUTTON_LEFT:
					 return(parent->back());
				 case BUTTON_RIGHT:
					 return(this);
				 default:
						throw std::runtime_error((string("Unexpected key ")+ string(k)).c_str());
					 break;
			 }
	   }

    // Visitor Interfaces
   void exe_click(MenuComponentVisitor& m,KeyButton& k) {
	   m.click(*this,k);
   }
   void exe_enter(MenuComponentVisitor& m,KeyButton& k) {
	   m.enter(*this,k);
   }
   void exe_leave(MenuComponentVisitor& m,KeyButton& k) {
	   m.leave(*this,k);
   }

};


}
#endif /* MENU_HPP_ */
