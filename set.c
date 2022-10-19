#include "touch.h"

void* lcd_check(void* data) {
  while (1) {
    lcd_show_color(RED);
    sleep(1);
    lcd_show_color(GREEN);
    sleep(1);
    lcd_show_color(BLUE);
    sleep(1);
  }
}

int Set(void) {
  int ret_set;
  struct ts_xy xy_set;
  pthread_t lcd_check_id;
  ret_set = pthread_create(&lcd_check_id, NULL, lcd_check, NULL);
  if (ret_set < 0) {
    perror("lcd check failed");
    return -1;
  }
  xy_set = read_touch();
  pthread_cancel(lcd_check_id);
  return 0;
}
