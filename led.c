#include "touch.h"

int LED(void) {
  struct ts_xy xy_LED;
  int fd;
  int ret;
  // buf[1]---控制7/8/9/10
  // buf[0]--- 1--on  0--off
  char buf[2];
  fd = open("/dev/led_drv", O_RDWR);
  if (fd < 0) {
    perror("open");
    return -1;
  }
  while (1) {
    lcd_show_bmp("/alot/ui/led_ctrl.bmp");
    xy_LED = read_touch();
    if (xy_LED.x > 90 && xy_LED.x < 180 && xy_LED.y > 114 && xy_LED.y < 357)
    {
      printf("LED1灭\n");
      buf[1] = 7;
      buf[0] = 0;
      ret = write(fd, buf, sizeof(buf));
      if (ret != 2) {
        perror("write");
        return -2;
      }
    }

    if (xy_LED.x > 90 && xy_LED.x < 180 && xy_LED.y > 255 && xy_LED.y < 357) {
      printf("LED1亮\n");
      buf[1] = 7;
      buf[0] = 1;
      ret = write(fd, buf, sizeof(buf));
      if (ret != 2) {
        perror("write");
        return -2;
      }
    }

    if (xy_LED.x > 265 && xy_LED.x < 362 && xy_LED.y > 114 && xy_LED.y < 357) {
      printf("LED2灭\n");
      buf[1] = 8;
      buf[0] = 0;
      ret = write(fd, buf, sizeof(buf));
      if (ret != 2) {
        perror("write");
        return -2;
      }
    }

    if (xy_LED.x > 265 && xy_LED.x < 362 && xy_LED.y > 255 && xy_LED.y < 357) {
      printf("LED2亮\n");
      buf[1] = 8;
      buf[0] = 1;
      ret = write(fd, buf, sizeof(buf));
      if (ret != 2) {
        perror("write");
        return -2;
      }
    }

    if (xy_LED.x > 436 && xy_LED.x < 537 && xy_LED.y > 114 && xy_LED.y < 357) {
      printf("LED3灭\n");
      buf[1] = 9;
      buf[0] = 0;
      ret = write(fd, buf, sizeof(buf));
      if (ret != 2) {
        perror("write");
        return -2;
      }
    }

    if (xy_LED.x > 436 && xy_LED.x < 537 && xy_LED.y > 255 && xy_LED.y < 357) {
      printf("LED3亮\n");
      buf[1] = 9;
      buf[0] = 1;
      ret = write(fd, buf, sizeof(buf));
      if (ret != 2) {
        perror("write");
        return -2;
      }
    }

    if (xy_LED.x > 608 && xy_LED.x < 711 && xy_LED.y > 114 && xy_LED.y < 357) {
      printf("LED4灭\n");
      buf[1] = 10;
      buf[0] = 0;
      ret = write(fd, buf, sizeof(buf));
      if (ret != 2) {
        perror("write");
        return -2;
      }
    }

    if (xy_LED.x > 608 && xy_LED.x < 711 && xy_LED.y > 255 && xy_LED.y < 357) {
      printf("LED4亮\n");
      buf[1] = 10;
      buf[0] = 1;
      ret = write(fd, buf, sizeof(buf));
      if (ret != 2) {
        perror("write");
        return -2;
      }
    }

    // 返回

    if (xy_LED.x > 0 && xy_LED.x < 90 && xy_LED.y > 390 && xy_LED.y < 480)
      break;
  }
  
  close(fd);
  return 0;
}
