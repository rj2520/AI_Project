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
	
	// init map   
	// ======================================================================
	// (1,1 is easier because we don't need to consider edge case like (i-1) < 0
	// ======================================================================
	for (int i = 0; i < _row; i++) {
		vector <MyTile> temp_rol;
		for (int j = 0; j < _col; j++) {
			MyTile cur_tile;
			if (i == 0 || j == 0 || i  == _row - 1 || j == _col - 1) {
				cur_tile.bump = true;
			}
			temp_rol.push_back(cur_tile);
			// _agentMap.push_back(cur_tile);
		}
		_agentMap.push_back(temp_rol);
		temp_rol.clear();
	}

	// for (int i = 0; i < _row; i++) {
	// 	for (int j = 0; j < _col; j++) {

	// 	}
	// }

	// piar<int,int> pos;
	// pos.fisrt = 1;
	// pos.second = 1;
	// _stack.push(pos);

	// cout << _agentMap.size() << endl;
	// cout << _agentMap[0].size() << endl;
	
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
	//updateMap後，前方能走就走，不能走就轉向，再檢查能不能走  (follow up: 怎麼轉可以比較快轉到能走的方向(easy to implement) )

	// ======================================================================
	// ======================================================================
	// ======================================================================
	// cur_loc.fisrt(_agentX);
	// cur_loc.second(_agentY);
	// cur_tile.stench = stench;
	// cur_tile.breeze = breeze;
	// cur

	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================
	//printf("agentY: %d, Dir: %d, path_back: %d\n", agentY, dir, is_return );

	// cout << "x:" << _agentX << "  y:" << _agentY << "  dir:" << _dir << endl;
	// cout << "breeze:" << breeze << "   scent:" << stench << endl;
	
	// finish all action remaining
	if (_actionQueue.size() > 0) {
		Action action = _actionQueue.front();
		// cout << action << endl;
		_actionQueue.pop();
		return action;
	}

	// gold 
	if (glitter){
		_is_return = true;
		//set this tile as known
		// _agentMap[_agentX][_agentY].visited = true;
		_agentMap[_agentY][_agentX].visited = true;		
		return GRAB;
	}
	
	// always pop stack
	if (_is_return) {
		if (_stack.size() == 0 && _agentX == 1 && _agentY == 1) {
			// cout << "CLIMB" << endl;
			return CLIMB;
		}

		return stackOperation();
	}
	
	//every time before we decide an action, we need to update our own map. 
	updateMap(stench, breeze, glitter, bump, scream); //其實不用glitter

	//pos we want to move to
	pair<int,int> pos = nextPosition();

	// cout << "next_X:" << pos.first << "  next_Y:" << pos.second << endl;

	//save go back route
	if (pos.first != _agentX || pos.second != _agentY) {
		pair<int,int> curPos;
		curPos.first = _agentX;
		curPos.second = _agentY;
		_stack.push(curPos);
	}
	// cout << "stack_size:" << _stack.size() << endl;

	decodePosition(pos);

	if (_actionQueue.size() > 0) {
		Action action = _actionQueue.front();
		_actionQueue.pop();
		return action;
	} else {
		//no where to move(including going backward)
		if (_stack.size() == 0 && _agentX == 1 && _agentY == 1) {
			// cout << "CLIMB" << endl;
			return CLIMB;
		}
		//no where to move, pop stack
		return stackOperation();
	}

	
	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
	
}

// ======================================================================
// YOUR CODE BEGINS
// ======================================================================

