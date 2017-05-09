#ifndef TUBE_H
#define TUBE_H

#include <stack>

using namespace std;

class tube {

	public:
		stack<int> cylinder;
        	int maxSize;
		bool isFull();
		bool solved();
};

#endif



