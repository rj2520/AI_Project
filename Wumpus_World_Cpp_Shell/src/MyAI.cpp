// ======================================================================
// FILE:        MyAI.cpp
//
// AUTHOR:      Yuchun Liu & Jian-Lun Huang
//
// DESCRIPTION: AI for Wumpus World. The AI can locate the Wumpus, find 
// the gold, and most importantly will not die under any circustance.  
//
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
                cur_tile.breeze = true;
                cur_tile.stench = false;
                cur_tile.wumpus = true;
                cur_tile.pit = true;
            }
            temp_rol.push_back(cur_tile);
            // _agentMap.push_back(cur_tile);
        }
        _agentMap.push_back(temp_rol);
        temp_rol.clear();
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
    // cout << "_agentX:" << _agentX << "  _agentY:" << _agentY << endl;
    
    // if (bump) {
    //     if (_dir == RIGHT) {
    //         _agentX--;
    //     } else {
    //         _agentY--;
    //     }
    //     return GRAB;
    // }

    // finish all action remaining
    if (_actionQueue.size() > 0) {
        Action action = _actionQueue.front();
        _actionQueue.pop();
        return action;
    }

    // gold 
    if (glitter){
        _is_return = true;
        updateMap(stench, breeze, glitter, bump, scream);
        return GRAB;
    }

    //TRY TO KILL THE WUMPUS
    
    if (scream){
    	for (int row = 0; row < _row; row++){
    		for (int col = 0; col < _col; col++){
    			_agentMap[row][col].wumpus = false;
    			_agentMap[row][col].stench = false;
    		}
    	}
    }
    //When facing the stench at the beginning, we shoot the arrow 
    //and locate the wumpus, if we hear the scream, then we killed 
    //the wumpus, otherwise, wumpus is at (2,1)    

    if(_isShoot && !scream && _agentX == 1 && _agentY == 1){
    	for (int row = 0; row < _row; row++){
    		for (int col = 0; col < _col; col++){
    			_agentMap[row][col].wumpus = false;
    			_agentMap[row][col].stench = false;
    		}
    	}
    	_agentMap[2][1].wumpus = true;
    }

    if (_agentX == 1 && _agentY == 1 && stench && !_isShoot){
    	_isShoot = true;
    	return SHOOT;
    }

    //General case, we can locate the wumpus by examing two
    //confirmed stench tile. logic is similar with the previous one
    //s     s
    //   @s
    //s     s

    if (stench && !_isShoot){
    	_wumpusPos = killWumpus(_agentX, _agentY);

    }

    if (_isShoot && _wumpusPos.first != 0 && stench){

    	for (int row = 0; row < _row; row++){
    		for (int col = 0; col < _col; col++){
    			_agentMap[row][col].wumpus = false;
    			_agentMap[row][col].stench = false;
    		}
    	}
    	_agentMap[_wumpusPos.second][_wumpusPos.first].wumpus = true;
    	_wumpusPos.first = 0;
    }
    //END OF KILLING THE WUMPUS


    // always pop stack
    if (_is_return) {
        if (_stack.size() == 0 && _agentX == 1 && _agentY == 1) {
            return CLIMB;
        }

        return stackOperation();
    }
    
    //every time before we decide an action, we need to update our own map. 
    updateMap(stench, breeze, glitter, bump, scream); //其實不用glitter

    //pos we want to move to
    pair<int,int> pos = nextPosition(bump);

    //save go back route
    if (pos.first != _agentX || pos.second != _agentY) {
        pair<int,int> curPos;
        curPos.first = _agentX;
        curPos.second = _agentY;
        _stack.push(curPos);
    }

    decodePosition(pos);

    if (_actionQueue.size() > 0) {
        Action action = _actionQueue.front();
        _actionQueue.pop();
        return action;
    } else {
        //no where to move(including going backward)
        if (_stack.size() == 0 && _agentX == 1 && _agentY == 1) {
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
        // if (!_agentMap[y][x - 1].visited)
            _agentMap[y][x - 1].pit = false;
        // if (!_agentMap[y][x + 1].visited)
            _agentMap[y][x + 1].pit = false;
        // if (!_agentMap[y - 1][x].visited)
            _agentMap[y - 1][x].pit = false;
        // if (!_agentMap[y + 1][x].visited)
            _agentMap[y + 1][x].pit = false;
    } else {
        _agentMap[y][x].breeze = true;
        // then update around tile(to eliminate pits)
        updatePositionNeighborPit(x + 1, y);
        updatePositionNeighborPit(x - 1, y);
        updatePositionNeighborPit(x, y - 1);
        updatePositionNeighborPit(x, y + 1);
    }

    //wumpus actually can be known earlier
    if (!_isShoot){
    	if (!stench) {
        //update around tile into "wumpus" = false"
        // if (!_agentMap[y][x - 1].visited)
            _agentMap[y][x - 1].wumpus = false;
        // if (!_agentMap[y][x + 1].visited)
            _agentMap[y][x + 1].wumpus = false;
        // if (!_agentMap[y - 1][x].visited)
            _agentMap[y - 1][x].wumpus = false;
        // if (!_agentMap[y + 1][x].visited)
            _agentMap[y + 1][x].wumpus = false;
    	} else {
	        _agentMap[y][x].stench = true;
	        updatePositionNeighborWumpus(x + 1, y);
	        updatePositionNeighborWumpus(x - 1, y);
	        updatePositionNeighborWumpus(x, y + 1);
	        updatePositionNeighborWumpus(x, y - 1);
    	}

    }
    
}
// ./Wumpus_World -f /home/parallels/Desktop/final_project/AI_Project/Wumpus_World_World_Generator/Worlds

