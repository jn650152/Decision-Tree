#ifndef NODE_H
#define NODE_H
#include <string>
#include <vector>
#include <iostream>
#include "child_node.h"
using namespace std;
class node{
  public:
	string name;
	vector<child_node> children;
        void print() {
            cout << "name:" << this->name << "\nlen:" << this->name.length() << endl;
        }
};

#endif
