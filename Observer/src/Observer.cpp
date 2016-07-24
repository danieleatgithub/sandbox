/*
 * Observer.cpp
 *
 *  Created on: 14/feb/2016
 *      Author: daniele
 */


#include "Observer.h"
#include <string>
#include <iostream>

using namespace std;
using namespace obs;

struct HubbleTelescope
{
  void foundNewPlanet(const int type, const string name) const
  {
    cout << "The hubble telescope found a new planet of type "
         << type << " named " << name << endl;
  }
};
struct NewHorizon
{
  void foundNewPlanet(const int type, const string name) const
  {
    cout << "The NewHorizon telescope found a new planet of type "
         << type << " named " << name << endl;
  }
};
int main()
{
  Subject<void (int, string)> space;

  // E.T. is watching space
  auto E_T_ = space.registerObserver( [] (int, string name)
    {
      cout << "E.T. phone " << name << "?" << endl;
    });

  {
    // The hubble telescope is watching only a portion of space
    HubbleTelescope ht;
    NewHorizon hr;

    auto registration = space.registerObserver(
      bind(&HubbleTelescope::foundNewPlanet, cref(ht), std::placeholders::_1, std::placeholders::_2));
    auto registration2 = space.registerObserver(
      bind(&NewHorizon::foundNewPlanet, cref(hr), std::placeholders::_1, std::placeholders::_2));


    space(2, "Mogo");
    space(1, "Mogoeee");
  } // the Hubble stops being interested here.

  space(3, "Oa");
}
