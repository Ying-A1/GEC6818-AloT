#include"touch.h"

int Key(void)
{
	struct ts_xy xy_Key;
	while (1)
	{
		lcd_show_bmp("/alot/ui/key.bmp");
		xy_Key = read_touch();
		if (xy_Key.x > 151 && xy_Key.x < 211 && xy_Key.y > 261 && xy_Key.y < 341)       //���ȿ�
			printf("���ȿ�\n");

		if (xy_Key.x > 244 && xy_Key.x < 309 && xy_Key.y >291 && xy_Key.y < 368)  //���ȹ� 
			printf("���ȹ�\n");

		if (xy_Key.x > 492 && xy_Key.x < 554 && xy_Key.y > 261 && xy_Key.y < 341)  //�յ���
			printf("�յ���\n");

		if (xy_Key.x > 586 && xy_Key.x < 652 && xy_Key.y > 291 && xy_Key.y < 368)  //�յ��� 
			printf("�յ���\n");

		// xy_LED.x > 0 && xy_LED.x < 90 && xy_LED.y > 390 && xy_LED.y < 480

		if (xy_Key.x > 0 && xy_Key.x < 90 && xy_Key.y > 390 && xy_Key.y < 480)
			break;
	}
	return 0;
}
