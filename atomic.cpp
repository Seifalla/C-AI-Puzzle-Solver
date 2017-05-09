#include "tube.h"
#include "node.h"
#include "node.cpp"

#include <functional>
#include <queue>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

struct CompareNode{

	bool operator()(const node* lhs, const node* rhs) const
	{
		return lhs -> f > rhs -> f;
	}
};

// functions prototypes

void initializePuzzle(tube [], tube []);

void display(tube [], tube []);

void printLine(tube);

void flip(tube [], tube []);

void rotateClockwise(tube []);

void rotateCounterClockwise(tube []);

int shuffle(tube [], tube [], int);

int heuristicFunction(tube [], tube []);

void doMove(node *, int, int&);

bool a_star(tube [], tube [], int);

void traceSolution(node *);

int main(){

	cout<<endl;

	int moves = 0;

// Set up the lengths and add balls to each tube.

	tube leftTubes[6];
	tube rightTubes[6];

// Set the puzzle to its initial state.

	initializePuzzle(leftTubes, rightTubes);

// Display the puzzle.

	display(leftTubes, rightTubes);

	cout<<endl;

	cout<<"h = "<<heuristicFunction(leftTubes, rightTubes)<<endl;

	cout<<"Enter the number of moves needed to randomize the puzzle: ";

	cin>>moves;

	for(int i = 0; i < 5; i ++){

		cout<<"New Puzzle"<<endl;

		tube leftTubes[6];
        	tube rightTubes[6];

		initializePuzzle(leftTubes, rightTubes);

		display(leftTubes, rightTubes);

		cout<<endl;

		int orientation = shuffle(leftTubes, rightTubes, moves);

       		display(leftTubes, rightTubes);

		cout<<endl;
 
        	cout<<"h = "<<heuristicFunction(leftTubes, rightTubes);
	
		cout<<endl;

		a_star(leftTubes, rightTubes, orientation);
		
		cout<<endl;
	}
	return 0;
}


void initializePuzzle(tube left[], tube right[]){

// Set the maximum number of balls each tube can hold.

	for(int i = 5; i >= 0; i --){

                (left[i]).maxSize = i + 1;

                (right[i]).maxSize = i + 1;
        }

// Push the balls into their appropriate tube until it's full.

	for(int j = 0; j < 6; j ++){
       
	 	for(int i = 0; i < ((left[j]).maxSize); i++)

                	(left[j]).cylinder.push((left[j]).maxSize);

	}
}


void display(tube left[], tube right[]){

// Variable to control the execution of the inner loops.

	int loopController;

// Adjust the alignment of each tube.

	for(int i = 5; i >= 0; i --){

		if(left[i].cylinder.size() == left[i].maxSize)

			loopController = left[i].cylinder.size();
		else
			loopController = left[i].maxSize;	

			for(int j = 0; j < (6 - loopController); j ++)

				cout<<' ';
//Display the left tube.

		printLine(left[i]);

		cout<<' ';

//Display the right tube.
		printLine(right[i]);
		
		cout<<endl;	
	}

}

void printLine(tube temp){

// Display the content of each tube.

	for (stack<int> dump = temp.cylinder; !dump.empty(); dump.pop())

                        cout << dump.top();

// Use 0's to represent unoccupied space.

        	for(int l = 0; l < ((temp.maxSize) - temp.cylinder.size()); l ++)

                	 cout<<"0";
}

void flip(tube left[], tube right[]){

/* Pop the balls off each stack, and push them
into the opposite stack.*/

	for(int i = 0; i < 6; i ++){

		while((right[i].cylinder.size()) < right[i].maxSize && !left[i].cylinder.empty()){
		
			right[i].cylinder.push(left[i].cylinder.top());

			left[i].cylinder.pop();
		}
	}
}

void rotateClockwise(tube tubes[]){

	//swap all tubes one position upwards.

	for(int i = 5; i > 0; i --){

		swap(tubes[i], tubes[(i+1)%6]);
	}
}

