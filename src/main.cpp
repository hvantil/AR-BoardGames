/* ////////////////////////////////////////////////////////////

File Name: main.cpp
Copyright (c) 2017 Soji Yamakawa.  All rights reserved.
http://www.ysflight.com

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS 
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//////////////////////////////////////////////////////////// */

#include <stdio.h>
#include <vector>
#include <ysclass.h>
#include <fslazywindow.h>
#include "binstl.h"
#include <iostream>
#include <stdlib.h>
#include <ctype.h>
#include <thread>
#include <stdlib.h>
#include <objectStl.h>
#include <cvStuff.h>
#include <string>


#include <ysclass.h>
#include <fslazywindow.h>

#include <gamesController.h>
#include <ysglfontdata.h>


using namespace std;



class FsLazyWindowApplication : public FsLazyWindowApplicationBase
{
protected:
	int fwid = 1280;
	int fhei = 720;
	int newstate = 0;
	bool needRedraw;
	int counter = 0;
	GLuint imageTexture;
	vector<stlObject> stlObjectsConnect4;
	vector<stlObject> stlObjectsTicTacToe;
	vector<stlObject> stlObjectsBS;
	vector<Object> someObjectsC;
	vector<Object> someObjectsT;
	vector<Object> someObjectsB;
    openCVcam camera;
    connect4board game1;
    ttt_board game2;
    battleshipBoard game3;
    int mode = 0;
    bool onlyimage = false;
    vector<int> button1;
    vector<int> button2;
    vector<int> button3;
    vector<int> button4;
    vector<int> button5;
    string message = "Welcome!";

public:
	FsLazyWindowApplication();
	virtual void BeforeEverything(int argc,char *argv[]);
	virtual void GetOpenWindowOption(FsOpenWindowOption &OPT) const;
	virtual void Initialize(int argc,char *argv[]);
	virtual void Interval(void);
	virtual void BeforeTerminate(void);
	virtual void Draw(void);
	virtual bool UserWantToCloseProgram(void);
	virtual bool MustTerminate(void) const;
	virtual long long int GetMinimumSleepPerInterval(void) const;
	virtual bool NeedRedraw(void) const;

};




FsLazyWindowApplication::FsLazyWindowApplication()
{
	needRedraw=false;

}

/* virtual */ void FsLazyWindowApplication::BeforeEverything(int argc,char *argv[])
{	FsChangeToProgramDir();
	if(!camera.readInCameraParam(argc,argv,onlyimage)){
		SetMustTerminate(true);
	}
	camera.Setup();
   	camera.camerainit();
   	//camera.interval(imageTexture);	
}
/* virtual */ void FsLazyWindowApplication::GetOpenWindowOption(FsOpenWindowOption &opt) const
{
	opt.x0=0;
	opt.y0=0;
	opt.wid=1280;
	opt.hei=720;
}

