#include "battleship.h"


size_t CurlHelper3(void *ptr, size_t size, size_t nmemb, std::string &stream)
{
    size_t realsize = size * nmemb;
    std::string temp(static_cast<const char*>(ptr), realsize);
    stream.append(temp);
    return realsize;
}
std::string CurlRequest3(std::string url)
{
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    std::string response;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.data());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlHelper3);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return response;
}


Battleship::Battleship()
{
    // domain="http://localhost/augreal/battleship/";
    domain="http://fslazyar.com/battleship/";

    nRow=10;
    nCol=10;
    player=3;
    pullCounter=1;
    turnStatus=0;
    winStatus=0;
    needToPush=false;
    
    for(int i=0;i<51;++i)
    {
        ships1.push_back(0);
        ships2.push_back(0); 
    }
    for(int i=0;i<(nCol*nRow);++i)
    {
        board.push_back(0); 
    }
}

void Battleship::Push()
{
    if(player>2)
    {
        return;
    }

    std::string url=domain;
    url.append("push.php?board=");
    for(auto x : ships1)
    {
        url.append(std::to_string(x));
    }
    for(auto x : ships2)
    {
        url.append(std::to_string(x));
    }
    CurlRequest3(url);
}
void Battleship::Pull()
{
    std::string url=domain;
    url.append("pull.php");
    std::string response=CurlRequest3(url);

    char buf[2];

    buf[0]=response[0];
    int turn=atoi(buf);

    if(turn==player)
    {
        turnStatus=1;
    }
    else if (turn != player && player < 3){
        turnStatus = 2;
    }
    else{
        turnStatus = 0;
    }
    //printf("player,turn,turn status:%d,%d,%d\n",player,turn,turnStatus);

    for(int i=0;i<ships1.size();++i)
    {
        buf[0]=response[i+1];
        ships1[i]=atoi(buf);
    }
    for(int i=0;i<ships2.size();++i)
    {
        buf[0]=response[i+1+ships1.size()];
        ships2[i]=atoi(buf);
    }
    //printf("%s\n",response.c_str());
}
bool Battleship::IsFull()
{
    // only checks this players guesses rn
    for(auto x : board)
    {
        if(x==0)
        {
            return false;
        }
    }
    return true;
}
int Battleship::CheckWin()
{
    int win = 1;
    for(int i = 0 ; i < 17*3; i=i+3){
        if(ships2[i] != 1){
            win = 0;
        }
    }
    if(win == 1){
        return win;
    }
    win = 2;
    for(int i = 0 ; i < 17*3; i=i+3){
        if(ships1[i] != 1){
            win = 0;
        }
    }
    return win;
}
void Battleship::CheckWinStatus()
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

int Battleship::Join()
{
    std::string url=domain;
    url.append("join.php");
    std::string response=CurlRequest3(url);

    // char buf[2];
    // buf[0]=response[0];
    // player=atoi(buf);
    player=std::stoi(response);

    if(player==2)
    {
        MakeShips();
        Push(); 
    }
    printf("bs player: %d\n",player);
    Pull();
    return player; 
}
void Battleship::Leave()
{
    std::string url=domain;
    url.append("leave.php?player=");
    url.append(std::to_string(player));
    CurlRequest3(url);

    pullCounter=1;
    turnStatus=0;
    winStatus=0;
    needToPush=false;
}
void Battleship::Update()
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
void Battleship::Move3d(int idx)
{
    if(winStatus==0 && turnStatus==1)
    {  
        if(Attack(idx/10, idx%10)==true)
        {   
            needToPush=true;
        }
    }
}



bool Battleship::Attack(int it, int jt){
    if (player == 1){
        if(board[it*nCol + jt] > 0){
            return false;
        }
        else{
           board[it*nCol + jt] = 1;
        }
        for(int i = 0; i < 51; i+=3){
           if(ships2[i] == it && ships2[i+1] == jt){
                ships2[i+2] == 1;
                board[it*nCol + jt] = 2;
           }
        }
    }
    if (player == 2){
        if(board[it*nCol + jt] > 0){
            return false;
        }
        else{
           board[it*nCol + jt] = 1;
        }
        for(int i = 0; i < 51; i+=3){
           if(ships1[i] == it && ships1[i+1] == jt){
                ships1[i+2] = 1;
                board[it*nCol + jt] = 2;
           }
        }
    }
    return true;
}
bool Battleship::makeShip(int size, int x, int y, int rot){
        if(rot == 0){
            if(x > 10 - size){
                return false;
            }
            else{
                for(int i = 0; i < size; i++){
                    if(board[x*nCol + y+ i*nCol] > 0){
                        return false;
                    }
                }
                for(int i = 0; i < size; i++){
                    board[x*nCol + y+ i*nCol ] = 1;
                }
                return true;
            }
        }
        if(rot == 1){
            if(y >  10 - size){
                return false;
            }
            else{
                for(int i = 0; i < size; i++){
                    if(board[x*nCol + y+ i] > 0){
                        return false;
                    }
                }
                for(int i = 0; i < size; i++){
                    board[x*nCol + y+ i] = 1;
                }
                return true;
            }
        }
}
void Battleship::MakeShips(){
    srand (time(NULL));
    int ss[] = {5,4,3,3,2,1};
    int indexer [] = {0, 15,27,36,45};
    int counter = 0;
    int index = 0;
    while(counter < 5){
        int x = rand()%10;
        int y = rand()%10;
        int z = rand()%2;
        if(makeShip(ss[counter],x,y,z)){
            for(int i = 0; i < ss[counter]; i++){
                ships1[indexer[counter] + i*3] = x + i - z*i;
                ships1[indexer[counter] + i*3 + 1] = y + z*i;
                ships1[indexer[counter] + i*3 + 2] = 0;
            }
            counter++;
        }
    }
    for(int i = 0; i < 100; i++){
        board[i] = 0;
    }
    index = 0;
    counter = 0;
    while(counter < 5){
        int x = rand()%10;
        int y = rand()%10;
        int z = rand()%2;
        if(makeShip(ss[counter],x,y,z)){
            for(int i = 0; i < ss[counter]; i++){
                ships2[indexer[counter] + i*3] = x + i - z*i;
                ships2[indexer[counter] + i*3 + 1] = y + z*i;
                ships2[indexer[counter] + i*3 + 2] = 0;
            }
            counter++;
        }
    }
    for(int i = 0; i < 100; i++){
        board[i] = 0;
    }
    for(int i = 0; i < 51; i+=3){
        printf(" %d %d %d \n",ships1[i],ships1[i+1],ships1[i+2]);
    }
    for(int i = 0; i < 51; i+=3){
        printf(" %d %d %d \n",ships2[i],ships2[i+1],ships2[i+2]);
    }
    
    
}