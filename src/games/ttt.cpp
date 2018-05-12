#include "ttt.h"


size_t CurlHelper1(void *ptr, size_t size, size_t nmemb, std::string &stream)
{
    size_t realsize = size * nmemb;
    std::string temp(static_cast<const char*>(ptr), realsize);
    stream.append(temp);
    return realsize;
}
std::string CurlRequest1(std::string url)
{
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    std::string response;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.data());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlHelper1);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return response;
}


TicTacToe::TicTacToe()
{
    // domain="http://localhost/augreal/ttt/";
    domain="http://fslazyar.com/ttt/";

    nRow=3;
    nCol=3;
    player=3;
    pullCounter=1;
    turnStatus=0;
    winStatus=0;
    needToPush=false;
    for(int i=0;i<(nCol*nRow);++i)
    {
        board.push_back(0); 
    }
}

void TicTacToe::Push()
{
    if(player>2)
    {
        return;
    }

    std::string url=domain;
    url.append("push.php?board=");
    for(auto x : board)
    {
        url.append(std::to_string(x));
    }
    CurlRequest1(url);
}
void TicTacToe::Pull()
{
    std::string url=domain;
    url.append("pull.php");
    std::string response=CurlRequest1(url);

    char buf[2];

    buf[0]=response[0];
    int turn=atoi(buf);
    if(turn==player)
    {
        turnStatus=1;
    }
    else if(player!=3)
    {
        turnStatus=2;
    }

    for(int i=0;i<board.size();++i)
    {
        buf[0]=response[i+1];
        board[i]=atoi(buf);
    }
}
bool TicTacToe::IsFull()
{
    for(auto x : board)
    {
        if(x==0)
        {
            return false;
        }
    }
    return true;   
}
int TicTacToe::CheckWin()
{
    for(int p=1;p<=2;++p)
    {
        if((board[0]==p && board[1]==p && board[2]==p) ||
           (board[3]==p && board[4]==p && board[5]==p) ||
           (board[6]==p && board[7]==p && board[8]==p) ||
           (board[0]==p && board[3]==p && board[6]==p) ||
           (board[1]==p && board[4]==p && board[7]==p) ||
           (board[2]==p && board[5]==p && board[8]==p) ||
           (board[0]==p && board[4]==p && board[8]==p) ||
           (board[2]==p && board[4]==p && board[6]==p))
        {
            return p;
        }
    }
    return 0;
}
void TicTacToe::CheckWinStatus()
{
    if(IsFull()==true)
    {
        winStatus=3;
    }
    else
    {
        int win=CheckWin();
        if(win==player)
        {
            winStatus=1;
        }
        else if(player<2 && win>0)
        {
            winStatus=2;
        }
    }
}

void TicTacToe::Join()
{
    std::string url=domain;
    url.append("join.php");
    std::string response=CurlRequest1(url);

    char buf[2];
    buf[0]=response[0];
    player=atoi(buf);

    Pull();
}
void TicTacToe::Leave()
{
    std::string url=domain;
    url.append("leave.php?player=");
    url.append(std::to_string(player));
    CurlRequest1(url);

    pullCounter=1;
    turnStatus=0;
    winStatus=0;
    needToPush=false;
}
void TicTacToe::Update()
{
    if(needToPush==true)
    {
        Push();
        Pull();
        needToPush=false;
        pullCounter=1;
    }

    if(pullCounter%20==0)
    {
        Pull();
        pullCounter=0;
    }
    ++pullCounter;

    if(winStatus==0)
    {
        CheckWinStatus();
    }
}
void TicTacToe::Move(int idx)
{
    if(winStatus==0 && turnStatus==1 && board[idx]==0)
    {
        board[idx]=player;
        needToPush=true;
    }
}