void makeButtons(int wid, int hei, vector<int> &inbut1,vector<int>& inbut2,vector<int>& inbut3,vector<int> &inbut4, vector<int> &inbut5){
	inbut1.clear();
	inbut2.clear();
	inbut3.clear();
	inbut4.clear();
	inbut1.push_back(10);
	inbut1.push_back(hei - 10);
	inbut1.push_back(200);
	inbut1.push_back(hei - 90);

	inbut2.push_back(10);
	inbut2.push_back(hei - 10 - 100);
	inbut2.push_back(200);
	inbut2.push_back(hei-90 - 100);

	inbut3.push_back(10);
	inbut3.push_back(hei - 10 - 200);
	inbut3.push_back(200);
	inbut3.push_back(hei-90 - 200);

	inbut4.push_back(10);
	inbut4.push_back(hei - 10 - 300);
	inbut4.push_back(200);
	inbut4.push_back(hei-90 - 300);

	inbut5.push_back(wid-300);
	inbut5.push_back(hei - 10);
	inbut5.push_back(wid-10);
	inbut5.push_back(hei-90 );
}
/* virtual */ void FsLazyWindowApplication::Initialize(int argc,char *argv[])
{	int wid,hei;
	// 1. Clear Screen
	FsGetWindowSize(wid,hei);
	fwid = wid;
	fhei = hei;
	makeButtons(wid,hei,button1,button2,button3,button4,button5);

 	double oscale = .04;
	stlObjectsConnect4.push_back(stlObject());
	stlObjectsConnect4.push_back(stlObject());
	stlObjectsTicTacToe.push_back(stlObject());
	stlObjectsTicTacToe.push_back(stlObject());
	stlObjectsTicTacToe.push_back(stlObject());
	stlObjectsBS.push_back(stlObject());
	stlObjectsBS.push_back(stlObject());
	stlObjectsBS.push_back(stlObject());
	stlObjectsBS.push_back(stlObject());
	stlObjectsBS.push_back(stlObject());
	stlObjectsBS.push_back(stlObject());
	char const * s1 = "c4_piece.STL";
	stlObjectsConnect4[0].init(s1,oscale);
	stlObjectsConnect4[0].translate(-57.5,-12.5,-57.5);
	char const * s2 = "c4_board.STL";
	stlObjectsConnect4[1].init(s2,oscale,1,1,0);
	stlObjectsConnect4[1].translate(-540,-600,-116.5);
	game1.init(&stlObjectsConnect4[1],&stlObjectsConnect4[0]);
	

	oscale = .05;
	stlObjectsTicTacToe[0].init("ttt_board.STL",oscale,1,1,0);
	stlObjectsTicTacToe[0].translate(-500,-500,-25);
	stlObjectsTicTacToe[1].init("ttt_x_piece.STL",oscale);
	stlObjectsTicTacToe[1].translate(-112.5,-112.5,-25);
	stlObjectsTicTacToe[2].init("ttt_o_piece.STL",oscale);
	stlObjectsTicTacToe[2].translate(-112.5,-112.5,-25);
	game2.init(&stlObjectsTicTacToe[0],&stlObjectsTicTacToe[1],&stlObjectsTicTacToe[2]);


	oscale = .03;
	game3.scale = .03;
	stlObjectsBS[0].init("battleship_board.STL",oscale);
	stlObjectsBS[0].translate(-500,-525,-525);
	stlObjectsBS[1].init("battleship_peg.STL",oscale);
	stlObjectsBS[1].translate(-22,-45,-22.5);
	stlObjectsBS[2].init("battleship_ship2.STL",oscale,1,1,0);
	stlObjectsBS[2].translate(-125,-35,-35);
	stlObjectsBS[3].init("battleship_ship3.STL",oscale,1,1,0);
	stlObjectsBS[3].translate(-220,-35,-35);
	stlObjectsBS[4].init("battleship_ship4.STL",oscale,1,1,0);
	stlObjectsBS[4].translate(-166,-35,-35);
	stlObjectsBS[5].init("battleship_ship5.STL",oscale,1,1,0);
	stlObjectsBS[5].translate(-211,-35,-35);
	
	
}


void drawButton(vector<int> inbut){
	glColor3f(1,1,1);
	glBegin(GL_QUADS);
	glVertex2d(inbut[0]-10, inbut[1]+10);
	glVertex2d(inbut[2]+10, inbut[1]+10);
	glVertex2d(inbut[2]+10, inbut[3]-10);
	glVertex2d(inbut[0]-10, inbut[3]-10);
	glEnd();
	glColor3f(0.0,0,0);
	glBegin(GL_QUADS);
	glVertex2d(inbut[0], inbut[1]);
	glVertex2d(inbut[2], inbut[1]);
	glVertex2d(inbut[2], inbut[3]);
	glVertex2d(inbut[0], inbut[3]);
	glEnd();
}
void drawButtons(int mode,vector<int> inbut1,vector<int> inbut2,vector<int> inbut3,vector<int> inbut4,vector<int> inbut5){
	if(mode == 0){
		drawButton(inbut1);
		drawButton(inbut2);
		drawButton(inbut3);
		drawButton(inbut4);
		drawButton(inbut5);
	}

	else{
		drawButton(inbut1);
		drawButton(inbut5);
	}
}
void drawText(int mode, int hei,vector<int> inbut1,vector<int> inbut2,vector<int> inbut3,vector<int> inbut4,vector<int> inbut5){
	if(mode == 0){
		glColor3d(1.0,0.0,0.0);
		glBegin(GL_QUADS);
		glEnd();
		glRasterPos2d(inbut1[0] + 10,(inbut1[1] - 50 ));
		YsGlDrawFontBitmap12x16("Connect4");
		glRasterPos2d(inbut2[0] + 10, (inbut2[1]) - 50);
		YsGlDrawFontBitmap12x16("Tic Tac Toe");
		glRasterPos2d(inbut3[0] + 10,  (inbut3[1]) - 50);
		YsGlDrawFontBitmap12x16("BattleShip");
		glRasterPos2d(inbut3[0] + 10,  (inbut4[1]) - 50);
		YsGlDrawFontBitmap12x16("Reset Games");
		glEnd();
	}
	if(mode > 0){
		glColor3f(1,0,0);
		glRasterPos2d(inbut1[0] + 10, hei - ((inbut1[1] - inbut1[3])/2 + 20));
		YsGlDrawFontBitmap12x16("Leave");
		glEnd();
	}
}