bool MyAI::checkValid(int x, int y) {
    if (x < 0 || x >= _col || y < 0 || y >= _row || _agentX == x || _agentY == y) {
        return false;
    } else {
        return true;
    }
}

void MyAI::updatePositionNeighborPit(int x, int y) {
    //check around is visited and not breeze, this tile would not be a pit
    if (checkValid(x + 1, y) && _agentMap[y][x + 1].visited && !_agentMap[y][x + 1].breeze && !_agentMap[y][x + 1].bump) {
        _agentMap[y][x].pit = false;
    } else if (checkValid(x - 1, y) && _agentMap[y][x - 1].visited && !_agentMap[y][x - 1].breeze && !_agentMap[y][x - 1].bump) {
        _agentMap[y][x].pit = false;
    } else if (checkValid(x, y + 1) && _agentMap[y + 1][x].visited && !_agentMap[y + 1][x].breeze && !_agentMap[y + 1][x].bump) {
        _agentMap[y][x].pit = false;
    } else if (checkValid(x, y - 1) && _agentMap[y - 1][x].visited && !_agentMap[y - 1][x].breeze && !_agentMap[y - 1][x].bump) {
        _agentMap[y][x].pit = false;
    } 
}

void MyAI::updatePositionNeighborWumpus(int x, int y) {
    //check around is visited and not breeze, this tile would not be a pit
    if (checkValid(x + 1, y) && _agentMap[y][x + 1].visited && !_agentMap[y][x + 1].stench && !_agentMap[y][x + 1].bump) {
        _agentMap[y][x].wumpus = false;
    } else if (checkValid(x - 1, y) && _agentMap[y][x - 1].visited && !_agentMap[y][x - 1].stench && !_agentMap[y][x - 1].bump) {
        _agentMap[y][x].wumpus = false;
    } else if (checkValid(x, y + 1) && _agentMap[y + 1][x].visited && !_agentMap[y + 1][x].stench && !_agentMap[y + 1][x].bump) {
        _agentMap[y][x].wumpus = false;
    } else if (checkValid(x, y - 1) && _agentMap[y - 1][x].visited && !_agentMap[y - 1][x].stench && !_agentMap[y - 1][x].bump) {
        _agentMap[y][x].wumpus = false;
    } 
}


