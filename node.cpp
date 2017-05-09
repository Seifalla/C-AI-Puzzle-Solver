#include "node.h"

using namespace std;

node::node(){

	parent = NULL;
	lastOp = "";
}

void node::pathCost(){

	f = g + h;
}
