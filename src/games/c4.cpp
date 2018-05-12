#include "c4.h"


size_t CurlHelper2(void *ptr, size_t size, size_t nmemb, std::string &stream)
{
    size_t realsize = size * nmemb;
    std::string temp(static_cast<const char*>(ptr), realsize);
    stream.append(temp);
    return realsize;
}
std::string CurlRequest2(std::string url)
{
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    std::string response;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.data());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlHelper2);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return response;
}


// player=-1/1, spectator=0
// board entries, turn same
// this makes checking the win condition easier

Connect4::Connect4()
{
    // domain="http://localhost/augreal/connect4/";
    domain="http://fslazyar.com/connect4/";

    nRow=6;
    nCol=7;
    player=0;
    pullCounter=1;
    turnStatus=0;
    winStatus=0;
    needToPush=false;

    for(int i=0;i<(nCol*nRow);++i)
    {
        board.push_back(0); 
    }
}

void Connect4::Push()
{
    if(player==0)
    {
        return;
    }

    std::string url=domain;
    url.append("push.php?board=");
    for(auto x : board)
    {
        url.append(std::to_string(x+1));
    }
    CurlRequest2(url);
}
void Connect4::Pull()
{
    std::string url=domain;
    url.append("pull.php");
    std::string response=CurlRequest2(url);

    char buf[2];

    buf[0]=response[0];
    int turn=atoi(buf);
    if(turn==2)
    {
        turn=-1;
    }

    if(turn==player)
    {
        turnStatus=1;
    }
    else if(player!=0)
    {
        turnStatus=2;
    }

    for(int i=0;i<board.size();++i)
    {
        buf[0]=response[i+1];
        board[i]=atoi(buf)-1;
    }
}

bool Connect4::IsFull()
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
int Connect4::CheckWin()
{
    int sum;
    
    for(int row=0;row<nRow;row++)
    {
        for(int col=0;col<nCol;col++)
        {            
            // Check right
            if(col+3<nCol)
            {
                sum=0;
                for(int i=col;i<=col+3;++i)
                {
                    sum+=board[row*nCol+i];
                }
                if(sum==4) return 1;
                if(sum==-4) return -1;
            }
            
            // Check down
            if(row+3<nRow)
            {
                sum=0;
                for(int j=row;j<=row+3;++j)
                {
                    sum+=board[j*nCol+col];
                }
                if(sum==4) return 1;
                if(sum==-4) return -1;                
            }
            
            // Check diag up right
            if(col+3<nCol && row-3>=0)
            {
                sum=0;
                int i=col+3;
                for(int j=row-3;j<=row;++j)
                {
                    sum+=board[j*nCol+i];
                    --i;
                }
                if(sum==4) return 1;
                if(sum==-4) return -1;
            }

            // Check diag down right
            if(col+3<nCol && row+3<nRow)
            {
                sum=0;
                int i = col;
                for(int j=row;j<=row+3;++j)
                {
                    sum+=board[j*nCol+i];
                    ++i;
                }
                if(sum==4) return 1;
                if(sum==-4) return -1;
            } 
        }
    }
    
    return 0; 
}
void Connect4::CheckWinStatus()
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
        else if(player!=0 && win!=0)
        {
            winStatus=2;
        }
    }
}
bool Connect4::DropCoin(int col)
{
    for(int row=nRow-1;row>=0;--row)
    {
        int idx=row*nCol+col;
        if(board[idx]==0)
        {
            board[idx]=player;
            return true;
        }
    }
    return false;
}

int Connect4::Join()
{
    std::string url=domain;
    url.append("join.php");
    std::string response=CurlRequest2(url);

    printf("c4 join response:%s\n", response.c_str());

    char buf[2];
    buf[0]=response[0];
    player=atoi(buf);
    if(player==2)
    {
        player=-1;
    }

    Pull();
    printf("%d\n",player);
    return player;
}
void Connect4::Leave()
{
    std::string url=domain;
    url.append("leave.php?player=");
    url.append(std::to_string(player+1));
    CurlRequest2(url);

    pullCounter=1;
    turnStatus=0;
    winStatus=0;
    needToPush=false;
}
void Connect4::Update()
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
void Connect4::Move(int idx)
{
    int col=idx%nCol;
    if(winStatus==0 && turnStatus==1)
    {
        if(DropCoin(col)==true)
        {
            needToPush=true;
        }
    }
}