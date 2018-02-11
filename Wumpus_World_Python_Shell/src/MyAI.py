# ======================================================================
# FILE:        MyAI.py
#
# AUTHOR:      Abdullah Younis
#
# DESCRIPTION: This file contains your agent class, which you will
#              implement. You are responsible for implementing the
#              'getAction' function and any helper methods you feel you
#              need.
#
# NOTES:       - If you are having trouble understanding how the shell
#                works, look at the other parts of the code, as well as
#                the documentation.
#
#              - You are only allowed to make changes to this portion of
#                the code. Any changes to other portions of the code will
#                be lost when the tournament runs your code.
# ======================================================================

from Agent import Agent

class MyAI ( Agent ):

    def __init__ ( self ):
        # ======================================================================
        # YOUR CODE BEGINS
        # ======================================================================
        self._AgentX = 0
        self._AgentY = 0
        #self._track_back = []
        self._is_back = False
        self._dir = 2 #left : 0, up : 1, right : 2, down : 3


        pass
        # ======================================================================
        # YOUR CODE ENDS
        # ======================================================================

    def getAction( self, stench, breeze, glitter, bump, scream ):
        # ======================================================================
        # YOUR CODE BEGINS
        # ======================================================================
        #print(self._AgentY, self._AgentX, self._dir, bump, stench, breeze)
        if self._dir == 0 and self._AgentY == 0:
            return Agent.Action.CLIMB

        if self._is_back and self._AgentX == 0 and self._AgentY == 0:
            return Agent.Action.CLIMB

        if glitter:
            return Agent.Action.GRAB

        if stench or breeze or bump:
            self._is_back = True

        if self._is_back and self._dir != 0:
            self._dir -= 1
            return Agent.Action.TURN_LEFT

        if self._is_back and self._dir == 0:
            self._AgentY -= 1
            return Agent.Action.FORWARD


        self._AgentY += 1
        return Agent.Action.FORWARD


        '''To do list:
        1. Logic append the back tracking
        2. Logic selet the nect path
        '''





        
        #return Agent.Action.CLIMB
        # ======================================================================
        # YOUR CODE ENDS
        # ======================================================================

    def getBack(self, trace_path):
        if not trace_path:
            return Agent.Action.CLIMB

        return trace_path.pop()

    # ======================================================================
    # YOUR CODE BEGINS
    # ======================================================================

    
    # ======================================================================
    # YOUR CODE ENDS
    # ======================================================================