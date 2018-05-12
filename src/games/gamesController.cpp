#include <stdio.h>
#include <vector>
#include <ysclass.h>
#include <fslazywindow.h>
#include <binstl.h>
#include <iostream>
#include <stdlib.h>
#include <ctype.h>
#include <thread>
#include <stdlib.h>
#include <objectStl.h>
#include "cvStuff.h"
#include <curl/curl.h>
#include <string>
#include <ysclass.h>
#include <fslazywindow.h>
#include <glutil.h>
#include "gamesController.h"

using namespace std;

size_t CurlHelper0(void *ptr, size_t size, size_t nmemb, std::string &stream)
{
    size_t realsize = size * nmemb;
    string temp(static_cast<const char*>(ptr), realsize);
    stream.append(temp);
    return realsize;
}
string CurlRequest0(string url)
{
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    string response;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.data());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlHelper0);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return response;
}
void HardReset()
{
    string url1 = "http://fslazyar.com/connect4/reset.php";
    string url2 = "http://fslazyar.com/ttt/reset.php";
    string url3 = "http://fslazyar.com/battleship/reset.php";
    // string url1 = "http://localhost/augreal/connect4/reset.php";
    // string url2 = "http://localhost/augreal/ttt/reset.php";
    CurlRequest0(url1);
    CurlRequest0(url2);
    CurlRequest0(url3);
    printf("reset");
    return;
}

void connect4board::start(){
	c4.Join();
	board.changeState(1);
}
void connect4board::leave(){
	c4.Leave();
	board.changeState(0);
	for(int i = 0; i < 42; i++){
		tokens[i].changeState(0);
	}
}

int connect4board::getCollision(openCVcam *camera, int mx,int my) const{
	int wid,hei;
	FsGetWindowSize(wid,hei);
	auto vp=WindowToViewPort(wid,hei,mx,my);
	YsMatrix4x4 perspective;
	perspective.Create((double*)camera->oglCam.data);
	perspective.Transpose();

	YsMatrix4x4 modelView;
	modelView.Create((double*)camera->viewMatrix.data);
	modelView.Transpose();
	YsMatrix4x4 all=perspective*modelView;
	int counter = 0;
	int index = -1;
	for(int i = 0; i < 42; i++){
		double x = tokens[i].x;
		double y = tokens[i].y;
		double z = tokens[i].z;
		YsVec3 temp = YsVec3(x,y,z);
		temp = all*temp;
		auto winPos=ViewPortToWindow(wid,hei,temp);
		int dx=(mx-winPos.x()),dy=(my-winPos.y());
		if(-50<=dx && dx<=50 && -50<=dy && dy<=50)
		{
			index = counter;
			return index;
		}
		counter++;
	
	}
	return index;
}


// void connect4board::sendCol(int column){
// 	c4.Move3d(column);
// }
void connect4board::sendIndex(int index){
	c4.Move(index);
}

void connect4board::init(stlObject * big, stlObject * small){
	double scale = big->scale;

	for(int i = 0; i < 42; i++){
		tokens.push_back(Object());
		tokens[i].init(small);
	}
	for(int i = 0; i < 6; i ++){
		for (int j = 0; j < 7; j++){
			tokens[i*7+j].moveTo(0,(j-3)*scale*414/3, 375*scale + 750*scale + scale*-1*i*700/5);
			tokens[i*7+j].rotateTo(0,0,YsPi/2);
			tokens[i*7+j].changeState(0);
			tokens[i*7+j].chooseColor(0);
		}
	}
	board.init(big);
	board.rotateTo(YsPi/2,YsPi/2,0);
	board.changeState(0);
	board.chooseColor(2);
	board.moveTo(0,0,scale*600);
}

void connect4board::update(){
	c4.Update();
	vector<int> board = c4.board;
	for(int i = 0; i < 42; i++){
		tokens[i].changeState(board[i]);
		tokens[i].chooseColor(board[i]);
	}
}

void connect4board::DrawFloor(){
	glColor3f(.1,.1,.1);
	glNormal3f(0,0,1);
	glBegin(GL_QUADS);
	glVertex3f(-15,-30,0);
	glVertex3f(-15,30,0);
	glVertex3f(15,30,0);
	glVertex3f(15,-30,0);
	glEnd();
}
void connect4board::draw(){
	for(int i = 0; i < 42; i++){
		if(tokens[i].state != 0){
			tokens[i].Draw();
		}
	}

	board.Draw();
	DrawFloor();
}


