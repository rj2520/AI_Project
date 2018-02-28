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
#include <vector>
#include <utility>
#include <queue>
#include <stack>
#include <iostream>


using namespace std;

struct MyTile
	{	// 確定的pits或wumpus, 設為visited

		bool pit    = true;
		bool wumpus = true;
		bool breeze = false;
		bool stench = false;
		bool bump 	= false;	
		bool visited = false;
		//bool gold   = false;
		//bool known = false;
	};

enum Direction 
	{
		RIGHT, // 0
		DOWN,  // 1
		LEFT,    // 2
		UP   // 3
  	};

class MyAI : public Agent
{
private:
	bool _is_return = false;
	int _agentX = 1;
	int _agentY = 1;
	int _dir = RIGHT;  //RIGHT 0, DOWN 1, LEFT 2, UP 3
	int _row = 9; // max_maze: 7*7
	int _col = 9;
	bool _isKillMonster = false;
	bool _isMoveDone = true;
	//map<pair, MyTile> agentMap;
	vector<vector<MyTile>> _agentMap;
	stack<pair<int, int>> _stack;
	queue<Action> _actionQueue;

	bool checkValid(int x, int y);

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

	void updateMap(bool stench, bool breeze, bool glitter, bool bump, bool scream);
	void getReturnMove();
	pair<int,int> nextPosition();
	void decodePosition(pair<int,int> pos);
	void updatePosition(int x, int y, bool breeze, bool stench);
	void updatePositionNeighborPit(int x, int y);
	void updatePositionNeighborWumpus(int x, int y);

	void moveUP();
	void moveDOWN();
	void moveLEFT();
	void moveRIGHT();

	Action stackOperation();
	
	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
};

#endif