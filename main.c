#include "touch.h"

int main(void) {
	struct ts_xy xy;

	int keysign = 0;
	//????
	lcd_show_bmp("/alot/ui/main.bmp");
	while (1) {



		xy = read_touch();
		// 进入
		// 576<x<800
		// 276<y<480

		if (xy.x > 576 && xy.x < 800 && xy.y > 276 && xy.y < 480) {
			while (1) {


				while (keysign == 0)
				{
					keysign = lock();
					if (keysign == 0)
					{
						keysign = lock();
					}
					if (keysign == 1)
					{
						break;
					}
				}

				lcd_show_bmp("/alot/ui/control.bmp");
				xy = read_touch();

				if (xy.x > 63 && xy.x < 177 && xy.y > 92 && xy.y < 368)
					Key();

				else if (xy.x > 200 && xy.x < 315 && xy.y > 114 && xy.y < 368)
					LED();

				else if (xy.x > 339 && xy.x < 461 && xy.y > 92 && xy.y < 368)
					Beep();

				else if (xy.x > 480 && xy.x < 590 && xy.y > 114 && xy.y < 368)
					// Photo();

					while (1)
					{
						struct ts_xy xy_album;
						lcd_show_bmp("/alot/ui/album.bmp");

						xy = read_touch();

						if (xy.x > 0 && xy.x < 90 && xy.y > 390 && xy.y < 480)
						{
							break;
						}
						if (xy.x > 0 && xy.x < 400)
						{
							Photo();
						}
						if (xy.x > 400 && xy.y < 800)
						{
							Video();
						}
						
					}
					
					

				else if (xy.x > 621 && xy.x < 731 && xy.y > 92 && xy.y < 368)
				{
					lcd_show_bmp("/alot/ui/music.bmp");
					int ret;
					pthread_t music;

					ret = pthread_create(&music, NULL, music_control, NULL);
					if (ret < 0)
					{
						perror("music_control failed");
						return -1;
					}
					printf("music = %d\n", music);

					while (1)
					{
						xy = read_touch();
						if (xy.x > 0 && xy.x < 90 && xy.y>390 && xy.y < 480)
						{
							pthread_cancel(music);
							lcd_show_bmp("/alot/ui/control.bmp");
							break;
						}
					}
				}


				else if (xy.x > 714 && xy.x < 800 && xy.y > 409 && xy.y < 480)
					Set();
			}
		}
	}
	return 0;
}