void rotateCounterClockwise(tube tubes[]){

	//take off the first tube, swap all tubes one position
	// backwards, and insert the first tube at the other end.

	vector<tube> v;

	tube temp;

	for(int i = 0; i < 6; i ++)

		v.push_back(tubes[i]);

	temp = v[0];

	v.erase (v.begin());

	v.insert(v.end(), temp);

	for(int i = 0; i < 6; i ++)

		tubes[i] = v[i];
}

int shuffle(tube left[], tube right[], int n){

	// randomly pick a move, either a rotate or a flip.

	char moves[2] = {'r', 'f'};

	int orientation = 0;

	// iterate as many times as specified by the user
	// in each iteration, perform the randomly picked move.
	// if it's a rotate, pick a random non-empty tube
	// push the balls against the gravity to the opposite
	// tube, and then rotate. This is the reverse of rotate.

	for(int i = 0; i < n; i ++){

		int RandIndex = rand() % 2;

		if(moves[RandIndex] == 'f'){

			if(orientation == 0){

				flip(left, right);
				orientation = 1;
			}
			else{

                                flip(right, left);
                                orientation = 0;
                        }	
		}
		else{
				int side;
		
				int tubeNum;

				bool picked = false;

				int x = 0;

				while(!picked && x < 5){
	
					tubeNum = rand()%6;

					if(!left[tubeNum].cylinder.empty()){

						picked = true;

						side = 0;
					}

					else if(!right[tubeNum].cylinder.empty()){

						picked = true;
                                                side = 1;
                                        }

					x ++;
				}

				int g = 0;

				if(side == 0){

					int randNum = rand()%(left[tubeNum].cylinder.size()) + 1;

					while((right[i].cylinder.size()) < right[i].maxSize && g < randNum){

                        			right[i].cylinder.push(left[i].cylinder.top());

                        			left[i].cylinder.pop();
	
						g++;
					}

					rotateClockwise(right);
				}
				else{

					int randNum = rand()%(right[tubeNum].cylinder.size()) + 1;

					while((left[i].cylinder.size()) < left[i].maxSize && g < randNum){

                                                left[i].cylinder.push(right[i].cylinder.top());

                                                right[i].cylinder.pop();

                                                g++;
                                        }

					rotateClockwise(left);
				}
		}
	}
	return orientation;
}

bool a_star(tube left[], tube right[], int orientation){

	// create a new node

	node * root = new node();

	// initialize the variables

	bool found = false;

	bool depthReached = false;

	int depth = 0;

	// set the initial state of the node

	for(int i = 0; i < 6; i ++){

		root -> left[i] = left[i];
		root -> right[i] = right[i];
	}

	//calculate f.

	root -> g = 0;

	root -> h = heuristicFunction(root -> left, root -> right);

	root -> pathCost();

	while(!found){

		// this priority queue is ordered by the f value.

		priority_queue<node*, vector<node*>, CompareNode> frontier;

	// push the node to the frontier

		frontier.push(root);

		depthReached = false;

		depth++;

		while(!depthReached){

			if(frontier.empty())
			
				break;

			node * parent = frontier.top();

			display(parent -> left, parent -> right);

			cout<<"h = "<<parent -> h<<endl;

			cout<<"depth = "<<depth<<endl;

			if((parent -> h) == 0){

				found = true;

				depthReached = true;

				traceSolution(parent);
			}
			else{

			// pop the node that will be expanded
		
				frontier.pop();

				if((parent -> g) > depth)

					depthReached = true;

				else{

			// for each legal move, generate a child
			// and push it to the frontier.

					for(int i = 1; i < 4; i ++){
		
						node * child = new node();

						for(int j = 0; j < 6; j ++){

                					child -> left[j] = parent -> left[j];
                					child -> right[j] = parent -> right[j];
        					}
						doMove(child, i, orientation);
			
				//calculate h, g, f.	

						child -> g = (parent -> g) + 1;

						child ->h = heuristicFunction(child-> left, child -> right);

						child -> pathCost();

				//connect it to parent
	
						child -> parent = parent;

				//add it to the queue

						frontier.push(child);
					}
				}
			}
		}
		while(!frontier.empty()){

			node * tempNode = frontier.top();

			frontier.pop();

			delete	tempNode;
		}
	}
	return found;
}

