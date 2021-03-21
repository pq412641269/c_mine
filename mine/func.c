#include "com.h"       //头文件，结构体，函数声明




int menu(void)         //游戏菜单函数
{
    mine_t *data = malloc(sizeof(mine_t));
    SYSERR(data,==,NULL,"malloc err!",-1);

                                                        
    char buf[4] = "";
    int level_row = PRI_ROW,level_mine = PRI_MINE,level = 0;
    
    while(1)
    {
        system("clear");               //清屏
        printf("\t\t扫雷游戏\n\n\n"); //打印菜单
        printf("1.开始游戏\n");
        printf("2.难度选择\n");
        printf("3.扫雷英雄榜\n");
        printf("4.退出游戏\n\n\n\n");
        printf("请选择:");
        setbuf(stdin,NULL);
        scanf("%4s",buf);        //输入游戏选择
        if(buf[0] == '4') 
        {
            break;
        }
        switch(buf[0])   //开始游戏
        {
        case '1':
            mine_init(data,level_row,level_mine);  //空间创建 棋盘初始化 布雷 开始游戏
            break;
        case '2':                                                   
                                            //选择菜单
            system("clear");               //清屏
            printf("\t\t请选择游戏难度\n");
            printf("1.初级难度(9*9雷盘)\n");
            printf("2.中等难度(16*16雷盘)\n");
            printf("3.高等难度(22*22雷盘 )\n");
            printf("请选择:");
            setbuf(stdin,NULL);
            scanf("%d",&level);

            while(level >= 1 && level <= 3)   //选择游戏难度
            {
                if(level == 1)
                {
                    level_row = PRI_ROW;
                    level_mine = PRI_MINE;
                }
                if(level == 2)
                {
                    level_row = MID_ROW;
                    level_mine = MID_MINE;
                }
                if(level == 3)
                {
                    level_row = SEN_ROW;
                    level_mine = SEN_MINE;
                }
                level = 0;
            }
            break;
        case '3':
                                                //打印扫雷英雄榜
                system("clear");               //清屏
                printf("\t\t  扫雷英雄榜\n");

                world_heroes(0,0,1);        //打印英雄榜

                printf("按回车返回菜单！\n");
                getchar();
                getchar();
            break;
        default:
            printf("输入错误请重新输入！\n");
            printf("按回车继续！\n");
            getchar();
            getchar();
            
        }
    }
    free(data);
    
    return 0;

}





int mine_init(mine_t *data,int row ,int mine)   //空间创建 初始化函数
{
    data->row = row;                            //行列数赋值
    data->mine_num = mine;                      //雷数赋值

    SYSERR(data,==,NULL,"data is err!",-1);    


    (data->board) = malloc(sizeof(char *) * (data->row+2)); //棋盘行空间创建
    SYSERR((data->board),==,NULL,"data is err!",-1);

    (data->canvas) = malloc(sizeof(char *) * (data->row));  //画布行空间创建
    SYSERR((data->canvas),==,NULL,"data is err!",-1);
    
    board_init(data->board,data->row+2);     //画布列空间创建
    board_init(data->canvas,data->row);     //画布列空间创建

    board_init1(data->canvas,data->row,'*');        //画布初始化 * 号
    board_init1(data->board,data->row+2,'0');       //棋盘初始化为 0
    
    setmine(data->board,data->row,data->mine_num);  //开始布雷,雷为1

    findmine(data->board,data->canvas,data->row,data->mine_num);  //开始游戏扫雷
    

    free(data->board);                          //空间释放
    free(data->canvas);

}





int board_init(char **data,int num)             //列空间创建函数
{
    int i;
    int flag = 0;
    for(i =0; i<num; i++)
    {
        *(data+i) = malloc(num);        //空间创建
        if(*(data + i) == NULL)
        {
            flag = 1;
            printf("malloc err\n");
            break;
        }
    }
    i--;
    if(flag == 1)               //如果失败就释放空间
    {
        for(;i>=0;i--)
        {
            free(*(data+i));
            *(data+i) = NULL;
        }
        exit(0);
    }

}






