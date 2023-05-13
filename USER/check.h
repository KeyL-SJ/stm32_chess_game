#ifndef __CHECK_H__
#define __CHECK_H__

#include "sys.h"

int8_t check(int8_t camp,int8_t *p);    //判断是否将军，有将军返回1，没有将军返�?0；camp阵营，决�? 是判断哪一方将�?
int8_t generalLine();

int8_t judgePosition(int8_t pX,int8_t pY);
int8_t judgeWin(int8_t assassinIndex);
int8_t escapeFromCannon(int8_t index,int8_t assassinIndex);
uint8_t tryMove(uint8_t index,uint8_t x,uint8_t y);





#endif