void doMove(node * temp, int move, int &orientation){

	if(move == 3){ 
	
		if(orientation == 0){
			rotateClockwise(temp -> left);
			temp -> lastOp = "rotate clockwise";
		}	
		else{
			rotateClockwise(temp -> right);
                        temp -> lastOp = "rotate clockwise";
		}
	}	
	else if(move == 2){

		if(orientation == 0){ 
			rotateCounterClockwise(temp -> left);
			temp -> lastOp = "rotate counter clockwise";
		}
		else{
			rotateCounterClockwise(temp -> right);
                        temp -> lastOp = "rotate counter clockwise";
                }	
	}
	else if(move == 1){
			
		if(orientation == 0){
		
			flip(temp -> left, temp -> right);

			orientation = 1;			
		}
		else{
			flip(temp -> right, temp -> left);

			orientation = 0;
		}
		temp -> lastOp = "flip";
	}
}

int heuristicFunction(tube right[], tube left[]){

	// calculate the manhattan distance once for all 
	// adjacent balls of the same color, then
	// calculate the sum of all manhattan distances
	// and since all six tubes could move in one move
	// divide the sum by six.

	int heuristicValue = 0;

	int manhattanDistance[21];

	int tempBall;

	for(int i = 0; i < 21; i ++)

		manhattanDistance[i] = 0;

	int index = 0;

	for(int i = 0; i < 6; i ++){

		int counter;

		if(!right[i].cylinder.empty()){
	
			counter = 0;

			for (stack<int> dump = right[i].cylinder; !dump.empty(); dump.pop()){

				if(counter >= 1)

					if(tempBall == dump.top())

						manhattanDistance[index] = 0;
					else{
						
						if(right[i].maxSize != dump.top())	
	
							manhattanDistance[index] = abs(right[i].maxSize - dump.top()) + 1;
						else
							manhattanDistance[index] = 0;
					}

				else{
					if(right[i].maxSize != dump.top())

						manhattanDistance[index] = abs(right[i].maxSize - dump.top()) + 1;
					else
                                                        manhattanDistance[index] = 0;
				}

				tempBall = dump.top();

				index ++;

				counter ++;
			}
		}
		if(!left[i].cylinder.empty()){
			
			counter = 0;

			for (stack<int> dump = left[i].cylinder; !dump.empty(); dump.pop()){

                        // Horizontal distance + 1 flip.                        

				if(counter >= 1)

                                        if(tempBall == dump.top())

                                                manhattanDistance[index] = 0;
                                        else{

					if(left[i].maxSize != dump.top())

                                		manhattanDistance[index] = abs(left[i].maxSize - dump.top()) + 1;
					else    
                                        	manhattanDistance[index] = 0;
					}
				else{
                                        if(right[i].maxSize != dump.top())

                                                manhattanDistance[index] = abs(right[i].maxSize - dump.top()) + 1;
                                        else
                                                        manhattanDistance[index] = 0;
                                }
				tempBall = dump.top();
                                index ++;
				counter ++;
                        }
                }
	}
	for(int i = 0; i < 21; i ++)

		heuristicValue += manhattanDistance[i];

	heuristicValue = heuristicValue/6;

	return heuristicValue;
}

void traceSolution(node * temp){

	/* This method will take as an input
	the linked list generated by our ida* 
	algorithm, and it will use it to generate a stack
	of strings, where each string is the move performed 
	to get to the state.*/

	stack<string> solution;

	node * pointer = temp;

	while(pointer -> parent != NULL){

		solution.push(pointer -> lastOp);
		
		pointer = pointer -> parent;
	}

	cout<<"Solution Steps: "<<endl;

	for (stack<string> dump = solution; !dump.empty(); dump.pop())

		cout<<solution.top()<<endl;
}