void board_init1(char **data,int num,char ch)   //棋盘画布初始化函数
{
    int i,j;
    for(i = 0;i < num; i++)
    {
        for(j = 0; j < num; j++)
        {
            data[i][j] = ch;
        }
        
    }

}








void setmine(char **data,int row,int mine)  // 布雷函数
{
    int count = mine;
    int x,y;

    srand(time(NULL));              //做种
    while(count)
    {
        x = rand() %row + 1;        //取随机值布雷
        y = rand() %row + 1;

        if(data[x][y] == '0')
        {
            data[x][y] = '1';
            count--;              
        }
    }

}








void print(char **data,int row)                 //画布打印函数
{
    int i,j;
    for(i = 0; i <=row ;i++)
    {
        printf("%3d ",i);
    }
    printf("\n");
    for(i = 0;i <row; i++ )
    {
        printf("%3d",i+1);
        for(j=0;j<row;j++)
        {
            printf("%4c",data[i][j]);
        }
        printf("\n");
        
    }
}








void print1(char **data,int row)                //棋盘打印函数
{
    int i,j;
    for(i = 0; i <= row ;i++)
    {
        printf("%3d ",i);
    }
    printf("\n");


    for(i = 1;i <= row; i++ )
    {
        printf("%3d",i);
        for(j=1;j<=row;j++)
        {
            printf("%4c",data[i][j]);
        }
        printf("\n");
    }
}








void findmine(char **board,char **canvas,int row ,int mine)    //开始游戏扫雷
{
    int flag;
    int x,y;
    int i,j,num = 0;
    clock_t start,stop;
    int time1 = 0;

    start = time(NULL); //开始游戏计时

    while(num < (row*row)-mine) 
    {
        
        system("clear");               //清屏
        //print1(board,row);           //打印雷盘调试用
        putchar('\n');
        printf("已翻: %d  剩余未翻:%d\n",num,(row*row)-num-mine);       //报告已翻棋盘数量
        print(canvas,row);
        printf("请输入要排查的作坐标:");            //输入棋盘坐标
        setbuf(stdin,NULL);
        scanf("%d%d",&x,&y);


        if(x >= 1 && x <= row && y>=1 && y <= row )        
        {
            if(board[x][y] == '1')              //判断是否触雷
            {
                system("clear");               //清屏
                printf("很遗憾，你被炸死了！\n");
                print1(board,row);
                break;
            }
            else
            {
                up_mine(board,canvas,x,y,row);          //没有触雷就执行扫雷递归函数
                                                        //打印画布 
                print(canvas,row);


                num = 0;

                for(i = 0; i < row; i++)          //同过已翻的画布数量判断是否已经排雷成功
                {
                    for(j = 0; j < row; j++)
                    {
                        if(canvas[i][j] != '*')
                        {
                            num++;
                        }
                    }
                }
                
            }
            
        }
        else
        {
            printf("坐标非法！\n");   // 判断输入坐标是否非法
            getchar();
            getchar();

        }
    }



    if(num  >= row*row-mine)
    {
        stop = time(NULL);  //游戏计时结束

        time1 =(int)(stop - start); //计算游戏用时

        system("clear");               //清屏
        putchar('\n');
        printf("游戏用时:%d sec\n",time1);
        printf("恭喜你排雷成功！\n");
        print1(board,row);               //打印雷盘


        world_heroes(time1,row,0);       // 进入扫雷英雄榜排序函数
    }



    printf("回车继续\n");
    getchar();    
    getchar();    

}




int up_mine(char **board,char **canvas,int x,int y,int row)   //扫雷递归函数
{    
    int num = 0;                                    //退出递归条件                            
    if(x > row || y > row || x < 1 || y < 1)
    {
        return 0;
    }
    if(board[x][y] == '1')
    {
        return 0;
    }
    if(canvas[x-1][y-1] != '*')
    {
        return 0;
    }
    num = get_mine(board,x,y);              //判断周围是否有雷
    if(num != 0)        
    {
        canvas[x-1][y-1] = '0'+num;         //有雷就将雷数赋值给画布 
        return 0;
    }

    canvas[x-1][y-1] = ' ';                 //没有雷就将画布赋值位空格
    

    up_mine(board,canvas,x-1,y,row);            //递归周围百个格子
    up_mine(board,canvas,x-1,y-1,row);
    up_mine(board,canvas,x,y-1,row);
    up_mine(board,canvas,x+1,y-1,row);
    up_mine(board,canvas,x+1,y,row);
    up_mine(board,canvas,x+1,y+1,row);
    up_mine(board,canvas,x,y+1,row);
    up_mine(board,canvas,x-1,y+1,row);
    
    return 0;

}




