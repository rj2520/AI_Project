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
		for (int j = 0; j < _col; j++) {
			MyTile cur_tile;
			if (i == 0 || j == 0 || i  == _row - 1 || j == _col - 1) {
				cur_tile.bump = true;
			}
			_agentMap.push_back(cur_tile);
		}
	}
	
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
	

	// finish all action remaining
	if (_actionQueue.size() > 0) {
		Action action = _actionQueue.front();
		_actionQueue.pop();
		return action;
	}

	// gold 
	if (glitter){
		_is_return = true;
		//set this tile as known
		agentMap[_agentX][_agentY].visited = true;
		return GRAB;
	}

	// always pop stack
	if (is_return) {
		return stackOperation();
	}

	//every time before we decide an action, we need to update our own map. 
	updateMap(stench, breeze, glitter, bump, scream); //其實不用glitter

	//pos we want to move to
	pair<int,int> pos = nextPosition();
	decodePosition(pos);

	if (_actionQueue.size() > 0) {
		Action action = _actionQueue.front();
		_actionQueue.pop();
		return action;
	} else {
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

void MyAI::updateAround(int x, int y, bool breeze, bool stench) {
	MyTile tile = _agentMap[x][y];

	if (_agentX == x && _agentY == y) {
		if (!breeze) {
			//update around tile into "pits = false"
			
		} else {
			tile.breeze = true;
		}
		if (!stench) {
			//update around tile into "wumpus" = false"
		} else {
			tile.stench = true;
		}


	}

	_agentMap[x][y] = tile;
}


Action MyAI::stackOperation() {

	if (_stack.size() == 0) {
		return CLIMB;
	}

	pair<int,int> pos = _stack.top();

	//function tell us where to move (parameter: pos)
	decodePosition(pos);

	Action action = _actionQueue.front();
	_actionQueue.pop();
	return action;
}


void MyAI::decodePosition(pair<int, int> pos) {
	int x = pos.first();
	int y = pos.second();

	//update _actionQueue base on our current position
	if (_agentX + 1 == x) {
		moveRIGHT();
	} else if (_agentX - 1 == x) {
		moveLEFT();
	} else if (_agentY + 1 == y) {
		moveUP();
	} else {
		moveDOWN();
	}
}

pair<int,int> MyAI::nextPosition() {

	//start from current direction, at most turn 3 times
	for (int i = _dir; i < _dir + 3; i++) {
		int dir = i % 4;
		if (dir == RIGHT) {
			MyTile tile = _agentMap[_agentX + 1][_agentY];

			if (!tile.pit && !tile.wumpus && !tile.visited && !tile.bump) {
				pair<int,int> pos;
				pos.first(_agentX + 1);
				pos.second(_agentY);
				return pos;
			}
		} else if (dir == LEFT) {
			MyTile tile = _agentMap[_agentX - 1][_agentY];

			if (!tile.pit && !tile.wumpus && !tile.visited && !tile.bump) {
				pair<int,int> pos;
				pos.first(_agentX - 1);
				pos.second(_agentY);
				return pos;
			}
		} else if (dir == UP) {
			MyTile tile = _agentMap[_agentX][_agentY + 1];

			if (!tile.pit && !tile.wumpus && !tile.visited && !tile.bump) {
				pair<int,int> pos;
				pos.first(_agentX);
				pos.second(_agentY + 1);
				return pos;
			}
		} else if (dir == DOWN) {
			MyTile tile = _agentMap[_agentX][_agentY - 1];

			if (!tile.pit && !tile.wumpus && !tile.visited && !tile.bump) {
				pair<int,int> pos;
				pos.first(_agentX);
				pos.second(_agentY - 1);
				return pos;
			}
		}
	}
	pair<int,int> pos;
	pos.first(_agentX);
	pos.second(_agentY);
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


void MyAI::updateMap(bool stench, bool breeze, bool glitter, bool bump, bool scream) {
	if (agentMap[_agentX][_agentY].visited) {
		return;
	}

	//實際地圖從 (1,1) 開始走
	for (int row = 1; i < world.size(); row++) {
		for (int col = 1; col < world[0].size(); col++) {

		}
	}


	// 若是有stench，先檢查當前這格的"四周" 有沒有Wumpus，
	// 有的話，  即可確認monster位置，並移除其他monster，  然後殺死monster!
	// 沒有的話，將四週設為monster


	// 若是這格沒有breeze 則四周的Pits都消掉
	// 若是這格有breeze，且其他都visit過剩下一個可能性，也可以確認pits

	// 設定成P之前，有沒有已知的信息 (visit過的點去推斷...)

	//(先設定 有問題再改掉)
	//設定P後，先把那個點周邊再去判斷一次(breeze之類的)   (不用先設P那些的，check不出東西在設定)
	//	  	 	  check
	//	    check check check
	//check check   me  check check
	//	    check check check
	//	          check

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