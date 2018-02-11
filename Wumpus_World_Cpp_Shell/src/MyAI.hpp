// ======================================================================
// FILE:        MyAI.hpp
//
// AUTHOR:      Abdullah Younis
//
// DESCRIPTION: This file contains your agent class, which you will
//              implement. You are responsible for implementing the
//              'getAction' function and any helper methods you feel you
//              need.
//
// NOTES:       - If you are having trouble understanding how the shell
//                works, look at the other parts of the code, as well as
//                the documentation.
//
//              - You are only allowed to make changes to this portion of
//                the code. Any changes to other portions of the code will
//                be lost when the tournament runs your code.
// ======================================================================

#ifndef MYAI_LOCK
#define MYAI_LOCK

#include "Agent.hpp"
#include <stdio.h>

class MyAI : public Agent
{
private:
	bool is_return = false;
	int agentX = 0;
	int agentY = 0;
	int dir = 2; //Left : 0, Up : 1, Right : 2, Down : 3


public:
	MyAI ( void );

	
	Action getAction
	(
		bool stench,
		bool breeze,
		bool glitter,
		bool bump,
		bool scream
	);
	
	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================
	
	
	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
};

#endif