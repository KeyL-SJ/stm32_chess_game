#ifndef __DISPLAYCHESS_H__
#define __DISPLAYCHESS_H__

#include <sys.h>

//路径结构体，执行getPath，一个Path类型的参数的指针将会传入，执行完之后，这个参数会保存玩家
//移子的路径。这个路径不一定是正确有效的，须经过后续判断才可知
typedef struct 
{
    int8_t indexS; //玩家选定的棋子
    int8_t x;      //把选定的棋子进行移动，目的地的坐标（x，y）
    int8_t y;
}Path;

void initUI(void);
extern void debugUI(u8 i,u8 j,u8 k);
void initPiece(void);
void drawSelection(void);
void clearSelection(void);
void selectPiece(u8 index);
void deSelectPiece(u8 index);
void makePiece(int8_t index);
void clearPiece(int8_t x,int8_t y);
void getPath(Path *p);


extern u8 lengthCH;
extern u8 lengthCH2;
extern u8 pieceR;
extern u16 sX;
extern u16 sY;
extern u8 sizeENG;
extern unsigned char * Libbuttom;
extern u16 turnPiX[9];
extern u16 turnPiY[10];


#endif
