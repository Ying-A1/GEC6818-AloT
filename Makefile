main:main.o lcd.o touch.o led.o set.o music.o beep.o photo.o key.o lock.o video.o
	arm-linux-gcc $^ -o main -lpthread
%.o:%.c
	arm-linux-gcc $< -o $@ -c
clean:
	rm -rf *.o main
