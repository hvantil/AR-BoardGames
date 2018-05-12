#include <stdio.h>
#include <vector>
#include <ysclass.h>
#include <iostream>
#include <stdlib.h>
#include <ctype.h>
#include <thread>
#include <stdlib.h>
#include <objectStl.h>
#include <cvStuff.h>
#include <curl/curl.h>
#include <string>
#include <ysclass.h>
#include <fslazywindow.h>

#include "ttt.h"
#include "c4.h"
#include "battleship.h"

using namespace std;

void HardReset();

class connect4board{
public:
	Connect4 c4;
	vector<Object> tokens;
	Object board;
	int player;
	void init(stlObject * big, stlObject * small);
	void update();
	void draw();
	void start();
	int getCollision(openCVcam * camera, int mx, int my) const;
	// void sendCol(int column);
	void sendIndex(int index);
	void leave();
	void DrawFloor();
};


class ttt_board{
public:
	TicTacToe ttt;
	vector<Object> x_tokens;
	vector<Object> o_tokens;
	Object board;
	void init(stlObject* big, stlObject* x_token, stlObject* o_token);
	void update();
	void draw();
	void start();
	int getCollision(openCVcam * camera, int mx, int my) const;
	void sendIndex(int index);
	void leave();
};

class battleshipBoard{
public:
	double scale;
	int joinedFirst;
	Battleship bs;
	vector<Object> pegs;
	vector<Object> ships1;
	Object board1;
	vector<Object> ships2;
	Object board2;
	vector<Object> shipPegs1;
	vector<Object> shipPegs2;
	int player = 0;
	void init(stlObject * big, stlObject * peg,stlObject * s2,stlObject * s3,stlObject * s4,stlObject * s5);
	void update();
	void draw();
	int getCollision(openCVcam * camera, int mx, int my) const;
	void sendIndex(int index);
	bool start();
	void leave();
	bool gotShips = false;
	bool getShips();
	void initShips();
};

