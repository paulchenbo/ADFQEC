#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include "sin.h"
float f;
//   Q_g  测量值
//   Q_S  找根区间
//   QEC  罗差
int QEC_Computer1(int Q_g,int QEC);
int QEC_Computer2(int Q_g,int QEC);

int main()
{
    int i;
    char text[1024];//定义一个字符串数组，用于存储读取的字符
    printf("7°罗差\n");
    printf("测量值                校正值\n");
    for(i=0;i<3600;i++)
    {
        sprintf(text,"%d      %d",i,QEC_Computer2(i,1));
        puts(text);//输出到屏幕
    }
    return 0;
}

//x+sin(2*y)=y
//f=x+sin(2*y)-y
// Q_g  输入0~3600  单位0.1°
// QEC  输入-32~32  单位 °
//粗算
int QEC_Computer1(int Q_g,int QEC)
{
    unsigned int i,Q,Qua;
    float Q_min,f_min,f,Q_temp;
    QEC=QEC*10;
    for(i=0;i<=QEC*10;i=i+10)
    {
        Qua= (int)((Q_g+i)/900);
        switch(Qua)
        {
            case 0:
            case 2:   Q_temp=(Q_g+i)*2;break;
            case 1:
            case 3:   Q_temp=(Q_g-i)*2;break;
        }
        Q_temp=(int)Q_temp%3600;
        Qua= (int)Q_temp/900;
        switch (Qua)
        {
            case 0:
                    Q=Q_temp;
            //      f=Q_g+QEC*sin(2*(Q_g+i)*3.1415926/180)-(Q_g+i);
                    f=Q_g+QEC*SIND[Q/2]-(Q_g+i);
                    break;
            case 1: Q=1800-Q_temp;
            //      f=Q_g+QEC*sin(2*(Q_g+i)*3.1415926/180)-(Q_g+i);
                    f=Q_g+QEC*SIND[Q/2]-(Q_g+i);break;
            case 2:
                    Q=Q_temp-1800;
            //      f=Q_g+QEC*sin(2*(Q_g-i)*3.1415926/180)-(Q_g-i);
                    f=Q_g-QEC*SIND[Q/2]-(Q_g-i);break;
            case 3:
                    Q=3600-Q_temp;
            //      f=Q_g+QEC*sin(2*(Q_g-i)*3.1415926/180)-(Q_g-i);
                    f=Q_g-QEC*SIND[Q/2]-(Q_g-i);break;
            default :break;
        }
        if(i==0)
        {
            f_min=fabs(f);
            Q_min=Q_g;
        }
        else
        {
            if(f_min>fabs(f))
            {
                f_min=fabs(f);
                switch(Qua)
                {
                    case 0:
                    case 1:   Q_min=Q_g+i;break;
                    case 2:
                    case 3:   Q_min=Q_g-i;break;
                }
            }
        }
    }
    return Q_min;
}

int QEC_Computer2(int Q_g,int QEC)
{
    unsigned int i,Qua;
    float Q_min,f_min,f,Q_S,Q,Q_temp;
    Q_S=QEC_Computer1(Q_g,QEC);
    Q_g=fabs(Q_g);
    Q_g=Q_g % 3600;
    if ((QEC>=32)||(QEC<=-32))
        QEC=0;
    QEC=QEC*10;
    Qua= (int)(Q_S/900);
    switch(Qua)
    {
        case 0:
        case 2:
            Q_S=Q_S-10;
            break;
        case 1:
        case 3:
            Q_S=Q_S+10;
            break;
    }
    for(i=0;i<=20;i++)
    {
        Qua= (int)((Q_S)/900);
        switch(Qua)
        {
            case 0:
            case 2:   Q_temp=(Q_S+i)*2;break;
            case 1:
            case 3:   Q_temp=(Q_S-i)*2;break;
        }
        Q_temp=(int)Q_temp%3600;
        Qua=  (int)Q_temp/900 ;
        switch (Qua)
        {
            case 0:
                    Q=Q_temp;
        //          f=Q_g+QEC*sin(2*(Q_S+i*0.1)*3.1415926/180)-(Q_S+i*0.1);
                    f=Q_g+QEC*SIND[(int)Q/2]-(Q_S+i);
                    break;
            case 1:
                    Q=1800-Q_temp;
        //          f=Q_g+QEC*sin(2*(Q_S+i*0.1)*3.1415926/180)-(Q_S+i*0.1);
                    f=Q_g+QEC*SIND[(int)Q/2]-(Q_S+i);
                   break;
            case 2:
                    Q=Q_temp-1800;
    //              f=Q_g+QEC*sin(2*(Q_S+i*0.1)*3.1415926/180)-(Q_S+i*0.1);
                    f=Q_g-QEC*SIND[(int)Q/2]-(Q_S-i);
                    break;
            case 3:
                    Q=3600-Q_temp;
    //              f=Q_g+QEC*sin(2*(Q_S+i*0.1)*3.1415926/180)-(Q_S+i*0.1);
                    f=Q_g-QEC*SIND[(int)Q/2]-(Q_S-i);
                    break;
            default :break;
        }


        if(i==0)
        {
            f_min=fabs(f);
            Q_min=Q_S;
        }
        else
        {
            if(f_min>fabs(f))
            {
                f_min=fabs(f);
                switch(Qua)
                {
                    case 0:
                    case 1:
                        Q_min=Q_S+i ;
                        break;
                    case 2:
                    case 3:
                        Q_min=Q_S-i;
                        break;
                }
            }
        }
    }
    if ((Q_g>=1790)&&(Q_g<1800))
       Q_min=Q_g;
    if ((Q_g>=1800)&&(Q_g<=1810))
       Q_min=Q_g;
    if (Q_g>=3590)
       Q_min=Q_g;
    return Q_min;
}
