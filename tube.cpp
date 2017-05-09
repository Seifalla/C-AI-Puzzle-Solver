#include "tube.h"

using namespace std;

bool tube::isFull(){

/* This method checks if the tube is full by comparing the stack's size with the maximum number of
balls the tube can contain. */

	if(cylinder.size() == maxSize)

		return true;
	else
		return false;
}

bool tube::solved(){

/* This method checks if all balls in the tube have the same color. 
It compares the first two balls and pops them off the stack.  
It continues to iterate over the procedure mentioned above as long as
the first two balls are the same. */

	//This variable will be set to true if the first two balls have the same color.
	bool same = true; 

	//A temporary array to store the values that are popped off the stack.
	int tempArray[cylinder.size()];

	//The temporary array index.
	int index = 0;

	//A variable to store the color of the ball on the top of the tube.
	int ball;

	/*Compare the first two balls, if they are the same, pop them off the stack
	 and repeat the procedure.*/

	while(same){

		ball = cylinder.top();

		tempArray[index] = ball;

		index ++;

		cylinder.pop();

		if(ball == cylinder.top())

			same = true;
		else 
			same = false;	
	}

	//Push the popped values back to the stack.
	for(int i = (cylinder.size() - 1); i >= 0; i --)

		cylinder.push(tempArray[i]);

}


