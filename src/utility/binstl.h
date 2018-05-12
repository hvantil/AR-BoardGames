#ifndef BINSTL_H_IS_INCLUDED
#define BINSTL_H_IS_INCLUDED

#include <ysclass.h>

#include <vector>
bool ReadBinStl(std::vector <float> &vtx,std::vector <float> &nom,const char fName[]);

void FindBoundingBox(YsVec3 minmax[2],const std::vector <float> &vtx);

bool IsAsciiStl(const char fName[]);

#endif
