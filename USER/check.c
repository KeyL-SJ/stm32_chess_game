#include "check.h"
#include "chess.h"
#include "lcd.h"


//判断将和帅是否在同一条直线上，并且该线上两子之间是否还有其它棋子，
//同线且该线上无其它子则返回1，否则0
int8_t generalLine()
{
    int8_t i;

	 if(piece[4].x != piece[27].x)     //index =4是将   index=27是帅，不在同一直线上，返回0
    return 0;
    else
    {
		 for(i=piece[4].y+1 ; i<piece[27].y ; i++)//依次轮询直线上的所以位置，如果有子则返回0
        {
            if(pieceTable[piece[4].x][i] != -1)
            return 0;
        }
    }

    return 1;
}

//判断camp阵营是否对敌方有将军，有将军返回1，没有将军返回0;
//camp:阵营
//p:执行函数之后，*p就是将军的棋子的index
int8_t check(int8_t camp,int8_t *p)    
{
    int8_t i;
    PieceTypedef tempPiece;
    int8_t index1[] = {0,1,7,8,9,10,11,12,13,14,15};     //车马炮兵的index，只有这些棋子可以将军
    int8_t index2[] = {16,17,18,19,20,21,22,23,24,30,31};
    if(camp)
    {
        for(i=0;i<=10;i++)     //循环判断camp阵营里车马炮兵是否可以一步到达对面的将（帅）的位置，如果可以那就是将军了
        {
            tempPiece = piece[index1[i]];
			   if(tempPiece.status)//如果该棋子已经阵亡，显然不能将军
            {
                if(tempPiece.judge(tempPiece.x,tempPiece.y,piece[27].x,piece[27].y,blackCAMP))
                {
                    *p = index1[i];
                    return 1;
                }
            }
        }
    }
    else
    {
		  for(i=0;i<=10;i++)   //同上一步原理
        {
            tempPiece = piece[index2[i]];
            if(tempPiece.status)
            {
                if(tempPiece.judge(tempPiece.x,tempPiece.y,piece[4].x,piece[4].y,redCAMP))
                {
                    *p = index2[i];
                    return 1;                  
                }
            }
        }
    }
	 
    return 0;//
}

//在将军时判断是否将死，将死了就返回1；否则0；
//assassinIndex：是哪个棋子将军      assassin是刺客的意思
int8_t judgeWin(int8_t assassinIndex)
{
    int8_t generalIndex,i,dir,tempIndex;   
    int8_t x1,y1,x2,y2;
	
    if(TURN)
    {
        generalIndex = 27;//目标是将
    }
    else 
    {
        generalIndex = 4;//目标是帅
    }
	 
    x1 = piece[assassinIndex].x;
    y1 = piece[assassinIndex].y;
    x2 = piece[generalIndex].x;
    y2 = piece[generalIndex].y;

	 //将（帅）尝试移动，看能不能逃脱被将军
	 if(tryMove(generalIndex,x2,y2+1))
	 return 0;
	 if(tryMove(generalIndex,x2,y2-1))
	 return 0;
	 if(tryMove(generalIndex,x2+1,y2))
	 return 0;
	 if(tryMove(generalIndex,x2-1,y2))
	 return 0;	 
    
    switch (assassinIndex)
    {
    
    case 9:                                //如果是炮将军
    case 10:
    case 21:
    case 22:

	     if(judgePosition(x1,y1))//看能不能直接把炮杀死
        return 0;

        if(x1 == x2)//炮纵向的情况
        {
            dir = y2>y1?-1:1;
            for(i=y2+dir ; i!=y1 ; i+=dir)//炮从将（帅）旁边位置开始，一个个位置轮询，直到炮的位置的前一个
            {
                tempIndex = pieceTable[x1][i];
					
					 if(tempIndex!=-1 && escapeFromCannon(tempIndex,assassinIndex))//有棋子，则看看这个棋子能不能通过移动来逃脱炮对将（帅）的威胁
                   return 0;
                else if
						 (tempIndex==-1 && judgePosition(x1,i))   //是一个空位置，则看看是否有棋子能补位
                return 0;
            }
            return 1;
        }
        else  //炮横向将军的情况，原理同上
        {
            dir = x2>x1?-1:1;
            for(i=x2+dir ; i!=x1 ; i+=dir)
            {
                tempIndex = pieceTable[i][y1];
                if(tempIndex!=-1 && escapeFromCannon(tempIndex,assassinIndex))
                return 0;
                else
                if(tempIndex==-1 && judgePosition(i,y1))
                return 0;
            }
            return 1;
        }   
        

    case 24:                              //如果是马将军
    case 30:
    case 1:
    case 7:

        if(judgePosition(x1,y1))//看能不能直接把炮杀死
        return 0;
        if(judgePosition((x2-x1)/2+x1,(y2-y1)/2+y1))//看能不能堵马脚
        return 0;

        return 1;

    case 0:                             //如果是车将军
    case 8:
    case 23:
    case 31:

        if(x1 == x2)                    //车和将（帅）在同一列的情况
        {
            dir = y2>y1?-1:1;
            for(i=y2+dir ; i!=y1+dir ; i+=dir)//从将的旁边开始循环直到车的位置，轮询，看是否有棋子能到达该位置
            {
                if(judgePosition(x1,i))
                return 0;
            }
            return 1;
        }
        else                            //车和将（帅）在同一行的情况
        {
            dir = x2>x1?-1:1;
            for(i=x2+dir ; i!=x1+dir ; i+=dir)
            {
                if(judgePosition(i,y1))
                return 0;
            }
            return 1;
        }   

    default:
                                    //不是车、马、炮的话，那只能是兵了
        if(judgePosition(x1,y1))    
        return 0;
        else
        return 1;                              
    }
}