// ------ TIC TAC TOE --------------------------------
void ttt_board::init(stlObject* big, stlObject* x_token, stlObject* o_token){
	double scale = big->scale;

	for(int i = 0; i < 9; i++){
		x_tokens.push_back(Object());
		x_tokens[i].init(x_token);
	}
	for(int i = 0; i < 9; i++){
		o_tokens.push_back(Object());
		o_tokens[i].init(o_token);
	}
	double movement = 310;
	for(int i = 0; i < 3; i ++){
		for (int j = 0; j < 3; j++){
			x_tokens[i*3+j].moveTo((i-1)*scale*movement,(j-1)*scale*movement, 25*scale);
			x_tokens[i*3+j].rotateTo(0,0,0);
			x_tokens[i*3+j].changeState(0);
			x_tokens[i*3+j].chooseColor(0);

			o_tokens[i*3+j].moveTo((i-1)*scale*movement,(j-1)*scale*movement, 25*scale);
			o_tokens[i*3+j].rotateTo(0,0,0);
			o_tokens[i*3+j].changeState(0);
			o_tokens[i*3+j].chooseColor(0);
		}
	}
	board.init(big);
	board.rotateTo(0,0,0);
	board.changeState(0);
	board.chooseColor(2);
	board.moveTo(0,0,scale*25);
}
void ttt_board::start(){
	ttt.Join();
	board.changeState(1);
}
void ttt_board::leave(){
	ttt.Leave();
	board.changeState(0);
	for(int i = 0; i < 9; i++){
		x_tokens[i].changeState(0);
		o_tokens[i].changeState(0);
	}
}
int ttt_board::getCollision(openCVcam *camera, int mx,int my) const{
	int wid,hei;
	FsGetWindowSize(wid,hei);
	auto vp=WindowToViewPort(wid,hei,mx,my);
	YsMatrix4x4 perspective;
	perspective.Create((double*)camera->oglCam.data);
	perspective.Transpose();

	YsMatrix4x4 modelView;
	modelView.Create((double*)camera->viewMatrix.data);
	modelView.Transpose();
	YsMatrix4x4 all=perspective*modelView;
	int counter = 0;
	int index = -1;
	for(int i = 0; i < 9; i++){
		double x = x_tokens[i].x;
		double y = x_tokens[i].y;
		double z = x_tokens[i].z;
		YsVec3 temp = YsVec3(x,y,z);
		temp = all*temp;
		auto winPos=ViewPortToWindow(wid,hei,temp);
		int dx=(mx-winPos.x()),dy=(my-winPos.y());
		if(-50<=dx && dx<=50 && -50<=dy && dy<=50)
		{
			index = counter;
			printf("%d\n",index);
			return index;
		}
		counter++;
	
	}
	return index;
}
void ttt_board::sendIndex(int index){
	ttt.Move(index);
}
void ttt_board::update(){
	ttt.Update();
	vector<int> board = ttt.board;
	for(int i = 0; i < 9; i++){
		if(board[i]==1)
		{
			x_tokens[i].changeState(1);			
			o_tokens[i].changeState(0);
		}
		else if(board[i]==2)
		{
			x_tokens[i].changeState(0);
			o_tokens[i].changeState(1);
		}
		else
		{
			x_tokens[i].changeState(0);
			o_tokens[i].changeState(0);
			
		}
		x_tokens[i].chooseColor(0);
		o_tokens[i].chooseColor(0);
	}
}
void ttt_board::draw(){
	for(int i = 0; i < 9; i++){
		if(x_tokens[i].state != 0){
			x_tokens[i].Draw();
		}
		if(o_tokens[i].state != 0){
			o_tokens[i].Draw();
		}
	}
	board.Draw();
}






