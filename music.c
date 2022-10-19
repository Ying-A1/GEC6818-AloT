#include"touch.h"
#include <pthread.h>


void* music_control(void* data)
{
	//列表随机播放
	//system("madplay  music/*.mp3 -r &");  					
	//system("madplay  music/*.mp3 -r &"); 
	//设置节点
	struct node
	{
		char* data;
		struct node* next;
		struct node* prev;
	};

	//初始化链表
	struct node* list_init(char* newdata)
	{
		struct node* head = malloc(sizeof(struct node));
		head->data = newdata;
		head->next = head;
		head->prev = head;
		return head;
	}

	//创建新节点
	struct node* newnode(char* newdata)
	{
		struct node* new = malloc(sizeof(struct node));
		new->data = newdata;
		new->next = NULL;
		new->prev = NULL;
	}

	//加入新节点
	int addnode(struct node* new, struct node* list)
	{
		struct node* p = list;
		while (p->next != list)
		{
			p = p->next;
		}
		new->prev = list->prev;
		new->next = list;
		list->prev = new;
		new->prev->next = new;
	}


	struct node* list = list_init("/alot/mp3/music1.mp3");						//初始化双向循环链表
	char* path = "/alot/mp3";										//music为音乐目录文件名
	DIR* dp = opendir(path);
	struct dirent* p;

	while (p = readdir(dp))										//遍历目录文件
	{
		if (p->d_type == DT_REG)
		{
			if (strstr(p->d_name, ".mp3")) 						//判断是否为.mp3文件
			{
				struct node* new = newnode(p->d_name); 			//创建新节点
				addnode(new, list);								//插入新节点
			}
		}
	}

	char buf[20];												//用于存放拼接后的字符串
	struct node* head = list->next;

int times=0;
	while (1)
	{
		struct ts_xy xy;
		xy = read_touch();

		if (xy.x > 283 && xy.x < 1380 && xy.y>311 && xy.y < 400&&times==0)
		{
			system("madplay /alot/mp3/*.mp3 &");
			times++;
		}
		if (xy.x > 430 && xy.x < 510 && xy.y>307 && xy.y < 409)
		{
			system("killall -STOP madplay &");
		}
		if (xy.x > 283 && xy.x < 380 && xy.y>311 && xy.y < 400&&times>0)
		{
			system("killall -CONT madplay &");
			times++;
		}
		if (xy.x > 0 && xy.x < 90 && xy.y>390 && xy.y < 480)
		{
			system("killall -KILL madplay");
		}

		if (xy.x > 571 && xy.x < 655 && xy.y>344 && xy.y < 395)	//下一首
		{
			system("killall -9 madplay");

			head = head->next;
			bzero(buf, 20);
			sprintf(buf, "madplay %s/%s &", path, head->data);
			system(buf);
		}

		if (xy.x > 142 && xy.x < 224 && xy.y>320 && xy.y < 395)	//上一首
		{
			system("killall -9 madplay");

			head = head->prev;
			bzero(buf, 20);
			sprintf(buf, "madplay %s/%s &", path, head->data);
			system(buf);
		}


	}
}