void drawMessage(int hei, int wid, vector<int>inbut5,const char * str){
	glColor3d(1.0,0.0,0.0);
	glBegin(GL_QUADS);
	glEnd();
	glRasterPos2d(inbut5[0] + 10,(inbut5[1] - 50 ));
	YsGlDrawFontBitmap12x16(str);
	glEnd();
}

int checkWhichButton(int mx, int my,vector<int> inbut1,vector<int> inbut2,vector<int> inbut3, vector<int>inbut4){

	if(mx < inbut1[2] && mx>inbut1[0] && my < inbut1[1] && my > inbut1[3]){
		return 1;
	}
	if(mx < inbut2[2] && mx>inbut2[0] && my < inbut2[1] && my > inbut2[3]){
		return 2;
	}
	if(mx < inbut3[2] && mx>inbut3[0] && my < inbut3[1] && my > inbut3[3]){
		return 3;
	}
	if(mx < inbut4[2] && mx>inbut4[0] && my < inbut4[1] && my > inbut4[3]){
		return 4;
	}
	return 0;
}
/* virtual */ void FsLazyWindowApplication::Interval(void)
{	makeButtons(fwid,fhei,button1,button2,button3,button4,button5);
	int mx,my,lb,mb,rb;
	int evt=FsGetMouseEvent(lb,mb,rb,mx,my);
		if(camera.interval(imageTexture)){
		//	printf("wopp");
		}
	auto key=FsInkey();
	if(FSKEY_ESC==key)
	{
		SetMustTerminate(true);
	}

	if(evt==FSMOUSEEVENT_LBUTTONDOWN)
	{	int button = 0;
		button = checkWhichButton(mx,fhei - my,button1,button2,button3,button4);
		if(mode == 0){
			//printf("%d\n",button);
			if(button == 1){
				game1.start();
				mode = 1;
			}
			else if(button == 2){
				game2.start();
				mode = 2;
			}
			if(button == 3){
				if(game3.start()){
					mode = 3;
					printf("joined");
					game3.init(&stlObjectsBS[0],&stlObjectsBS[1],&stlObjectsBS[2],&stlObjectsBS[3],&stlObjectsBS[4],&stlObjectsBS[5]);
				}

			}
			if(button == 4){
				HardReset();
			}
		}
		else if(mode == 1){
			int press = game1.getCollision(&camera,mx,my);
			if(press>-1){
				game1.sendIndex(press);
			}
			if(button == 1){
				game1.leave();
				mode = 0;
			}
		}
		else if(mode == 2){
			int press = game2.getCollision(&camera,mx,my);
			if(press>-1){;
				game2.sendIndex(press);
			}
			if(button == 1){
				game2.leave();
				mode = 0;
			}
		}
		else if(mode == 3){
			if(!game3.gotShips){
				game3.getShips();
			}
			else{
				int press = game3.getCollision(&camera,mx,my);
				if(press>-1){;
					game3.sendIndex(press);
				}
			}
			if(button == 1){
				game3.leave();
				mode = 0;
			}
		}
		
	}
	if(mode == 0){
		message.erase();
		message = "Welcome!";
	}
	if(mode == 1){
		message.erase();
		game1.update();
		if(game1.c4.winStatus == 0){
			if(game1.c4.turnStatus == 1){			
				message = "Your Turn!";
			}
			if(game1.c4.turnStatus == 2){			
				message = "Opponents Turn!";
			}
			if(game1.c4.player == 3){
				printf("player: %d\n",game1.c4.player);
				message = "Spectator";
			}
		}
		if(game1.c4.winStatus == 1){	
			message = "You Won!";
		}
		if(game1.c4.winStatus == 2){		
			message = "You Lost!";
		}
		if(game1.c4.winStatus == 3){		
			message = "Draw";
		}
	}
	if(mode == 2){
		message.erase();
		game2.update();
		if(game2.ttt.winStatus == 0){
			if(game2.ttt.turnStatus == 1){			
				message = "Your Turn!";
			}
			if(game2.ttt.turnStatus == 2){			
				message = "Opponents Turn!";
			}
			if(game2.ttt.player == 3){
				message = "Spectator";
			}
		}
		if(game2.ttt.winStatus == 1){	
			message = "You Won!";
		}
		if(game2.ttt.winStatus == 2){		
			message = "You Lost!";
		}
		if(game2.ttt.winStatus == 3){		
			message = "Draw";
		}
	}
	if(mode == 3){
		message.erase();
		if(game3.gotShips){
			game3.update();
		}
		if(!game3.gotShips){
			game3.getShips();
		}
		if(game3.bs.winStatus == 0){
			if(game3.bs.turnStatus == 1){			
				message = "Your Turn!";
			}
			if(game3.bs.turnStatus == 2){			
				message = "Opponents Turn!";
			}
			if(game3.bs.player == 3){
				message = "Spectator";
			}
		}
		if(game3.bs.winStatus == 1){	
			message = "You Won!";
		}
		if(game3.bs.winStatus == 2){		
			message = "You Lost!";
		}
		if(game3.bs.winStatus == 3){		
			message = "Draw";
		}
	}
	needRedraw=true;
}

