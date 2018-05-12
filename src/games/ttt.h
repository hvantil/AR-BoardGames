#ifndef TTT_H
#define TTT_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <curl/curl.h>

class TicTacToe
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
    TicTacToe();

private:
    void Push();
    void Pull();
    bool IsFull();
    int CheckWin();
    void CheckWinStatus();

public:
    void Join();
    void Leave();
    void Update();
    void Move(int idx);
};

#endif

