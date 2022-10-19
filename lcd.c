#include"touch.h"

#pragma pack(1) //强制1个字节对齐
//BMP文件头（14字节）
struct bitmap_header
{
	int16_t type;// 位图文件的类型，必须为BM(1-2字节）
	int32_t size;// 位图文件的大小，以字节为单位（3-6字节）
	int16_t reserved1;// 位图文件保留字，必须为0(7-8字节）
	int16_t reserved2;// 位图文件保留字，必须为0(9-10字节）
	int32_t offbits;// 位图数据的起始位置，以相对于位图（11-14字节）
	// 文件头的偏移量表示，以字节为单位
};
//attribute((packed))：作用：告诉编译器取消结构体在编译过程中的优化对齐
//，按照实际占用字节数进行对齐


//位图信息头（40字节）
struct bitmap_info
{
	int32_t size;// 本结构所占用字节数（15-18字节）    
	int32_t width;// 位图的宽度，以像素为单位（19-22字节）
	int32_t height;// 位图的高度，以像素为单位（23-26字节）
	int16_t planes;// 目标设备的级别，必须为1(27-28字节）
	int16_t bit_count;// 每个像素所需的位数，必须是1（双色），（29-30字节）
	// 4(16色），8(256色）或24（真彩色）之一
	int32_t compression;// 位图压缩类型，必须是 0（不压缩），（31-34字节）
	// 1(BI_RLE8压缩类型）或2(BI_RLE4压缩类型）之一
	int32_t size_img;// 位图的大小，以字节为单位（35-38字节）
	int32_t X_pel;// 位图水平分辨率，每米像素数（39-42字节）
	int32_t Y_pel;// 位图垂直分辨率，每米像素数（43-46字节)
	int32_t clrused;// 位图实际使用的颜色表中的颜色数（47-50字节）
	int32_t clrImportant;// 位图显示过程中重要的颜色数（51-54字节）
};

int lcd_show_color(unsigned int color)
{
	//解决液晶屏花屏
	int fd_lcd;
	//1.打开液晶屏
	fd_lcd = open("/dev/fb0", O_RDWR);
	if (fd_lcd == -1)
	{
		perror("open lcd");
		return -1;
	}
	printf("fd_lcd = %d\n", fd_lcd);

	//2.显存映射，得到显存的首地址
	int* fb_base = NULL;  //int *型，显存中，每个像素点占用4字节
	fb_base = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd_lcd, 0);
	if (fb_base == NULL)
	{
		printf("mmap error\n");
		return -2;
	}

	//3.通过显存的首地址，写入要显示的数据
	for (int i = 0;i < 800 * 480;i++)
		*(fb_base + i) = color;

	//4.解除显存映射
	munmap(fb_base, 800 * 480 * 4);
	close(fd_lcd);

	return 0;
}

int lcd_show_bmp(char* bmp_name)
{
	//1.打开bmp图片
	int fd_bmp;
	fd_bmp = open(bmp_name, O_RDONLY);
	if (fd_bmp == -1)
	{
		perror("open bmp");
		return -1;
	}
	//2.移动文件指针54B，指向RGB数据区
	lseek(fd_bmp, 54, SEEK_SET);
	//3.读取bmp图片的RGB数据
	char bmp_buf[800 * 480 * 3];
	read(fd_bmp, bmp_buf, sizeof(bmp_buf));

	//4.关闭bmp文件
	close(fd_bmp);
	//5.打开液晶屏
	int fd_lcd;
	fd_lcd = open("/dev/fb0", O_RDWR);
	if (fd_lcd == -1)
	{
		perror("open lcd");
		return -1;
	}
	//6.显存映射，得到显存的首地址

	int* fb_base = NULL; //指针类型int--映射后的显存的首地址
	fb_base = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED,
		fd_lcd, 0);
	if (fb_base == NULL)
	{
		printf("mmap error\n");
		return -2;
	}
	//7.通过显存的首地址，写入bmp的RGB数据
	//显存的每个像素点 = bmp图片的每个像素点

	int temp_fb_base[800 * 480];
	for (int i = 0;i < 800 * 480;i++)
		temp_fb_base[i] = (bmp_buf[3 * i + 2] << 16) + (bmp_buf[3 * i + 1] << 8) + (bmp_buf[3 * i] << 0);

	//上下翻转
	for (int y = 0; y < 480; y++)
	{
		for (int x = 0; x < 800; x++)
		{
			*(fb_base + 800 * y + x) = temp_fb_base[x + (479 - y) * 800];
		}
	}


	//8.解除显存映射
	munmap(fb_base, 800 * 480 * 4);

	//9.关闭液晶屏
	close(fd_lcd);
	return 0;
}