void MyAI::updatePosition(int x, int y, bool breeze, bool stench) {
	if (x < 0 || x >= _col || y < 0 || y >= _row) {
		return;
	}
	if (!breeze) {
		//update around tile into "pits = false"
		_agentMap[y][x - 1].pit = false;
		_agentMap[y][x + 1].pit = false;
		_agentMap[y - 1][x].pit = false;
		_agentMap[y + 1][x].pit = false;
	} else {
		// cout << "PPPPPP" << endl;
		_agentMap[y][x].breeze = true;
		// then update around tile(to eliminate pits)
		updatePositionNeighborPit(x + 1, y);
		updatePositionNeighborPit(x - 1, y);
		updatePositionNeighborPit(x, y - 1);
		updatePositionNeighborPit(x, y + 1);
		// cout << "PPPP_END" << endl;
	}

	//wumpus actually can be known earlier
	if (!stench) {
		//update around tile into "wumpus" = false"
		_agentMap[y][x - 1].wumpus = false;
		_agentMap[y][x + 1].wumpus = false;
		_agentMap[y - 1][x].wumpus = false;
		_agentMap[y + 1][x].wumpus = false;
	} else {
		// cout << "WWWWWWWWWWWWWWWWW" << endl;
		_agentMap[y][x].stench = true;
		updatePositionNeighborWumpus(x + 1, y);
		updatePositionNeighborWumpus(x - 1, y);
		updatePositionNeighborWumpus(x, y + 1);
		updatePositionNeighborWumpus(x, y - 1);
		// cout << "WWWW_END" << endl;
	}
}

bool MyAI::checkValid(int x, int y) {
	if (x < 0 || x >= _col || y < 0 || y >= _row || _agentX == x || _agentY == y) {
		return false;
	} else {
		return true;
	}
}

void MyAI::updatePositionNeighborPit(int x, int y) {

	//check around is visited and not breeze, this tile would not be a pit
	if (checkValid(x+1, y) && _agentMap[y][x + 1].visited && !_agentMap[y][x + 1].breeze) {
		_agentMap[y][x].pit = false;
	} else if (checkValid(x-1, y) &&_agentMap[y][x - 1].visited && !_agentMap[y][x - 1].breeze) {
		_agentMap[y][x].pit = false;
	} else if (checkValid(x, y + 1) && _agentMap[y + 1][x].visited && !_agentMap[y + 1][x].breeze) {
		_agentMap[y][x].pit = false;
	} else if (checkValid(x, y -1) && _agentMap[y - 1][x].visited && !_agentMap[y - 1][x].breeze) {
		_agentMap[y][x].pit = false;
	} 
}

void MyAI::updatePositionNeighborWumpus(int x, int y) {
	//check around is visited and not breeze, this tile would not be a pit
	if (checkValid(x + 1, y) && _agentMap[y][x + 1].visited && !_agentMap[y][x + 1].breeze) {
		_agentMap[y][x].wumpus = false;
	} else if (checkValid(x - 1, y) && _agentMap[y][x - 1].visited && !_agentMap[y][x - 1].stench) {
		_agentMap[x][y].wumpus = false;
	} else if (checkValid(x, y + 1) && _agentMap[y + 1][x].visited && !_agentMap[y + 1][x].stench) {
		_agentMap[y][x].wumpus = false;
	} else if (checkValid(x, y - 1) && _agentMap[y - 1][x].visited && !_agentMap[y - 1][x].stench) {
		_agentMap[y][x].wumpus = false;
	} 
}


Agent::Action MyAI::stackOperation() {

	if (_stack.size() == 0 && _agentX == 1 && _agentY == 1) {
		return CLIMB;
	}

	pair<int,int> pos = _stack.top();
	_stack.pop();

	//function tell us where to move (parameter: pos)
	decodePosition(pos);

	Action action = _actionQueue.front();
	_actionQueue.pop();
	return action;
}


void MyAI::decodePosition(pair<int, int> pos) {
	int x = pos.first;
	int y = pos.second;

	//update _actionQueue base on our current position
	if (_agentX + 1 == x) {
		moveRIGHT();
	} else if (_agentX - 1 == x) {
		moveLEFT();
	} else if (_agentY + 1 == y) {
		moveUP();
	} else if (_agentY - 1 == y) {
		moveDOWN();
	}
}