void battleshipBoard::init(stlObject * big, stlObject * peg,stlObject * s2,stlObject * s3,stlObject * s4,stlObject * s5)
{	double scale = big->scale;
	printf("player: %d\n",player);
	if(player == 1){
		for(int i = 0; i < 10; i++){
			for(int j = 0; j < 10; j++){
				pegs.push_back(Object());
				pegs[i*10 + j].init(peg);
				pegs[i*10 + j].rotateTo(0,0,0);
				pegs[i*10 + j].changeState(0);
				pegs[i*10 + j].chooseColor(0);
				pegs[i*10 + j].moveTo(425*scale - j*93*scale,550*scale,i*93*scale + 140*scale);
				//pegs[i*10 + j].moveTo(1000*scale + 1000,22.5*scale + 1000*(-1*j/10)*scale,525*scale + 1000*(-1*i/10)*scale);
			}
		}
	}
	else if(player == 2){
		for(int i = 0; i < 10; i++){
			for(int j = 0; j < 10; j++){
				pegs.push_back(Object());
				pegs[i*10 + j].init(peg);
				pegs[i*10 + j].rotateTo(0,0,1*YsPi);
				pegs[i*10 + j].changeState(0);
				pegs[i*10 + j].chooseColor(0);

				pegs[i*10 + j].moveTo(-425*scale + j*93*scale,-550*scale,i*93*scale + 140*scale);
				//pegs[i*10 + j].moveTo(1000*scale + 1000,22.5*scale + 1000*(-1*j/10)*scale,525*scale + 1000*(-1*i/10)*scale);
			}
		}
	}
	else{
		for(int i = 0; i < 10; i++){
			for(int j = 0; j < 10; j++){
				pegs.push_back(Object());
				pegs[i*10 + j].init(peg);
				pegs[i*10 + j].rotateTo(0,0,1*YsPi);
				pegs[i*10 + j].changeState(0);
				pegs[i*10 + j].chooseColor(0);
				pegs[i*10 + j].moveTo(-425*scale + j*93*scale,-550*scale,i*93*scale + 140*scale);
				//pegs[i*10 + j].moveTo(1000*scale + 1000,22.5*scale + 1000*(-1*j/10)*scale,525*scale + 1000*(-1*i/10)*scale);
			}
		}

	}
	for(int i = 0; i < 5; i++){
		ships1.push_back(Object());
		ships2.push_back(Object());
	}

	for(int i = 0; i < 17; i++){
		shipPegs1.push_back(Object());
		shipPegs2.push_back(Object());
		shipPegs1[i].init(peg);
		shipPegs2[i].init(peg);
	}
	ships1[0].init(s5);
	ships1[1].init(s4);
	ships1[2].init(s3);
	ships1[3].init(s3);
	ships1[4].init(s2);

	ships2[0].init(s5);
	ships2[1].init(s4);
	ships2[2].init(s3);
	ships2[3].init(s3);
	ships2[4].init(s2);


	board1.init(big);
	board1.rotateTo(0,0,0);
	board1.changeState(1);
	board1.chooseColor(0);
	board1.moveTo(0,1000*scale,525*scale);
	board2.init(big);
	board2.rotateTo(0,0,YsPi);
	board2.changeState(1);
	board2.chooseColor(0);
	board2.moveTo(0,-1000*scale,525*scale);
}



void battleshipBoard::initShips(){

	int indexers [] = {0,15,27,36,45};
	double sizer [] = {195,145,5,5,10};
	int index = 0;
	for(int i = 0; i < 5; i++){
		index = indexers[i];
		if(bs.ships1[index] == bs.ships1[index+3]){
			ships1[i].moveTo(425*scale + -1*bs.ships1[index+1]*93*scale - sizer[i]*scale, 1430*scale - bs.ships1[index]*90*scale,70*scale);
			ships1[i].rotateTo(-1*YsPi/2,0,0);
		}
		else{
			ships1[i].moveTo(425*scale + -1*bs.ships1[index+1]*93*scale, 1430*scale - bs.ships1[index]*90*scale - sizer[i]*scale ,70*scale);
			ships1[i].rotateTo(0,-YsPi/2,YsPi/2);
		}
		ships1[i].chooseColor(2);
		ships1[i].changeState(1);
	}

	int indexers2 [] = {0,15,27,36,45};
	double sizer2 [] = {180,145,190,190,100};
	for(int i = 0; i < 5; i++){
		index = indexers2[i];
		if(bs.ships2[index] == bs.ships2[index+3]){
			ships2[i].moveTo(-425*scale + 1*bs.ships2[index+1]*93*scale + sizer2[i]*scale, -1430*scale + bs.ships2[index]*90*scale,70*scale);
			ships2[i].rotateTo(-1*YsPi/2,0,0);
		}
		else{
			ships2[i].moveTo(-425*scale + 1*bs.ships2[index+1]*93*scale, -1430*scale + bs.ships2[index]*90*scale + sizer2[i]*scale ,70*scale);
			ships2[i].rotateTo(0,-YsPi/2,YsPi/2);
		}
		ships2[i].chooseColor(2);
		ships2[i].changeState(1);
	}



	for(int i = 0; i < 51; i=i+3){
		shipPegs1[i/3].moveTo(425*scale + -1*bs.ships1[i+1]*93*scale,1430*scale - bs.ships1[i]*90*scale,120*scale);
		shipPegs1[i/3].rotateTo(YsPi/2,0,0);
		if(bs.ships1[i+2] == 0){
			shipPegs1[i/3].chooseColor(1);
			shipPegs1[i/3].changeState(0);
		}
		else{
		shipPegs1[i/3].chooseColor(1);
		shipPegs1[i/3].changeState(0);
		}
	}
	for(int i = 0; i < 51; i=i+3){
		shipPegs2[i/3].moveTo(-425*scale + 1*bs.ships2[i+1]*93*scale,-1430*scale + bs.ships2[i]*90*scale,120*scale);
		shipPegs2[i/3].rotateTo(YsPi/2,0,0);
		if(bs.ships2[i+2] == 0){
			shipPegs2[i/3].chooseColor(1);
			shipPegs2[i/3].changeState(0);
		}
		else{
		shipPegs2[i/3].chooseColor(1);
		shipPegs2[i/3].changeState(0);
		}
	}
	
	//double scale = board1.
}


