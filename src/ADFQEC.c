#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include "sin.h"
float f;
//   Q_g  测量值
//   Q_S  找根区间
//   QEC  罗差
float QEC_Computer1(float Q_g,int QEC);
float QEC_Computer2(float Q_g,int QEC);

int main()
{
    int i;
    char text[1024];//定义一个字符串数组，用于存储读取的字符
    printf("7°罗差\n");
    printf("测量值                校正值\n");
    for(i=0;i<3600;i++)
    {
        sprintf(text,"%3.1f      %3.1f",(float)i/10,QEC_Computer2((float)i/10,7));
        puts(text);//输出到屏幕
    }
    return 0;
}

//x+sin(2*y)=y
//f=x+sin(2*y)-y
float QEC_Computer1(float Q_g,int QEC)
{
    unsigned int i,Q,Qua;
    float Q_min,f_min,f,Q_temp;
    for(i=0;i<=QEC;i++)
    {
        Qua= (int)((Q_g+i)/90);
        switch(Qua)
        {
            case 0:
            case 2:   Q_temp=(Q_g+i)*20;break;
            case 1:
            case 3:   Q_temp=(Q_g-i)*20;break;
        }
        Q_temp=(int)Q_temp%3600;
        Qua= (int)Q_temp/900;
        switch (Qua)
        {
            case 0:
                    Q=Q_temp;
                //  f=Q_g+QEC*sin(2*(Q_g+i)*3.1415926/180)-(Q_g+i);
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

float QEC_Computer2(float Q_g,int QEC)
{
    unsigned int i,Qua;
    float Q_min,f_min,f,Q_S,Q,Q_temp;
    if ((Q_g>179)&&(Q_g<181))
    {
       if (Q_g>179)
           Q_g=Q_g-1;
       if (Q_g<181)
           Q_g=Q_g+1;
    }
    Q_S=QEC_Computer1(Q_g,QEC);
    Qua= (int)(Q_S/90);
    switch(Qua)
    {
        case 0:
        case 2:
            Q_S=Q_S-1;
            break;
        case 1:
        case 3:
            Q_S=Q_S+1;
            break;
    }
    for(i=0;i<=20;i++)
    {
        Qua= (int)((Q_S)/90);
        switch(Qua)
        {
            case 0:
            case 2:   Q_temp=(Q_S*10+i)*2;break;
            case 1:
            case 3:   Q_temp=(Q_S*10-i)*2;break;
        }
        Q_temp=(int)Q_temp%3600;
        Qua=  (int)Q_temp/900 ;
        switch (Qua)
        {
            case 0:
                    Q=Q_temp;
        //          f=Q_g+QEC*sin(2*(Q_S+i*0.1)*3.1415926/180)-(Q_S+i*0.1);
                    f=Q_g+QEC*SIND[(int)Q/2]-(Q_S+i*0.1);
                    break;
            case 1:
                    Q=1800-Q_temp;
        //          f=Q_g+QEC*sin(2*(Q_S+i*0.1)*3.1415926/180)-(Q_S+i*0.1);
                    f=Q_g+QEC*SIND[(int)Q/2]-(Q_S+i*0.1);
                   break;
            case 2:
                    Q=Q_temp-1800;
    //              f=Q_g+QEC*sin(2*(Q_S+i*0.1)*3.1415926/180)-(Q_S+i*0.1);
                    f=Q_g-QEC*SIND[(int)Q/2]-(Q_S-i*0.1);
                    break;
            case 3:
                    Q=3600-Q_temp;
    //              f=Q_g+QEC*sin(2*(Q_S+i*0.1)*3.1415926/180)-(Q_S+i*0.1);
                    f=Q_g-QEC*SIND[(int)Q/2]-(Q_S-i*0.1);break;
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
                        Q_min=Q_S+i*0.1 ;
                        break;
                    case 2:
                    case 3:
                        Q_min=Q_S-i*0.1;
                        break;
                }
            }
        }
    }
    if ((Q_g>179)&&(Q_g<180))
       Q_min=Q_g;
    if ((Q_g>=180)&&(Q_g<=181))
       Q_min=Q_g;
    if (Q_g>=359)
       Q_min=Q_g;
    return Q_min;
}
