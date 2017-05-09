# C-AI-Puzzle-Solver
Introduction:

Atomic Chaos is a tricky puzzle consisting of 6 tubes of different lengths arranged in a circle. In the starting position, all tubes are filled exactly halfway with balls of the same color. The shortest tube has only one black ball, the longest six red ones. All the tubes are split in half, and you can rotate one half of the puzzle so that the tubes in one half of the puzzle are matched against tubes of different lengths in the other half.


Program execution instructions:

•	This program runs on Linux 4.4.0-34-generic x86_64 (DMB 005)

•	All the files must be in the same directory

•	Compile using the command: g++ atomic.cpp

•	Run as: ./a.out


Program Description:

Once you start the program, it will display the puzzle in its initial state and it will ask for the k-value. As soon as you enter the value, it will generate 5 k-randomized puzzles and solve them.

Implementation:

In this project, I had to design and implement data structures for the puzzle itself, produce some (possibly crude) GUI for sanity checks, and write a program to randomize the puzzle.

The randomizer function should take as input the number of moves. A move consists of flipping the puzzle over or rotating it one cylinder (and allowing balls to drop). If you simply randomly assign balls/colors to positions, you may get unsolvable instances. So, instead of randomly assigning balls/colors to positions, we will use anti-moves. The randomizer picks a move randomly (either a flip or a rotate). If it picked rotate, the balls will be pushed against the gravity, and then the puzzle will be rotated by one cylinder. 

Puzzle Representation:

Each tube will be represented by an object. The object will consist of a stack to hold the balls, a variable to store the maximum number of balls the tube can contain, a pointer variable that points at the opposite tube, a method to check if the tube is full, and a method to check if the tube contains balls of the same color.



Moves Representation:

When we flip the puzzle, we will pop all the balls in each stack and push them to the opposite stack.

If we rotated the puzzle, we will set the tube pointer variable to point at the next opposite tube. 

The data structure is included in tube.h and tube.cpp


Search Algorithm Data Structure:

Each node will be represented by an object. The object consists of two arrays of stacks, each representing a set of tubes, a string to store the last operation, three variables to store the g, h, and f values, a pointer variable that points at the parent node, and a function that calculates f by adding g to h.


Sample GUI Output:

666666 000000

 55555 00000
 
  4444 0000
  
   333 000
   
    22 00
    
     1 0

h = 0

Enter the number of moves needed to randomize the puzzle: 47

    13 660000
    
     4 55000
     
562666 0000

 44400 000
 
  3355 00
  
   200 0

h = 5







666666 000000

 55555 00000
 
  4444 0000
  
   333 000
   
    22 00
    
     1 0

h = 0

Enter the number of moves needed to randomize the puzzle: 63

342666 000000

 55255 00000
 
  3163 0000
  
   544 000
   
    66 00
    
     4 0

h = 6

IDA* pseudocode:

•	Create a new node

•	Calculate its f value

•	Add it to the priority queue

•	Initialize depth to 0

•	Loop until solved:

  •	Loop until the depth is reached
  
    •	Create a node 

    •	Node = first element in the frontier
    
    •	If this node contains the solved state: return true

    •	For each legal move:

      •	Create a child node
      
      •	Connect it to its parent

      •	Add it to the queue

  •Increment the depth by one

Delete the old queue and create a new one.

Heuristic Evaluation Function:

•	For each tube, calculate the Manhattan distance once for all adjacent balls of the same color, where the Manhattan distance is the distance between the tube the ball is in and its goal tube plus 1 flip. 
•	Calculate the sum of all Manhattan distances. 
•	Since multiple balls could fall in one move, divide the sum by 6 (number of tubes). 


This heuristic is admissible because each ball needs at least n rotations and 1 flip to get to its goal tube, where n is the difference between the current tube and the goal tube. 

The heuristic evaluation function is included in atomic.cpp