bool battleshipBoard::start(){
	player = bs.Join();
	if(player == 1){
		joinedFirst = true;
	}
	return true;
}
void battleshipBoard::leave(){
	gotShips = false;
	bs.Leave();
	
}

void battleshipBoard::draw(){
	if(gotShips){
		for(int i = 0; i < 100; i++){
			if(pegs[i].state != 0){
				pegs[i].Draw();
			}
		}
		for(int i = 0; i < 17; i++){
			if(shipPegs1[i].state != 0){
				shipPegs1[i].Draw();
			}
		}
		for(int i = 0; i < 17; i++){
			if(shipPegs2[i].state != 0){
				shipPegs2[i].Draw();
			}
		}
		for(int i = 0; i < 5; i++){
			if(ships1[i].state != 0){
				ships1[i].Draw();
			}
		}
		for(int i = 0; i < 5; i++){
			if(ships2[i].state != 0){
				ships2[i].Draw();
			}
		}

	}
	board1.Draw();
	board2.Draw();
}

int battleshipBoard::getCollision(openCVcam *camera, int mx,int my) const{
	int wid,hei;
	FsGetWindowSize(wid,hei);
	auto vp=WindowToViewPort(wid,hei,mx,my);
	YsMatrix4x4 perspective;
	perspective.Create((double*)camera->oglCam.data);
	perspective.Transpose();

	YsMatrix4x4 modelView;
	modelView.Create((double*)camera->viewMatrix.data);
	modelView.Transpose();
	YsMatrix4x4 all=perspective*modelView;
	int counter = 0;
	int index = -1;
	for(int i = 0; i < 100; i++){
		double x = pegs[i].x;
		double y = pegs[i].y;
		double z = pegs[i].z;
		YsVec3 temp = YsVec3(x,y,z);
		temp = all*temp;
		auto winPos=ViewPortToWindow(wid,hei,temp);
		int dx=(mx-winPos.x()),dy=(my-winPos.y());
		if(-25<=dx && dx<=25 && -25<=dy && dy<=25)
		{
			index = counter;
			printf("%d\n",index);
			return index;
		}
		counter++;
	
	}
	return index;
}


void battleshipBoard::update(){
	bs.Update();
	vector<int> board = bs.board;
	for(int i = 0; i < 100; i++){
		if(board[i]==1)
		{
			pegs[i].changeState(1);
			pegs[i].chooseColor(2);
		}
		else if(board[i]==2)
		{
			pegs[i].changeState(1);
			pegs[i].chooseColor(1);
		}
		else
		{
			pegs[i].changeState(0);
			pegs[i].chooseColor(0);
			
		}
	}
	for(int i = 0; i < 51; i+=3){
		if(bs.ships1[i+2] == 0){
			shipPegs1[i/3].changeState(0);
			shipPegs1[i/3].chooseColor(0);
		}
		else if(bs.ships1[i+2] == 1){
			shipPegs1[i/3].changeState(1);
			shipPegs1[i/3].chooseColor(1);
		}
	}
	for(int i = 0; i < 51; i+=3){
		if(bs.ships2[i+2] == 0){
			shipPegs2[i/3].changeState(0);
			shipPegs2[i/3].chooseColor(0);
		}
		else if(bs.ships1[i+2] == 1){
			shipPegs2[i/3].changeState(1);
			shipPegs2[i/3].chooseColor(1);
		}
	}

}

bool battleshipBoard::getShips(){
	bs.Update();
	for(int i = 0; i < 51; i++){
		if(bs.ships1[i] != 0){
			gotShips = true;
			initShips();
			return true;
		}
	}
	return false;
}

void battleshipBoard::sendIndex(int index){
	bs.Move3d(index);
}