/* virtual */ void FsLazyWindowApplication::Draw(void)
{

	int wid,hei;
	
	// 1. Clear Screen
	glDisable(GL_COLOR_MATERIAL);
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	FsGetWindowSize(wid,hei);
	fwid = wid;
	fhei = hei;
	glViewport(0,0,wid,hei);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, wid, 0.0, hei, -1.0, 1.0);
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, imageTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex2f(0, 0);
	glTexCoord2f(0, 0); glVertex2f(0, hei);
	glTexCoord2f(1, 0); glVertex2f(wid, hei);
	glTexCoord2f(1, 1); glVertex2f(wid, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glColor3f(1,0,0);
	glEnable(GL_COLOR_MATERIAL);
	drawButtons(mode,button1,button2,button3,button4,button5);
	drawText(mode,hei,button1,button2,button3,button4,button5);
	drawMessage(wid,hei,button5,message.c_str());
	glPopMatrix();


	// 2. Enable Depth Test
	glEnable(GL_DEPTH_TEST);

	glShadeModel(GL_SMOOTH);


	// 3. Use entire window

	
	// 4. Set up projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glLoadMatrixd(projectionMat);
	glMultMatrixd((double*)camera.oglCam.data);

	// 5. Calculate & set modelview matrix
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glMultMatrixd((double*)camera.viewMatrix.data);
	GLfloat lightDir[]={0,0,1.0f,0};
	GLfloat AmbientLight[] = {.3,.3,.3,1.0};
	glLightfv(GL_LIGHT0,GL_POSITION,lightDir);
	glLightfv(GL_LIGHT0,GL_AMBIENT,AmbientLight);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//printf("woop\n");
	if(mode == 1){
		game1.draw();
	}
	if(mode == 2){
		game2.draw();
	}
	if(mode == 3){
		game3.draw();
	}
	glDisable(GL_DEPTH_TEST);
	
	
	glLoadIdentity();
	// 7. Show it
	FsSwapBuffers();
	needRedraw=false;
}
/* virtual */ bool FsLazyWindowApplication::UserWantToCloseProgram(void)
{
	return true; // Returning true will just close the program.
}
/* virtual */ bool FsLazyWindowApplication::MustTerminate(void) const
{
	return FsLazyWindowApplicationBase::MustTerminate();
}
/* virtual */ long long int FsLazyWindowApplication::GetMinimumSleepPerInterval(void) const
{
	return 10;
}
/* virtual */ void FsLazyWindowApplication::BeforeTerminate(void)
{
	game1.c4.Leave();
	game2.ttt.Leave();
	camera.inputVideo.release();
}
/* virtual */ bool FsLazyWindowApplication::NeedRedraw(void) const
{
	return needRedraw;
}


static FsLazyWindowApplication *appPtr=nullptr;

/* static */ FsLazyWindowApplicationBase *FsLazyWindowApplicationBase::GetApplication(void)
{
	if(nullptr==appPtr)
	{
		appPtr=new FsLazyWindowApplication;
	}
	return appPtr;
}
