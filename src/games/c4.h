#ifndef CONNECT4_H
#define CONNECT4_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <curl/curl.h>

// player=-1/1, spectator=0
// board entries, turn same
// this makes checking the win condition easier

class Connect4
{
private:
    int nRow,nCol;
    int pullCounter;
    bool needToPush;
    std::string domain;

public:
    std::vector<int> board;
    int turnStatus,winStatus;
    int player;

public:
    Connect4();

private:
    void Push();
    void Pull();
    bool IsFull();
    int CheckWin();
    void CheckWinStatus();
    bool DropCoin(int col);

public:
    int Join();
    void Leave();
    void Update();
    void Move(int idx);

};

#endif
