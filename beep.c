#include "touch.h"

int fd;
int ret;
//buf[1]---1,蜂鸣器1 只有一个蜂鸣器 永远是1
//buf[0]---1--on，0--off
char buf[2];


void* beep_on(void* data) {
	while (1) {
		printf("beep on thread!!!");

		fd = open("/dev/beep_drv", O_RDWR);
		if (fd < 0)
		{
			perror("open");
		}

		printf("beep on\n");
		while (1) {
			buf[1] = 1;
			buf[0] = 1;  // on
			ret = write(fd, buf, sizeof(buf));
			if (ret != 2) {
				perror("write");
			}
			sleep(2);

			buf[1] = 1;
			buf[0] = 0;  // off
			ret = write(fd, buf, sizeof(buf));
			if (ret != 2) {
				perror("write");
			}
			sleep(2);
		}
	}
}

int Beep(void) {

	struct ts_xy xy_Beep;

	int ret_beep_pthread;
	pthread_t beep_on_id;

	while (1) {
		lcd_show_bmp("/alot/ui/beep_ctrl.bmp");
		xy_Beep = read_touch();
		if (xy_Beep.x > 481 && xy_Beep.x < 645 && xy_Beep.y > 158 && xy_Beep.y < 323)
		{
			ret_beep_pthread = pthread_create(&beep_on_id, NULL, beep_on, NULL);
			if (ret_beep_pthread < 0) {
				perror("beep_on failed");
				return -1;
			}
			while (1)
			{
				xy_Beep = read_touch();
				if (xy_Beep.x > 145 && xy_Beep.x < 313 && xy_Beep.y > 158 && xy_Beep.y < 323)
				{
					pthread_cancel(beep_on_id);
					printf("beep on thread CANCEL!!!");
					buf[1] = 1;
					buf[0] = 0;  // off
					ret = write(fd, buf, sizeof(buf));
					if (ret != 2) {
						perror("write");
					}
					close(fd);
					break;
				}
			}
		}

		if (xy_Beep.x > 0 && xy_Beep.x < 90 && xy_Beep.y > 390 && xy_Beep.y < 480)
			break;
	}




	return 0;
}