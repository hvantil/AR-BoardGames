#include <stdio.h>
#include <iostream>
#include <vector>

#include "objectStl.h"
#include <binstl.h>
#include <fslazywindow.h>

void stlObject::init(char const * filename, double scale1){

	ReadBinStl(vtx,nom,filename);
	YsVec3 minmax[2];
	FindBoundingBox(minmax,vtx);
	printf("%f %f %f    %f %f %f\n",minmax[0][0],minmax[0][1],minmax[0][2],minmax[1][0],minmax[1][1],minmax[1][2]);
	int nnVtx=vtx.size()/3;
	nVtx = nnVtx;
	scale = scale1;
	for(int i=0; i<nVtx; ++i) // color 0 = gray
	{
		col0.push_back(0.1f);
		col0.push_back(0.1f);
		col0.push_back(0.1f);
		col0.push_back(1.0f);
	}
	for(int i=0; i<nVtx; ++i) //color1 = red
	{
		col1.push_back(1.0f);
		col1.push_back(0.0f);
		col1.push_back(0.0f);
		col1.push_back(1.0f);
	}
	for(int i=0; i<nVtx; ++i) // color 1 = blue
	{
		col2.push_back(0.0f);
		col2.push_back(0.0f);
		col2.push_back(1.0f);
		col2.push_back(1.0f);
	}
	for(int i=0; i<nVtx*3; ++i)
	{
		vtx[i]=vtx[i]*scale;
	}
}

void stlObject::init(char const * filename, double scale1,double red, double green, double blue){
	ReadBinStl(vtx,nom,filename);
	YsVec3 minmax[2];
	FindBoundingBox(minmax,vtx);
	printf("%f %f %f    %f %f %f\n",minmax[0][0],minmax[0][1],minmax[0][2],minmax[1][0],minmax[1][1],minmax[1][2]);

	int nnVtx=vtx.size()/3;
	nVtx = nnVtx;
	scale = scale1;
	for(int i=0; i<nVtx; ++i) // color 0 = gray
	{
		col0.push_back(0.1f);
		col0.push_back(0.1f);
		col0.push_back(0.1f);
		col0.push_back(1.0f);
	}
	for(int i=0; i<nVtx; ++i) //color1 = red
	{
		col1.push_back(0.8f);
		col1.push_back(0.0f);
		col1.push_back(0.0f);
		col1.push_back(1.0f);
	}

	for(int i=0; i<nVtx; ++i) // color 2 = blue
	{
		col2.push_back(red);
		col2.push_back(green);
		col2.push_back(blue);
		col2.push_back(1.0f);
	}
	for(int i=0; i<nVtx*3; ++i)
	{
		vtx[i]=vtx[i]*scale;
	}
}


void stlObject::translate(double x, double y, double z){
	for(int i = 0; i < nVtx*3; i+=3){
		vtx[i]+= x*scale;
		vtx[i+1]+= y*scale;
		vtx[i+2]+= z*scale;
	}
}



std::vector<float> * stlObject::getVtx(){
	std::vector<float> * output = &vtx;
	return output;
}
std::vector<float> * stlObject::getNom(){
	std::vector<float> * output = &nom;
	return output;
}
std::vector<float> * stlObject::getCol(int input){
	if(input == 0){
		std::vector<float> * output = &col0;
		return output;
	}
	else if(input == 1){
		std::vector<float> * output = &col1;
		return output;
	}
	else if(input == 2){
		std::vector<float> * output = &col2;
		return output;
	}
	return nullptr;
}



void Object::init(stlObject *newStl){
	vtx = newStl->getVtx();
	nom = newStl->getNom();
	col0 = newStl->getCol(0);
	col1 = newStl->getCol(1);
	col2 = newStl->getCol(2);
}
void Object::moveTo(double x1, double y1, double z1){
	x = x1;
	y = y1;
	z = z1;
}
void Object::rotateTo(double roll1, double pitch1, double yaw1){
	roll = roll1;
	pitch = pitch1;
	yaw = yaw1;
}
void Object::chooseColor(int ncolor){
	color = ncolor;
}

void Object::changeState(int newstate){
	state = newstate;
}
void Object::Draw(){
	if(state != 0){
		glEnable(GL_COLOR_MATERIAL);
		glPushMatrix();
		glTranslated(x,y,z);
		glRotated(roll*180.0/YsPi,1.0,0,0);
		glRotated(pitch*180.0/YsPi,0,1.0,0);
		glRotated(yaw*180.0/YsPi,0,0,1.0);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		if(color == 0){
			glColorPointer(4,GL_FLOAT,0,col0->data());
		}
		
		else if(color == 1){
			glColorPointer(4,GL_FLOAT,0,col1->data());
		}
		else{
			glColorPointer(4,GL_FLOAT,0,col2->data());
			
		}
		glNormalPointer(GL_FLOAT,0,nom->data());
		glVertexPointer(3,GL_FLOAT,0,vtx->data());
		glDrawArrays(GL_TRIANGLES,0,vtx->size()/3);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glPopMatrix();
	}
}