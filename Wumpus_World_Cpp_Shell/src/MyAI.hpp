// ======================================================================
// FILE:        MyAI.hpp
//
// AUTHOR:      Yuchun Liu & Jian-Lun Huang
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
    {   
        bool pit    = true;
        bool wumpus = true;
        bool breeze = false;
        bool stench = false;
        bool bump   = false;    
        bool visited = false;
    };

enum Direction 
    {
        RIGHT, 
        DOWN,  
        LEFT,  
        UP    
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
    bool _isShoot = false;
    bool _isMoveDone = true;
    pair <int, int> _wumpusPos = make_pair(0,0);
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
    
    void updateMap(bool stench, bool breeze, bool glitter, bool bump, bool scream);
    void getReturnMove();
    pair<int,int> nextPosition(bool bump);
    void decodePosition(pair<int,int> pos);
    void updatePosition(int x, int y, bool breeze, bool stench);
    void updatePositionNeighborPit(int x, int y);
    void updatePositionNeighborWumpus(int x, int y);
    pair<int,int> killWumpus(int x, int y);

    void moveUP();
    void moveDOWN();
    void moveLEFT();
    void moveRIGHT();
    void faceLEFT();
    void faceRIGHT();


    Action stackOperation();
};

#endif