//炮将军才会执行，炮的规则的特殊，这里外加一个函数来处理，让judgeWin函数结构更清晰
//index:炮将军的中间子  assassin:将军的棋子的index，这里是4个炮中的一个
//判断对面炮对己方将军时，是否能通过移动中间子来逃过被将军，
//能逃脱返回1；不能则返回0
int8_t escapeFromCannon(int8_t betweenIndex,int8_t assassinIndex)
{
	 //四个数组表示，中间子分别是马，象，士，兵时，移动时x和y允许的变化值
    int8_t dxdy[] = {1,2,1,-2,-1,2,-1,-2,2,1,2,-1,-2,1,-2,-1};  //马
    int8_t dxdy2[] = {2,2,2,-2,-2,2,-2,-2};                     //象
    int8_t dxdy3[] = {1,1,1,-1,-1,1,-1,-1};                     //士
    int8_t dxdy4[] = {0,1,0,-1,1,0,-1,0};                       //兵
    
	 int8_t x1,y1,x2,y2,i,dir,tempIndex;

    if(piece[betweenIndex].camp == TURN)  //如果中间子是对方阵营的，自己不能移动，显然返回0
    return 0;
    
    x1 = piece[betweenIndex].x;
    y1 = piece[betweenIndex].y;
    x2 = piece[assassinIndex].x;
    y2 = piece[assassinIndex].y;

    switch(betweenIndex)
    {
    case 9:                                //如果中间子是炮
    case 10:
    case 21:
    case 22:

        if(x1 == x2)                      //炮和中间子在同同一列的情况
        {
            for(i=0;i<=8;i++)         //尝试让中间的子水平方向移动，看是否能解除将军
            {
                if(i == x1)
                continue;
 
					 if(tryMove(betweenIndex,i,y1))//能成功移动的话就解除将军了
					 return 1;
            }

            dir = y2>y1?1:-1;
            i = y2+dir;          
            while(i<=9 && i>=0)
            {
                tempIndex = pieceTable[x1][i];//炮将军的反方向的位置，轮询，找到第一个棋子，如果是敌方的，则中间子炮可以打过去
                if(tempIndex == -1)//还没有找到
                {
                    i+=dir;
                }
                else//找到了第一个棋子
                {
						  if(piece[tempIndex].camp == !TURN)//找到的棋子是己方的，则不能打过去
                    return 0;
						  
                    else
                    {
 								if(tryMove(betweenIndex,x1,i))//尝试将中间子炮打过去
								return 0;
								
                        return 1;
                    }
                }
            }
				
				return 0;
        }
        else                         //炮和中间子在同一行的情况，同理
        {
            for(i=0;i<=9;i++)
            {
                if(i == y1)
                continue;
						  
					 if(tryMove(betweenIndex,x1,i))
					 return 1;					              
            }

            dir = x2>x1?1:-1;
            i = x2+dir;
            while(i<=8 && i>=0)
            {
                tempIndex = pieceTable[i][y1];
                if(tempIndex == -1)
                {
                    i+=dir;
                }
                else
                {
                    if(piece[tempIndex].camp == !TURN)
                    return 1;
                    else
                    {							
								if(tryMove(betweenIndex,i,y1))
								return 0;
								
                        return 1;
                    }
                }
            }    
				
				return 0;
        }

    case 24:                              //如果中间子是马
    case 30:
    case 1:
    case 7:    

        for(i=0;i<=7;i++)               //马移动一次有8种可能的情况出现
        {
				if(tryMove(betweenIndex,x1+dxdy[2*i],y1+dxdy[2*i+1]))//尝试移动中间子，看是否能解除被将军
			   return 1;				
        }
        return 0;

    case 0:                             //如果中间子是车
    case 8:
    case 23:
    case 31:
         

	     if(tryMove(betweenIndex,x2,y2))//看能不能直接把正在威胁将（帅）的炮杀死
		  return 1;	
        
		  if(x1 == x2)                  //炮和中间子在同一列
        {
            for(i=0;i<=8;i++)         //尝试让中间子横向移动
            {
                if(i == x1)
                continue;

				    if(tryMove(betweenIndex,i,y1))//能移走且不被将军，则解除了将军
					 return 1;						 
            } 
            return 0;
        }
        else                         //炮和中间子在同一行，同理
        {
            for(i=0;i<=9;i++)
            {
                if(i == y1)
                continue;

				    if(tryMove(betweenIndex,x1,i))
					 return 1;						 
            }  
            return 0;          
        }       
       

    case 2:                             //如果中间子是象
    case 6:
    case 25:
    case 29:                            

        for(i=0;i<=3;i++)
        {
			   if(tryMove(betweenIndex,x1+dxdy2[2*i],y1+dxdy2[2*i+1]))
				return 1;				
        }
        return 0;

    case 3:                             //如果中间子是士
    case 5:
    case 26:
    case 28:

        for(i=0;i<=3;i++)
        {
			   if(tryMove(betweenIndex,x1+dxdy3[2*i],y1+dxdy3[2*i+1]))
				return 1;
        }
        return 0;


    default:                            //其它情况只能是兵了

        for(i=0;i<=3;i++)
        {
				if(tryMove(betweenIndex,x1+dxdy4[2*i],y1+dxdy4[2*i+1]))
				return 1;
        }
        return 0;
    }

}