int get_mine(char **board,int x ,int y)    //判断周围有几个雷
{
    return board[x-1][y]+               //返回周围有几个雷数
        board[x-1][y-1]+
        board[x][y-1]+
        board[x+1][y-1]+
        board[x+1][y]+
        board[x+1][y+1]+
        board[x][y+1]+
        board[x-1][y+1]-
        8 * '0';

}




int world_heroes(int time1,int row,int flag)     //扫雷英雄榜函数
{
    FILE *fp1 = fopen("hero1.txt","ar+");
    SYSERR(fp1,==,NULL,"open fp1 err",-1);          //打开扫雷英雄榜存储文件,如果没有则创建文件

    FILE *fp2 = fopen("hero2.txt","ar+");
    SYSERR(fp1,==,NULL,"open fp2 err",-2);

    FILE *fp3 = fopen("hero3.txt","ar+");           
    SYSERR(fp1,==,NULL,"open fp3 err",-3);
    
    hero1_t *hero1 = malloc(sizeof(hero1_t)*4);         //创建扫雷英雄榜存储结构体空间 
    SYSERR(hero1,==,NULL,"malloc err!",-1);

    hero1_t *hero2 = malloc(sizeof(hero1_t)*4);
    SYSERR(hero2,==,NULL,"malloc err!",-1);

    hero1_t *hero3 = malloc(sizeof(hero1_t)*4);
    SYSERR(hero3,==,NULL,"malloc err!",-1);


//读取存储文件数据到结构体

    int i,j,a,num;
    for(i = 0; i < 3; i++)
    {
        num = fscanf(fp1,"英雄:%s\t时长:%d\t\n",(hero1+i)->hero,&(hero1+i)->timep);
        if(num == -1)                                                                   //通过返回值判断是否读取成功
        {
            strcpy((hero1+i)->hero,"NA");                       //如果没有读取成功，默认无数据，用NA替代
            (hero1+i)->timep = 99999;
        }

    }
    for(i = 0; i < 3; i++)                                  //同上
    {
        num = fscanf(fp2,"英雄:%s\t时长:%d\t\n",(hero2+i)->hero,&(hero2+i)->timep);
        if(num == -1)
        {
            strcpy((hero2+i)->hero,"NA");
            (hero2+i)->timep = 99999;
        }
    }

    for(i = 0; i < 3; i++)                                  //同上
    {
        num = fscanf(fp3,"英雄:%s\t时长:%d\t\n",(hero3+i)->hero,&(hero3+i)->timep);
        if(num == -1)
        {
            strcpy((hero3+i)->hero,"NA");
            (hero3+i)->timep = 99999;
        }
    }
    

    fclose(fp1);                    //读取完毕关闭打开文件
    fclose(fp2);
    fclose(fp3);

    
    hero1_t temp;           //创建冒泡排序中间转存容器

    int tab = 0;

    if(row == PRI_ROW)                              //判断游戏难度分类
    {
        for(i = 0; i < 3; i++)
        {
            if(time1 < (hero1+i)->timep)            //判断新完成时间是否小于英雄榜时间，小于则往下继续
            {
                putchar('\n');
                printf("恭喜你进入了英雄榜！\n");               
                putchar('\n');

                printf("请输入英雄名:");               //输入新纪录英雄名
                setbuf(stdin,NULL);
                scanf("%s",(hero1+3)->hero);            

                (hero1+3)->timep = time1;       //将新记录时间赋值给结构体，往下排序

                for(a = 0; a < 4; a++)              //冒泡排序名次
                {
                    for(j = 0; j<3-a; j++)
                    {
                        if((hero1+j)->timep > (hero1+j+1)->timep)
                        {
                            temp = hero1[j];
                            hero1[j] = hero1[j+1];
                            hero1[j+1] = temp;
                            tab = 1;
                        }
                    }
                }
                break;
            }
        }
        
        if(tab == 1)                                //标志位判断是否排序成功
        {
            FILE *fp1 = fopen("hero1.txt","w+");            //打开存储文件，清空重新写入排名
            SYSERR(fp1,==,NULL,"open fp1 err",-1);

            for(i = 0; i < 3; i++)                          //写入文件
            {
                fprintf(fp1,"英雄:%s\t时长:%d\t\n",(hero1+i)->hero,(hero1+i)->timep);
            }
             
            fclose(fp1);        //完成关闭文件

            return 0;
        }

    }


    
    if(row == MID_ROW)                              //同上
    {
        for(i = 0; i < 3; i++)
        {
            if(time1 < (hero2+i)->timep)
            {
                putchar('\n');
                printf("恭喜你进入了英雄榜！\n");               
                putchar('\n');
                printf("请输入英雄名:");               
                setbuf(stdin,NULL);
                scanf("%s",(hero2+3)->hero);
                (hero2+3)->timep = time1;

                for(a = 0; a < 4; a++)
                {
                    for(j = 0; j<3-a; j++)
                    {
                        if((hero2+j)->timep > (hero2+j+1)->timep)
                        {
                            temp = hero2[j];
                            hero2[j] = hero2[j+1];
                            hero2[j+1] = temp;
                            tab = 2;
                        }
                    }
                }
                break;
            }
        }
        
         

        if(tab == 2)    
        {

            FILE *fp2 = fopen("hero2.txt","w+");
            SYSERR(fp2,==,NULL,"open fp2 err",-1);

            for(i = 0; i < 3; i++)
            {
                fprintf(fp2,"英雄:%s\t时长:%d\t\n",(hero2+i)->hero,(hero2+i)->timep);
            }
             
            fclose(fp2);
            return 0;

        }



    }





    if(row == SEN_ROW)                  //同上
    {
        for(i = 0; i < 3; i++)
        {
            if(time1 < (hero3+i)->timep)
            {
                putchar('\n');
                printf("恭喜你进入了英雄榜！\n");               
                printf("请输入英雄名:");               
                setbuf(stdin,NULL);
                scanf("%s",(hero3+3)->hero);
                (hero3+3)->timep = time1;

                for(a = 0; a < 4; a++)
                {
                    for(j = 0; j<3-a; j++)
                    {
                        if((hero3+j)->timep > (hero3+j+1)->timep)
                        {
                            temp = hero3[j];
                            hero3[j] = hero3[j+1];
                            hero3[j+1] = temp;
                            tab = 3;
                        }
                    }
                    
                }
                break;
            }
        }

        if(tab == 3)    
        {

            FILE *fp3 = fopen("hero3.txt","w+");
            SYSERR(fp3,==,NULL,"open fp3 err",-1);

            for(i = 0; i < 3; i++)
            {
                fprintf(fp3,"英雄:%s\t时长:%d\t\n",(hero3+i)->hero,(hero3+i)->timep);
            }
             
            fclose(fp3);
            return 0;
        }
    }


    


    if(flag == 1)                                   //根据形参判断是否打印英雄榜,用吧菜单打印英雄榜
    {
        putchar('\n');
        putchar('\n');
        printf("\t\t初级难度英雄榜\n");
        putchar('\n');

        for(i = 0;i < 3; i++)
        {
            printf("\t英雄:%s  \t时长:%d sec\t\n",(hero1+i)->hero,(hero1+i)->timep);
        }

        putchar('\n');
        printf("\t\t中级难度英雄榜\n");
        putchar('\n');

        for(i = 0;i < 3; i++)
        {
            printf("\t英雄:%s  \t时长:%d sec\t\n",(hero2+i)->hero,(hero2+i)->timep);
        }

        putchar('\n');
        printf("\t\t高级难度英雄榜\n");
        putchar('\n');

        for(i = 0;i < 3; i++)
        {
            printf("\t英雄:%s  \t时长:%d sec\t\n",(hero3+i)->hero,(hero3+i)->timep);
        }


    }
    putchar('\n');


    free(hero1);            //释放英雄榜存储空间
    free(hero2);    
    free(hero3);
}




