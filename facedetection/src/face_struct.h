#pragma once
#ifndef __FACE_STRUCT_H__
#define __FACE_STRUCT_H__

struct Bbox
{
    float score;
    int x1;
    int y1;
    int x2;
    int y2;
    float area;
    float ppoint[10];
    float regreCoord[4];
};

#endif // !__FACE_STRUCT_H__

