#include <stdio.h>
#include <string.h>
#include <vector>

#include "binstl.h"



bool CPUisLittleEndian(void)
{
	unsigned int x=1;
	unsigned char *ptr=(unsigned char *)&x;
	if(0!=ptr[0])
	{
		return true;
	}
	return false;
}

unsigned int InterpretInt(const unsigned char dat[4])
{
	return (unsigned int)dat[0]+
	       (unsigned int)dat[1]*0x100+
	       (unsigned int)dat[2]*0x10000+
	       (unsigned int)dat[3]*0x1000000;
}

float InterpretFloat(const unsigned char dat[4])
{
	if(true==CPUisLittleEndian())
	{
		auto fPtr=(const float *)dat;
		return *fPtr;
	}
	else
	{
		unsigned char flip[4]=
		{
			dat[3],
			dat[2],
			dat[1],
			dat[0],
		};
		auto fPtr=(const float *)flip;
		return *fPtr;
	}
}

bool ReadBinStl(std::vector <float> &vtx,std::vector <float> &nom,const char fName[])
{
	if(true==IsAsciiStl(fName))
	{
		printf("Is ASCII stl.\n");
		return false;
	}

	FILE *fp=fopen(fName,"rb");
	if(nullptr==fp)
	{
		return false;
	}

	unsigned char buf[80];
	fread(buf,1,80,fp);

	long long int readSize=fread(buf,1,4,fp);
    auto nTri=InterpretInt(buf);
    printf("%d triangles\n",nTri);
	if(4!=readSize)
	{
		goto BAILOUT;
	}


	for(int i=0; i<nTri; ++i)
	{
		if(50!=fread(buf,1,50,fp))
		{
			break;
		}

		float nx=InterpretFloat(buf);
		float ny=InterpretFloat(buf+4);
		float nz=InterpretFloat(buf+8);

		float x0=InterpretFloat(buf+12);
		float y0=InterpretFloat(buf+16);
		float z0=InterpretFloat(buf+20);

		float x1=InterpretFloat(buf+24);
		float y1=InterpretFloat(buf+28);
		float z1=InterpretFloat(buf+32);

		float x2=InterpretFloat(buf+36);
		float y2=InterpretFloat(buf+40);
		float z2=InterpretFloat(buf+44);

		nom.push_back(nx);nom.push_back(ny);nom.push_back(nz);
		nom.push_back(nx);nom.push_back(ny);nom.push_back(nz);
		nom.push_back(nx);nom.push_back(ny);nom.push_back(nz);

		vtx.push_back(x0);vtx.push_back(y0);vtx.push_back(z0);
		vtx.push_back(x1);vtx.push_back(y1);vtx.push_back(z1);
		vtx.push_back(x2);vtx.push_back(y2);vtx.push_back(z2);

		// printf("%f %f %f\n",nx,ny,nz);
		// printf("%f %f %f\n",x0,y0,z0);
		// printf("%f %f %f\n",x1,y1,z1);
		// printf("%f %f %f\n",x2,y2,z2);
		// printf("\n");
	}


	fclose(fp);
	return true;

BAILOUT:
	fclose(fp);
	return false;
}



void FindBoundingBox(YsVec3 minmax[2],const std::vector <float> &vtx)
{
	if(3<=vtx.size())
	{
		minmax[0].Set(vtx[0],vtx[1],vtx[2]);
		minmax[1].Set(vtx[0],vtx[1],vtx[2]);
		for(long long int i=0; i+3<=vtx.size(); i+=3)
		{
			if(vtx[i]<minmax[0].x())
			{
				minmax[0].SetX(vtx[i]);
			}
			if(minmax[1].x()<vtx[i])
			{
				minmax[1].SetX(vtx[i]);
			}

			if(vtx[i+1]<minmax[0].y())
			{
				minmax[0].SetY(vtx[i+1]);
			}
			if(minmax[1].y()<vtx[i+1])
			{
				minmax[1].SetY(vtx[i+1]);
			}

			if(vtx[i+2]<minmax[0].z())
			{
				minmax[0].SetZ(vtx[i+2]);
			}
			if(minmax[1].z()<vtx[i+2])
			{
				minmax[1].SetZ(vtx[i+2]);
			}
		}
	}
	else
	{
		minmax[0]=YsVec3::Origin();
		minmax[1]=YsVec3::Origin();
	}
}



bool IsAsciiStl(const char fName[])
{
	const int bufSize=2024;

	long long int fSize=0;
	char dat[bufSize];
	bool hasSolid=false,hasVertex=false,hasFacet=false,hasLoop=false;

	FILE *fp=fopen(fName,"rb");
	if(nullptr!=fp)
	{
		fSize=fread(dat,1,2024,fp);
		fclose(fp);
	}
	else
	{
		return false;
	}

	for(int i=0; i<fSize; ++i)
	{
		if(i+5<fSize && 0==strncmp(dat+i,"solid",5))
		{
			hasSolid=true;
		}
		else if(i+6<fSize && 0==strncmp(dat+i,"vertex",6))
		{
			hasVertex=true;
		}
		else if(i+5<fSize && 0==strncmp(dat+i,"facet",5))
		{
			hasFacet=true;
		}
		else if(i+4<fSize && 0==strncmp(dat+i,"loop",4))
		{
			hasLoop=true;
		}
	}

	return (true==hasSolid && true==hasVertex && true==hasFacet && true==hasLoop);
}