int lcd_show_bmp2(char* bmp_name1, char* bmp_name2, int a)
{
	int fd_bmp1, fd_bmp2;
	fd_bmp1 = open(bmp_name1, O_RDONLY);
	if (fd_bmp1 == -1)
	{
		perror("open bmp");
		return -1;
	}
	fd_bmp2 = open(bmp_name2, O_RDONLY);
	if (fd_bmp2 == -1)
	{
		perror("open bmp");
		return -1;
	}

	char Phead[54] = { 0 };

	read(fd_bmp1, Phead, 54);
	unsigned int high1 = Phead[25] << 24 | Phead[24] << 16 | Phead[23] << 8 | Phead[22];
	lseek(fd_bmp1, 54, SEEK_SET);
	char bmp_buf1[800 * high1 * 3];

	read(fd_bmp2, Phead, 54);
	unsigned int high2 = Phead[25] << 24 | Phead[24] << 16 | Phead[23] << 8 | Phead[22];
	lseek(fd_bmp2, 54, SEEK_SET);
	char bmp_buf2[800 * high2 * 3];

	read(fd_bmp1, bmp_buf1, sizeof(bmp_buf1));
	read(fd_bmp2, bmp_buf2, sizeof(bmp_buf2));
	close(fd_bmp1);
	close(fd_bmp2);

	int fd_lcd;
	int* fb_base = NULL;
	fd_lcd = open("/dev/fb0", O_RDWR);
	if (fd_lcd == -1)
	{
		perror("open lcd");
		return -1;
	}
	fb_base = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd_lcd, 0);
	if (fb_base == NULL)
	{
		printf("mmap error\n");
		return -2;
	}
	int i, j, k;
	if (a == 1)
	{
		for (i = 0;i < 800 * 80;i++)
		{
			*(fb_base + i) = (bmp_buf1[((800 * (79 - i / 800) + i % 800) * 3 + 2)] << 16) + (bmp_buf1[((800 * (79 - i / 800) + i % 800) * 3 + 1)] << 8) + (bmp_buf1[((800 * (79 - i / 800) + i % 800) * 3)] << 0);
			*(fb_base + 800 * (479 - i / 800) + i % 800) = (bmp_buf2[(i * 3 + 2)] << 16) + (bmp_buf2[(i * 3 + 1)] << 8) + (bmp_buf2[(i * 3)] << 0);
			if (i % 800 == 0) for (j = 0;j < 80000;j++);
		}
	}
	else if (a == 2)   //(800*(478-i/800)+i%800)
	{
		for (i = 800 * 80 - 1;i > 0;i--)
		{
			*(fb_base + i) = (bmp_buf1[((800 * (479 - i / 800) + i % 800) * 3 + 2)] << 16) + (bmp_buf1[((800 * (479 - i / 800) + i % 800) * 3 + 1)] << 8) + (bmp_buf1[((800 * (479 - i / 800) + i % 800) * 3)] << 0);
			*(fb_base + 800 * (479 - i / 800) + i % 800) = (bmp_buf1[(i * 3 + 2)] << 16) + (bmp_buf1[(i * 3 + 1)] << 8) + (bmp_buf1[(i * 3)] << 0);
			if (i % 800 == 0) for (j = 0;j < 80000;j++);
		}
	}
	else if (a == 3)
	{
		for (i = 0;i < 800 * 80;i++)
		{
			*(fb_base + 800 * (479 - i / 800) + i % 800) = (bmp_buf2[(i * 3 + 2)] << 16) + (bmp_buf2[(i * 3 + 1)] << 8) + (bmp_buf2[(i * 3)] << 0);
		}
	}
	munmap(fb_base, 800 * 480 * 4);
	close(fd_lcd);
	return 0;
}