pair<int,int> MyAI::nextPosition() {

	//start from current direction, at most turn 3 times
	for (int i = _dir; i < _dir + 3; i++) {
		int dir = i % 4;
		if (dir == RIGHT) {
			MyTile tile = _agentMap[_agentY][_agentX + 1];

			if (!tile.pit && !tile.wumpus && !tile.visited && !tile.bump) {
				pair<int,int> pos;
				pos.first = _agentX + 1;
				pos.second = _agentY;
				return pos;
			}
		} else if (dir == LEFT) {
			MyTile tile = _agentMap[_agentY][_agentX - 1];

			if (!tile.pit && !tile.wumpus && !tile.visited && !tile.bump) {
				pair<int,int> pos;
				pos.first = _agentX - 1;
				pos.second = _agentY;
				return pos;
			}
		} else if (dir == UP) {
			MyTile tile = _agentMap[_agentY + 1][_agentX];

			if (!tile.pit && !tile.wumpus && !tile.visited && !tile.bump) {
				pair<int,int> pos;
				pos.first = _agentX;
				pos.second = _agentY + 1;
				return pos;
			}
		} else if (dir == DOWN) {
			MyTile tile = _agentMap[_agentY - 1][_agentX];

			if (!tile.pit && !tile.wumpus && !tile.visited && !tile.bump) {
				pair<int,int> pos;
				pos.first = _agentX;
				pos.second = _agentY - 1;
				return pos;
			}
		}
	}
	pair<int,int> pos;
	pos.first = _agentX;
	pos.second = _agentY;
	return pos;

// dir
// RIGHT, // 0
// DOWN,  // 1
// LEFT,    // 2
// UP   // 3

/*
Tile
bool pit    = true;
bool wumpus = true;
bool breeze = false;
bool stench = false;
bool bump 	= false;	
bool visited = false;

*/

	// if (_dir == RIGHT) {
	// 	MyTile tile = _agentMap[_agentX + 1][_agentY];
	// 	// can go this direction
	// 	if (!tile.pit && !tile.wumpus && !tile.visited && !tile.bump) {
	// 		moveRIGHT(); // update actionQueue
	// 	} else {
	// 		//need to change into better way (look up the current map)
	// 		_actionQueue.push(TURN_LEFT);
	// 	}
	// } else if(_dir == LEFT) {
	// 	MyTile tile = _agentMap[_agentX - 1][_agentY];
	// 	// can go this direction
	// 	if (!tile.pit && !tile.wumpus && !tile.visited && !tile.bump) {
	// 		moveLEFT(); // update actionQueue
	// 	} else {
	// 		//need to change into better way (look up the current map)
	// 		_actionQueue.push(TURN_LEFT);
	// 	}
	// } else if (_dir == UP) {
	// 	MyTile tile = _agentMap[_agentX][_agentY + 1];
	// 	// can go this direction
	// 	if (!tile.pit && !tile.wumpus && !tile.visited && !tile.bump) {
	// 		moveUP(); // update actionQueue
	// 	} else {
	// 		//need to change into better way (look up the current map)
	// 		_actionQueue.push(TURN_LEFT);
	// 	}
	// } else {
	// 	MyTile tile = _agentMap[_agentX][_agentY - 1];
	// 	// can go this direction
	// 	if (!tile.pit && !tile.wumpus && !tile.visited && !tile.bump) {
	// 		moveDOWN(); // update actionQueue
	// 	} else {
	// 		//need to change into better way (look up the current map)
	// 		_actionQueue.push(TURN_LEFT);
	// 	}
	// }
}

