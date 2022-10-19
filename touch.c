#include"touch.h"

struct ts_xy read_touch(void)
{
	int fd_ts;
	fd_ts=open("/dev/input/event0",O_RDONLY);
	if(fd_ts==-1)
	{
		perror("open ts");
		exit(-1);
	}
	
	struct ts_xy temp;
	struct input_event ts_data;
	int i;
	for(i=0;i<6;i++)
	{
		read(fd_ts,&ts_data,sizeof(struct input_event));
		if(ts_data.type==EV_ABS&&ts_data.code==ABS_X) temp.x=ts_data.value*800/1024;
		else if(ts_data.type==EV_ABS&&ts_data.code==ABS_Y) temp.y=ts_data.value*480/600;
	}
	
	close(fd_ts);
	return temp;
}
