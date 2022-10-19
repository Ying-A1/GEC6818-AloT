#include"touch.h"

int keynum1 = 0;                        //输入密码个数
int key1[7] = {0};                      //输入密码的储存
int truekey[7] = {0, 1, 1, 2, 2, 3, 3}; //正确密码
int panduan = 0;                        //最后的判断

int lock()
{
    
    keynum1 = 0;

    int keyshow() //输入时的图片切换
    {
        switch (keynum1)
        {
        case 0:
            lcd_show_bmp("/alot/ui/lock_0.bmp");
            break;
        case 1:
            lcd_show_bmp("/alot/ui/lock_1.bmp");
            break;
        case 2:
            lcd_show_bmp("/alot/ui/lock_2.bmp");
            break;
        case 3:
            lcd_show_bmp("/alot/ui/lock_3.bmp");
            break;
        case 4:
            lcd_show_bmp("/alot/ui/lock_4.bmp");
            break;
        case 5:
            lcd_show_bmp("/alot/ui/lock_5.bmp");
            break;
        case 6: 
            lcd_show_bmp("/alot/ui/lock_6.bmp");
            break;
        default:
            break;
        }
    }
    
    struct ts_xy lock_xy;
    
    while (1) //密码输入
    {
        lock_xy = read_touch();

        if (lock_xy.x > 0 && lock_xy.x < 265 && lock_xy.y > 0 && lock_xy.y < 120) //按钮0
        {
            printf("key1\n");
            keynum1++;
            keyshow();
            key1[keynum1] = 0;
        }

        if (lock_xy.x > 0 && lock_xy.x < 265 && lock_xy.y > 120 && lock_xy.y < 240) //按钮1
        {
            printf("key1\n");
            keynum1++;
            keyshow();
            key1[keynum1] = 1;
        }

        if (lock_xy.x > 266 && lock_xy.x < 535 && lock_xy.y > 120 && lock_xy.y < 240) //按钮2
        {
            printf("key1\n");
            keynum1++;
            keyshow();
            key1[keynum1] = 2;
        }

        if (lock_xy.x > 536 && lock_xy.x < 800 && lock_xy.y > 120 && lock_xy.y < 240) //按钮3
        {
            printf("key1\n");
            keynum1++;
            keyshow();
            key1[keynum1] = 3;
        }

        if (lock_xy.x > 0 && lock_xy.x < 265 && lock_xy.y > 240 && lock_xy.y < 360) //按钮4
        {
            printf("key1\n");
            keynum1++;
            keyshow();
            key1[keynum1] = 4;
        }

        if (lock_xy.x > 266 && lock_xy.x < 535 && lock_xy.y > 240 && lock_xy.y < 360) //按钮5
        {
            printf("key1\n");
            keynum1++;
            keyshow();
            key1[keynum1] = 5;
        }

        if (lock_xy.x > 536 && lock_xy.x < 800 && lock_xy.y > 240 && lock_xy.y < 360) //按6
        {
            printf("key1\n");
            keynum1++;
            keyshow();
            key1[keynum1] = 6;
        }

        if (lock_xy.x > 0 && lock_xy.x < 265 && lock_xy.y > 360 && lock_xy.y < 480) //按钮7
        {
            printf("key1\n");
            keynum1++;
            keyshow();
            key1[keynum1] = 7;
        }

        if (lock_xy.x > 266 && lock_xy.x < 535 && lock_xy.y > 360 && lock_xy.y < 480) //按钮8
        {
            printf("key1\n");
            keynum1++;
            keyshow();
            key1[keynum1] = 8;
        }

        if (lock_xy.x > 536 && lock_xy.x < 800 && lock_xy.y > 360 && lock_xy.y < 480) //按钮9
        {
            printf("key1\n");
            keynum1++;
            keyshow();
            key1[keynum1] = 9;
        }

        if (keynum1 == 6) //退出条件
        {
            printf("密码达到6位\n");
            break;
        }
    }

    for (; panduan < 7; panduan++)//判断密码正确性
    {
        if (key1[panduan] != truekey[panduan])
        {
            break;
        }
    }

    if (panduan == 7)//锁的的判断输出
    {
        return 1;
    }
    else
        return 0;
}