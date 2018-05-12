#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include <stdio.h>
#include <vector>
#include <stdlib.h>
// #include <time.h>
// #include "fssimplewindow.h"
#include <math.h>
// #include "ysglfontdata.h"
#include <curl/curl.h>
#include <string>
// #include <chrono>
// #include <thread>
#include <cstdlib>

class Battleship
{
private:
	int nCol,nRow;
	int pullCounter;
    bool needToPush;
    std::string domain;
	
public:
	std::vector<int> board;
	std::vector<int> ships1;
	std::vector<int> ships2;
	int turnStatus,winStatus;
	int player;

public:
	Battleship();

private:
	void Push();
	void Pull();
	bool IsFull();
	int CheckWin();
	void CheckWinStatus();

	bool makeShip(int size, int x, int y, int rot);
	bool Attack(int row, int col);
	
public:
	int Join();
	void Leave();
	void Update();
	void Move3d(int idx);
	
	void MakeShips();
};

#endif