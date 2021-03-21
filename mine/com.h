#ifndef COM_H
#define COM_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PRI_ROW 9       //行列设置
#define MID_ROW 16
#define SEN_ROW 22

#define PRI_MINE 10     //雷数量设置
#define MID_MINE 40
#define SEN_MINE 99

//错误判断宏
#define SYSERR(X,OPTION,Y,K,Z) if((X)OPTION(Y))\
                                {\
                                    printf("%s:%s:%d:%s\n",__FILE__,__func__,__LINE__,(K));\
                                    return Z;\
                                }




typedef struct mine  //棋盘画布结构体
{
    int row;
    int mine_num;
    char **board;
    char **canvas;
}mine_t;

typedef struct hero1  //存储英雄榜结构体
{
    char hero[20];
    int timep;
}hero1_t;



int board_init(char **data,int num); //创建雷盘画布列空间
int mine_init(mine_t *data,int row,int mine);   //创建空间
int menu(void);                     //菜单
void board_init1(char **data,int num,char ch);      //初始化雷排画布
void findmine(char **board,char **canvas,int row ,int mine);    //开始游戏扫雷
void setmine(char **data,int row,int mine);         //布雷
void print(char **,int row);                    //打印画布
void print1(char **,int row);                   //打印棋盘
int up_mine(char **board,char **canvas,int x,int y,int row);  //递归找雷
int get_mine(char **board,int x ,int y);        //找周围的雷函数
int world_heroes(int time1,int row,int flag);   //创建英雄榜 打印英雄榜 



#endif
