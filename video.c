#include"touch.h"


static int fd_fifo; //管道文件




int  VideoInit() //初始化
{
    //判断是否存在管道文件， 不存在才创建
    if (access("/fifo", F_OK)) //默认管道文件创建在根目录 F_OK 判断是否存在
    {
        //如果条件成立，不存在
        mkfifo("/fifo", 777);//创建管道文件
    }

    fd_fifo = open("/fifo", O_RDWR);
    if (fd_fifo == -1)
    {
        printf("打开管道文件失败！\n");
        return -1;
    }

    return 0;
}




int  VideoFree() //资源回收
{
    system("killall -9 mplayer");
    return 0;
}

int  SendCmd(char *cmd)//发送命令
{
    write(fd_fifo, cmd, strlen(cmd));
    return 0;
}

int nextvideo(int nextnum){
    if (nextnum<0)
    {
        nextnum=2;
    }
    
    nextnum=nextnum%3;
    system("killall -9 mplayer");
    if (nextnum==0)
    {
        system("mplayer -slave -quiet -input file=/fifo -geometry 0:0 -zoom -x 800 -y 450 ./video/video1.avi &");
    }
    
    else if (nextnum==1)
    {
        system("mplayer -slave -quiet -input file=/fifo -geometry 0:0 -zoom -x 800 -y 450 ./video/video2.avi &");
    }
    
    else if (nextnum==2)
    {
        system("mplayer -slave -quiet -input file=/fifo -geometry 0:0 -zoom -x 800 -y 450 ./video/video3.avi &");
    }
    return nextnum;
}



int Video() //启动视频
{
    
    
    struct ts_xy xy_video;

    int nextnum = 0;
    lcd_show_bmp("/alot/ui/video.bmp");
    VideoInit(); //初始化
    
    system("mplayer -slave -quiet -input file=/fifo -geometry 0:0 -zoom -x 800 -y 450 ./video/video1.avi &");
    while(1)
    {
        xy_video = read_touch();//获取坐标
        if (xy_video.x > 0 && xy_video.x < 43 && xy_video.y > 400 && xy_video.y < 480) //+按钮
        {
            printf("+按钮！\n");
            SendCmd("volume +10\n");
        }

        if (xy_video.x > 46 && xy_video.x < 95 && xy_video.y > 400 && xy_video.y < 480) //-按钮
        {
            printf("-按钮！\n");
            SendCmd("volume -10\n");
        }

        if (xy_video.x > 155 && xy_video.x < 251 && xy_video.y > 400 && xy_video.y < 480) //快退按钮
        {
            printf("快退按钮！\n");
            SendCmd("seek -10\n");
        }

        if (xy_video.x > 355 && xy_video.x < 451 && xy_video.y > 400 && xy_video.y < 480) //暂停/播放按钮
        {
            printf("暂停/播放按钮！\n");
            SendCmd("pause\n");
        }

        if (xy_video.x > 551 && xy_video.x < 646 && xy_video.y > 400 && xy_video.y < 480) //快进按钮
        {
            printf("快进按钮！\n");
            SendCmd("seek +10\n");
        }


        if (xy_video.x > 451 && xy_video.x < 551 && xy_video.y > 400 && xy_video.y < 480) //下一集按钮
        {
            nextnum++;
            nextnum=nextvideo(nextnum);
        }

        if (xy_video.x > 251 && xy_video.x < 355 && xy_video.y > 400 && xy_video.y < 480) //下一集按钮
        {
            nextnum--;
            nextnum=nextvideo(nextnum);
        }

        if (xy_video.x > 708 && xy_video.x < 800 && xy_video.y > 400 && xy_video.y < 480) //退出按钮
        {
            printf("退出按钮！\n");
            break;
        }
    }

    VideoFree();
    return 0;
}