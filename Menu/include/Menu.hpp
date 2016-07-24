/*
 * Menu.hpp
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

using namespace std;

class MenuComponent {
	protected:
		string label;
		MenuComponent* parent;
	public:
		MenuComponent(string label,MenuComponent* parent) {
			this->label = label;
			this->parent = parent;
		}
		MenuComponent(string label) {
			this->label = label;
			this->parent = nullptr;
		}
		virtual ~MenuComponent() {};
		virtual void traverse() = 0;
		virtual MenuComponent*  up() { return(parent->prev()); }
		virtual MenuComponent*  down() { return(parent->next()); }
		virtual MenuComponent*  left() { return(parent->back()); }
		virtual MenuComponent*  right() { return(this); }
		virtual MenuComponent*  prev() { return(this); }
		virtual MenuComponent*  next() { return(this); }
		virtual MenuComponent*  back() { return(this); }

		virtual void show() = 0;
		void set_parent(MenuComponent * p) {
			this->parent = p;
		}

};



class MenuComposite : public MenuComponent {
   vector<MenuComponent*> children;
   stack<MenuComponent*> tracker;
   vector<MenuComponent*>::iterator cursor;

public:
   MenuComposite(string label) : MenuComponent(label) {

   }
   void add( MenuComponent* c ) {
	   children.push_back( c );

	   c->set_parent(this);
	   cursor = children.begin();
   }
   MenuComponent *get_active() {
	   return(*cursor);
   }
   void traverse() {
      for (unsigned int i=0; i < children.size(); i++)
          children[i]->traverse();
   }
   MenuComponent* prev() {
	   if(cursor != children.begin()) cursor--;
	   return(*cursor);
   }
   MenuComponent* next() {
	   cursor++;
	   if(cursor == children.end()) {
		   cursor--;
	   }
	   return(*cursor);
   }
   MenuComponent* back() {
	   if(tracker.size() > 0) {
		   auto ret = tracker.top();
		   tracker.pop();
		   return(ret);
	   } else {
		   return(this);
	   }
   }
   MenuComponent* right() {
	   tracker.push(this);
	   return(*cursor);
   }

   void show() {
	   cout << __PRETTY_FUNCTION__ << label << ">" << endl;
   }
};

class MenuSysInfo : public MenuComponent {

public:
   MenuSysInfo(string label  ) : MenuComponent(label) {  }
   void traverse()      {
	   cout << "(SYSINFOMENU) " << label << endl;
   }
   void show() {
	   cout << __PRETTY_FUNCTION__ << label<< endl;
   }
};

class MenuLeaf : public MenuComponent {

public:
   MenuLeaf(string label)  : MenuComponent(label) {  }
   void traverse()      {
	   cout << "(LEAF) "  << label <<endl;
   }
   void show() {
	   cout << __PRETTY_FUNCTION__ << label<< endl;
   }

};

class Menu : public MenuComposite {
	public:
	Menu(string label) : MenuComposite(label) { }   // "container".  La maggior parte del
   void traverse() {                       // codice è nella classe base Composite
	   cout << "(MENU) " << label <<endl;
      MenuComposite::traverse();
   }

};


class Row : public MenuComposite {
   public:     // Due diversi tipi di classi
   Row(  ) : MenuComposite(label ) { }   // "container".  La maggior parte del
   void traverse() {                       // codice è nella classe base Composite
      cout << "Row";
      MenuComposite::traverse();
}  };



class Column : public MenuComposite {
   public:
   Column(string label  ) : MenuComposite(label  ) { }
   void traverse() {
      cout << "Col";
      MenuComposite::traverse();
}  };



#endif /* MENU_HPP_ */