int lcd_show_bmpxy(char* bmp_name, int start_x, int start_y)
{
	//1.打开bmp图片
	int fd_bmp;
	fd_bmp = open(bmp_name, O_RDONLY);
	if (fd_bmp == -1)
	{
		perror("open bmp");
		return -1;
	}
	//2.1、获取需要的数据，长宽的像素
	struct bitmap_header head;
	struct bitmap_info info;

	read(fd_bmp, &head, 14);
	read(fd_bmp, &info, 40);

	int width = info.width;
	int height = info.height;

	printf("width = %d height = %d\n", width, height);

	// int start_x = (800-width)/2;
	// int start_y = (480-height)/2;

	if (start_x + width > 800 || start_y + height > 480)
	{
		printf("超出显示范围\n");
		return -2;
	}

	//3.读取bmp图片的RGB数据
	// char bmp_buf[800*480*3];
	// read(fd_bmp, bmp_buf, sizeof(bmp_buf));
	char bmp_buf[width * height * 3];

	//2.2.移动文件指针54B，指向RGB数据区
	//lseek(fd_bmp, 54, SEEK_SET);

	switch (4 - (width * info.bit_count / 8 % 4)) {
	case 1:
		for (int i = 0; i < height; ++i) {
			read(fd_bmp, bmp_buf + width * 3 * i, width * 3);
			lseek(fd_bmp, 1, SEEK_CUR);
		}
		break;
	case 2:
		for (int i = 0; i < height; ++i) {
			read(fd_bmp, bmp_buf + width * 3 * i, width * 3);
			lseek(fd_bmp, 2, SEEK_CUR);
		}
		break;
	case 3:
		for (int i = 0; i < height; ++i) {
			read(fd_bmp, bmp_buf + width * 3 * i, width * 3);
			lseek(fd_bmp, 3, SEEK_CUR);
		}
		break;
	default:
		read(fd_bmp, bmp_buf, width * height * 3);
	}

	printf("ok\n", width, height);

	//4.关闭bmp文件
	close(fd_bmp);
	//5.打开液晶屏
	int fd_lcd;
	fd_lcd = open("/dev/fb0", O_RDWR);
	if (fd_lcd == -1)
	{
		perror("open lcd");
		return -1;
	}
	//6.显存映射，得到显存的首地址

	int* fb_base = NULL; //指针类型int--映射后的显存的首地址
	fb_base = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED,
		fd_lcd, 0);
	if (fb_base == NULL)
	{
		printf("mmap error\n");
		return -2;
	}
	//7.通过显存的首地址，写入bmp的RGB数据
	//显存的每个像素点 = bmp图片的每个像素点

	int temp_fb_base[width * height];
	for (int i = 0;i < width * height;i++)
		temp_fb_base[i] = (bmp_buf[3 * i + 2] << 16) + (bmp_buf[3 * i + 1] << 8) + (bmp_buf[3 * i] << 0);
	// printf("ok2\n", width, height);


	int i = 0;
	//上下翻转
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			//*(fb_base + width * y + x) = temp_fb_base[x + (height - 1 - y) * width];
			*(fb_base + (height - 1 - y + start_y) * 800 + x + start_x) = temp_fb_base[i++];
		}
	}

	printf("ok3\n", width, height);
	//8.解除显存映射
	munmap(fb_base, 800 * 480 * 4);

	//9.关闭液晶屏
	close(fd_lcd);
	return 0;
}











