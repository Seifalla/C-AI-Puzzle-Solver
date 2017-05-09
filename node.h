#ifndef NODE_H
#define NODE_H

#include "tube.h"
#include <string>

class node {

	public:
		tube left[6];
		tube right[6];
		string lastOp;
		node * parent;
		int g;
		int h;
		int f;
		void pathCost();
		node();
};

#endif