pair<int, int>  MyAI::killWumpus(int x, int y){
	pair <int, int> pos;
	pos.first = 0;
	pos.second = 0;

	if (checkValid(x-1,y-1) && _agentMap[y-1][x-1].stench && _agentMap[y-1][x-1].visited){
		pos.first = x;
		pos.second = y - 1;
		_isShoot = true;
		faceLEFT();
		return pos;
	}

	if (checkValid(x-1,y+1) && _agentMap[y+1][x-1].stench && _agentMap[y+1][x-1].visited){
		pos.first = x;
		pos.second = y + 1;
		_isShoot = true;
		faceLEFT();
		return pos;
	}

	else if (checkValid(x+1,y+1) && _agentMap[y+1][x+1].stench && _agentMap[y+1][x+1].visited){
		pos.first = x;
		pos.second = y + 1;
		_isShoot = true;
		faceRIGHT();
		return pos;
	}

	else if (checkValid(x-1,y-1) && _agentMap[y-1][x+1].stench && _agentMap[y-1][x+1].visited){
		pos.first = x;
		pos.second = y - 1;
		_isShoot = true;
		faceRIGHT();
		return pos;
	}

	return pos;
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

pair<int,int> MyAI::nextPosition(bool bump) {
    //start from current direction, at most turn 3 times
    for (int i = _dir; i <= _dir + 3; i++) {
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
}

//   ./Wumpus_World -f ../../Wumpus_World_World_Generator/Worlds/
void MyAI::updateMap(bool stench, bool breeze, bool glitter, bool bump, bool scream) {
    if (_agentMap[_agentY][_agentX].visited) {
        return;
    }

    if (bump) {
        if (_dir == RIGHT) {
            _agentX--;
            _stack.pop();
            for (int row = 0; row < _row; row++) {
                _agentMap[row][_agentX + 1].bump = true;
                _agentMap[row][_agentX + 1].visited = true;
                _agentMap[row][_agentX + 1].breeze = true;
                _agentMap[row][_agentX + 1].stench = false;
            }
        }
        if (_dir == UP) {
            _agentY--;
            _stack.pop();
            for (int col = 0; col < _col; col++) {
                _agentMap[_agentY + 1][col].bump = true;
                _agentMap[_agentY + 1][col].visited = true;
                _agentMap[_agentY + 1][col].breeze = true;
                _agentMap[_agentY + 1][col].stench = false;
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

void MyAI::moveRIGHT() {
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

//USED TO KILL THE WUMPUS
void MyAI::faceLEFT(){
    if(_dir == LEFT) {
        _actionQueue.push(SHOOT);
    } else if(_dir == UP) {
        _actionQueue.push(TURN_LEFT);
        _actionQueue.push(SHOOT);
    } else if(_dir == DOWN) {
        _actionQueue.push(TURN_RIGHT);
        _actionQueue.push(SHOOT);
    } else if (_dir == RIGHT) {
        _actionQueue.push(TURN_LEFT);
        _actionQueue.push(TURN_LEFT);
        _actionQueue.push(SHOOT);
    }
    _dir = LEFT;
}

void MyAI::faceRIGHT(){
	if(_dir == RIGHT){
        _actionQueue.push(SHOOT);
    } else if(_dir == UP){
        _actionQueue.push(TURN_RIGHT);
        _actionQueue.push(SHOOT);
    } else if(_dir == DOWN){
        _actionQueue.push(TURN_LEFT);
        _actionQueue.push(SHOOT);
    } else if (_dir == LEFT) {
        _actionQueue.push(TURN_LEFT);
        _actionQueue.push(TURN_LEFT);
        _actionQueue.push(SHOOT);
    }
    _dir = RIGHT;
}


// ======================================================================
// YOUR CODE ENDS
// ======================================================================