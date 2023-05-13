#include "led.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "displayChess.h"
#include "chess.h"
#include "touch.h"
#include "exti.h"
#include "beep.h"
#include "check.h"

int8_t selectIndex = -2;
int8_t gameRunFlag;

int main(void)
{
	PieceTypedef tempPiece;
	Path path;
	ChangeBuffer changeBuffer;
	int8_t clearFlag;
	int8_t assassin; //刺客，是哪个棋子将军的

	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200);
	KEY_Init();
	BEEP_Init();
	extiInit();
	LCD_Init();
	tp_dev.init();
	POINT_COLOR = RED;
	initUI(); //象棋的初始界面

	while (1)
	{
		getPath(&path);
		tempPiece = piece[path.indexS];

		/*判断得到的path是否符合规则，符合则返回1*/
		if (tempPiece.judge(tempPiece.x, tempPiece.y, path.x, path.y, TURN))
		{
			change(path.indexS, path.x, path.y, &changeBuffer); //先把棋子的数据和棋局数据更改，界面并没有改变，因为还没判断这样走是否被将军

			/*判断执行change()过后，是否被将军，被将军则移动无效，需要backChange()*/
			if (check(!tempPiece.camp, &assassin) || generalLine())
			{
				backChange(&changeBuffer);	//这样走不行，会被将军，所以撤销移动
				deSelectPiece(path.indexS); //把屏幕中选中的棋子取消选中

				write_Chinese(sX, sY, '无', DARKBLUE, Libbuttom, lengthCH2);
				write_Chinese(sX + lengthCH2, sY, '效', DARKBLUE, Libbuttom, lengthCH2);
				clearFlag = 1;
			}
			else //这样走可行
			{
				clearPiece(changeBuffer.AX, changeBuffer.AY); //把旧棋子擦除,注意，到这里才改动界面
				makePiece(path.indexS);						  //在移动路径的目的地画一个新棋子
				changeBuffer2 = changeBuffer;				  //结构体赋值，changeBuffer2专用来悔棋，它存储的是真正移子过程的数据
				undoEn = 1;									  //悔棋允许标志位，每次移子之后使能，悔棋之后禁止，

				if (check(tempPiece.camp, &assassin)) //每次移子后判断是否将军
				{
					PBout(8) = 1;			//蜂鸣器响
					if (judgeWin(assassin)) //如果将军了接着进一步判断是否将死
					{
						POINT_COLOR = CYAN;
						LCD_Fill(sX, sY, sX + 2 * lengthCH2, sY + lengthCH2, BACK_COLOR);
						LCD_ShowString(sX + 3 * lengthCH2, sY, sizeENG * 6, sizeENG, sizeENG, " game over!");

						if (TURN)
							write_Chinese(sX, sY, '黑', BLACK, Libbuttom, lengthCH2);
						else
							write_Chinese(sX, sY, '红', RED, Libbuttom, lengthCH2);

						write_Chinese(sX + lengthCH2, sY, '方', CYAN, Libbuttom, lengthCH2);
						write_Chinese(sX + lengthCH2 * 2, sY, '胜', CYAN, Libbuttom, lengthCH2);

						PBout(8) = 0;
						changeBuffer2.indexSrc = -2; //中断里会检测这个值，检测到就return
						while (1)
							;
					}
					write_Chinese(sX, sY, '将', TURN ? BLACK : RED, Libbuttom, lengthCH2);
					write_Chinese(sX + lengthCH2, sY, '军', TURN ? BLACK : RED, Libbuttom, lengthCH2);
					clearFlag = 1; //下一次可以擦除提示
					PBout(8) = 0;  //关闭蜂鸣器
				}
				else if (clearFlag) //是否有提示信息要擦除
				{
					LCD_Fill(sX, sY, sX + 2 * lengthCH2, sY + lengthCH2, BACK_COLOR);
					clearFlag = 0;
				}

				TURN = !TURN;
			}
		}
		else //得到的path路径不符合规则
		{
			deSelectPiece(path.indexS); //取消选中状态
			write_Chinese(sX, sY, '无', DARKBLUE, Libbuttom, lengthCH2);
			write_Chinese(sX + lengthCH2, sY, '效', DARKBLUE, Libbuttom, lengthCH2);
			clearFlag = 1;
		}
	}
}

void EXTI4_IRQHandler(void) //按键中断，悔棋处理函数
{
	int8_t index2 = changeBuffer2.indexDes;
	int8_t index = changeBuffer2.indexSrc;
	int8_t tmpX, tmpY;
	int8_t noUse;
	tmpX = piece[index].x;
	tmpY = piece[index].y;

	if (changeBuffer2.indexSrc == -2) //刚复位的状态，或者是结束状态，此时按悔棋则return即可
	{
		EXTI_ClearITPendingBit(EXTI_Line4);
		return;
	}

	delay_ms(10); //延时消抖

	if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0 && undoEn == 1)
	{

		if (selectIndex != -2) //如果这个时候玩家已经选中了棋子，则取消选中
		{
			deSelectPiece(selectIndex);
		}

		backChange(&changeBuffer2); //撤销移子过程的数据更改

		if (index2 != -1) //如果原来移子目的地有棋子的话，要把它画回来
		{
			makePiece(index2);
		}
		else //没有的话，擦除棋子即可

			clearPiece(tmpX, tmpY);
		makePiece(index); //在移子的源位置把棋子画回来

		if (check(TURN, &noUse) || generalLine()) //检查是否被将军
		{
			write_Chinese(sX, sY, '将', TURN ? BLACK : RED, Libbuttom, lengthCH2);
			write_Chinese(sX + lengthCH2, sY, '军', TURN ? BLACK : RED, Libbuttom, lengthCH2);
		}
		else
		{
			LCD_Fill(sX, sY, sX + lengthCH2 * 2, sY + lengthCH2, BACK_COLOR);
		}

		TURN = !TURN; //换一方移子

		undoEn = 0; //悔棋之后不允许再悔，移子之后再使能悔棋
	}

	while (!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4))
		; //等待按键抬起
	EXTI_ClearITPendingBit(EXTI_Line4);
}
