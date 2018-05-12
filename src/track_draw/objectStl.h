

#ifndef OBJECTSTL_H
#define OBJECTSTL_H

#include <stdio.h>
#include <iostream>
#include <vector>

class stlObject{
public:
	std::vector <float> vtx;
	std::vector <float> nom;
	std::vector <float> col0;
	std::vector <float> col1;
	std::vector <float> col2;
	int type;
	int nVtx;
	double scale;
	void init(char const * filename,double scale1);
	void init(char const * filename,double scale1, double red, double green, double blue);
	std::vector<float> * getVtx();
	std::vector<float> * getNom();
	std::vector<float> * getCol(int input);
	void translate (double x, double y, double z);
};

class Object{
public:
	double x;
	double y;
	double z;
	double roll;
	double pitch;
	double yaw;
	int state = 0;
	int color = 0;
	std::vector <float> *vtx;
	std::vector <float> *nom;
	std::vector <float> *col0;
	std::vector <float> *col1;
	std::vector <float> *col2;
	void init(stlObject *inStl);
	void changeState(int newstate);
	void Draw();
	void moveTo(double x1, double y1, double z1);
	void rotateTo(double roll1, double pitch1, double yaw1);
	void chooseColor(int ncolor);

};

#endif
