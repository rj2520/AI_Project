// ======================================================================
// FILE:        MyAI.cpp
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

#include "MyAI.hpp"

MyAI::MyAI() : Agent()
{
	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================
	
	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
}
	
Agent::Action MyAI::getAction
(
	bool stench,
	bool breeze,
	bool glitter,
	bool bump,
	bool scream
)
{
	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================
	//printf("agentY: %d, Dir: %d, path_back: %d\n", agentY, dir, is_return );
	if (agentY == 0 && dir == 0)
		return CLIMB;

	if (glitter){
		is_return = true;
		return GRAB;
	}

	if (stench || breeze || bump )
		is_return = true;

	if (is_return && dir != 0){
		dir -= 1;
		return TURN_LEFT;
	}

	if (is_return && dir == 0){
		agentY -= 1;
		return FORWARD;
	}

	agentY += 1;
	return FORWARD;
	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
	
}

// ======================================================================
// YOUR CODE BEGINS
// ======================================================================


// ======================================================================
// YOUR CODE ENDS
// ======================================================================