//在被将军时，给定一个坐标(pX,pY),看己方是否存在一个棋子（非将和帅）能到达，
//且到达后不被将军，存在返回1，不存在返回0
int8_t judgePosition(int8_t pX,int8_t pY)
{
    int8_t index;
    int8_t noUse;
    ChangeBuffer changeBuffer;
    if(TURN)
    {
        for(index=16;index<=31;index++)//轮询所有棋子，看它是否能到达（pX，pY）；
        {
            if(index==27)//将（帅）出外
            continue;

				if(tryMove(index,pX,pY))
				return 1;
					
        }
        return 0;
    }
    else
    {
        for(index=0;index<=15;index++)
        {
            if(index==4)
            continue;

				if(tryMove(index,pX,pY))
				return 1;
					
        }    
        return 0;    
    }
}

//将索引为index的棋子尝试移动到（x，y）位置，如果移动之后被将军，
//则认为移动失败，返回0；移动成功则返回1；
//无论成功与否，都要把移动进行撤回，因为这只是一个判断而不是真正的移子
uint8_t tryMove(uint8_t index,uint8_t x,uint8_t y)
{
	  int8_t noUse;               //noUse:  一个没用的参数，check要用，那么我们就要给 
     ChangeBuffer changeBuffer;
	
     if(piece[index].status && piece[index].judge(piece[index].x,piece[index].y,x,y,piece[index].camp))//index所对应的棋子必须活着，而且
     {                                                                                                 //按规则可以到达（x，y）
         
		  
		   change(index,x,y,&changeBuffer); //尝试移子，并且把过程数据记录到changeBuffer，接下来必有一次backChange（）；
		  
         if(!check(TURN,&noUse) && !generalLine())
         {
             backChange(&changeBuffer);
             return 1;
         }
			else
         backChange(&changeBuffer);
     }
     
	  return 0;
			
}

