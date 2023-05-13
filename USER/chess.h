#ifndef __CHESS_H__
#define __CHESS_H__

#include <sys.h>

#define redCAMP 0         //代表红色阵营
#define blackCAMP 1       //代表黑色阵营
#define ALIVE 1           //活着
#define DEATH 0           //阵亡





//移子缓存结构体，移动棋子时，把源棋子目的地棋子(目的地无棋子的话用-1表示索引)的索引、位置进行缓存，
//悔棋、判断将军时会用到这个缓存
typedef struct 
{
   int8_t indexSrc;    //源棋子的索引
   int8_t indexDes;    //目的地棋子的索引（目的地无棋子的话为-1）
   int8_t AX;          //源棋子的横坐标
   int8_t AY;          //源棋子的纵坐标
   int8_t BX;          //目的地棋子的横坐标
   int8_t BY;          //目的地棋子的纵坐标
}ChangeBuffer;


//棋子结构体
typedef struct
{
	unsigned int name;     //棋子的名字，是汉字的内码（'车'表示汉字”车“的一个内码），用两个字节存储
    int8_t x;             //棋子的格点坐标的横坐标                                             
    int8_t y;             //棋子的格点坐标的纵坐标
    int8_t camp;          //棋子属于哪个阵营的，是红方还是黑方，redCAMP：0      blackCAMP；1
	 int8_t (*judge)(int8_t x1,int8_t y1,int8_t x2,int8_t y2,int8_t Camp);//函数指针，路径判断函数。用函数名给这个成员赋值，此结构体就与该函数绑定了
    int8_t status;        //棋子的状态，是生存还是阵亡         ALIVE：1         DEATH：0
}PieceTypedef;



//路径判断函数，在棋子结构体初始化时，和相应的结构体绑定
int8_t car(int8_t x1,int8_t y1,int8_t x2,int8_t y2,int8_t Camp);//车
int8_t horse(int8_t x1,int8_t y1,int8_t x2,int8_t y2,int8_t Camp);//马
int8_t elephant(int8_t x1,int8_t y1,int8_t x2,int8_t y2,int8_t Camp);//象
int8_t guard(int8_t x1,int8_t y1,int8_t x2,int8_t y2,int8_t Camp);//士
int8_t general(int8_t x1,int8_t y1,int8_t x2,int8_t y2,int8_t Camp);//将
int8_t cannon(int8_t x1,int8_t y1,int8_t x2,int8_t y2,int8_t Camp);//炮
int8_t soldier(int8_t x1,int8_t y1,int8_t x2,int8_t y2,int8_t Camp);//兵

void change(int8_t index,int8_t x,int8_t y,ChangeBuffer *p);
void backChange(ChangeBuffer *p);

extern PieceTypedef piece[32];
extern int8_t pieceTable[9][10];
extern int8_t TURN;
extern int8_t undoEn;
extern ChangeBuffer changeBuffer2;



#endif