//   ./Wumpus_World -f ../../Wumpus_World_World_Generator/Worlds/
void MyAI::updateMap(bool stench, bool breeze, bool glitter, bool bump, bool scream) {
	if (_agentMap[_agentY][_agentX].visited) {
		return;
	}

	if (bump) {
		if (_dir == RIGHT) {
			for (int row = 0; row < _row; row++) {
				_agentMap[row][_agentX + 1].bump = true;
				_agentMap[row][_agentX + 1].visited = true;
				_agentMap[row][_agentX + 1].breeze = true;
				_agentMap[row][_agentX + 1].stench = true;
			}
		}
		if (_dir == UP) {
			for (int col = 0; col < _col; col++) {
				_agentMap[_agentY + 1][col].bump = true;
				_agentMap[_agentY + 1][col].visited = true;
				_agentMap[_agentY + 1][col].breeze = true;
				_agentMap[_agentY + 1][col].stench = true;
			}
		}
	}

	_agentMap[_agentY][_agentX].visited = true;
	updatePosition(_agentX, _agentY, breeze, stench);
}

/* ------------------- MAKE SIMPLE MOVES -----------------------*/
//RIGHT 0, DOWN 1, LEFT 2, UP 3 clockwise is TURN_RIGHT, anti-clockwise is TURN_LEFT
void MyAI::moveUP() {
	if(_dir == UP){
		_actionQueue.push(FORWARD);
	}else if(_dir == LEFT){
		_actionQueue.push(TURN_RIGHT);
		_actionQueue.push(FORWARD);
	} else if(_dir == RIGHT){
		_actionQueue.push(TURN_LEFT);
		_actionQueue.push(FORWARD);
	} else if (_dir == DOWN) {
		_actionQueue.push(TURN_LEFT);
		_actionQueue.push(TURN_LEFT);
		_actionQueue.push(FORWARD);
	}
	_agentY++;
	_dir = UP;
}
void MyAI::moveDOWN(){
	if(_dir == DOWN) {
		_actionQueue.push(FORWARD);
	} else if(_dir == LEFT) {
		_actionQueue.push(TURN_LEFT);
		_actionQueue.push(FORWARD);
	} else if(_dir == RIGHT) {
		_actionQueue.push(TURN_RIGHT);
		_actionQueue.push(FORWARD);
	} else if (_dir == UP) {
		_actionQueue.push(TURN_LEFT);
		_actionQueue.push(TURN_LEFT);
		_actionQueue.push(FORWARD);
	}
	_agentY--;
	_dir = DOWN;
}
//RIGHT 0, DOWN 1, LEFT 2, UP 3 clockwise is TURN_RIGHT, anti-clockwise is TURN_LEFT
void MyAI::moveLEFT() {
	if(_dir == LEFT) {
		_actionQueue.push(FORWARD);
	} else if(_dir == UP) {
		_actionQueue.push(TURN_LEFT);
		_actionQueue.push(FORWARD);
	} else if(_dir == DOWN) {
		_actionQueue.push(TURN_RIGHT);
		_actionQueue.push(FORWARD);
	} else if (_dir == RIGHT) {
		_actionQueue.push(TURN_LEFT);
		_actionQueue.push(TURN_LEFT);
		_actionQueue.push(FORWARD);
	}
	_dir = LEFT;
	_agentX--;
}

//RIGHT 0, DOWN 1, LEFT 2, UP 3 clockwise is TURN_RIGHT, anti-clockwise is TURN_LEFT
void MyAI::moveRIGHT(){
	if(_dir == RIGHT){
		_actionQueue.push(FORWARD);
	} else if(_dir == UP){
		_actionQueue.push(TURN_RIGHT);
		_actionQueue.push(FORWARD);
	} else if(_dir == DOWN){
		_actionQueue.push(TURN_LEFT);
		_actionQueue.push(FORWARD);
	} else if (_dir == LEFT) {
		_actionQueue.push(TURN_LEFT);
		_actionQueue.push(TURN_LEFT);
		_actionQueue.push(FORWARD);
	}
	_dir = RIGHT;
	_agentX++;
}
// ======================================================================
// YOUR CODE ENDS
// ======================================================================