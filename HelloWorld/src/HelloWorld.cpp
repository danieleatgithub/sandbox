//============================================================================
// Name        : HelloWorld.cpp
// Author      : Daniele Colombo
// Version     :
// Copyright   : GPL 2
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <set>
#include <stdio.h>
using namespace std;

int main() {

  int a[5] = { 0, 1, 2, 3, 4 };
  printf("%d\n", a);
  return (0);
  std::set<int> s;
  s.insert(1);
  s.insert(2);
  s.insert(3);

  for (std::set<int>::iterator it = s.begin(); it != s.end(); ++it) {
    if (*it == 2)
      s.erase(it);
  }

  cout << "HelloComment" << endl;  // prints HelloComment

  return 0;
}
