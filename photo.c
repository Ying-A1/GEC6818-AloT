#include"touch.h"

int pic = 0;

void* Photo_show(void* data)
{
	char buf2[20];
	while (1)
	{
		if (pic == 0)
		{
			bzero(buf2, 20);
			sprintf(buf2, "/alot/photo/%s", head2->data);
			lcd_show_bmp(buf2);
			head2 = head2->next;
			sleep(1);
		}
	}
}





int Photo(void)
{
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

	struct node* list = list_init("/alot/photo/photo1.bmp");
	DIR* dp = opendir("/alot/photo");
	struct dirent* p;

	while (p = readdir(dp))
	{
		if (p->d_type == DT_REG)
		{
			if (strstr(p->d_name, ".bmp"))
			{
				struct node* new = newnode(p->d_name);
				addnode(new, list);
			}
		}
	}

	head2 = list->next;

	int ret_Photo;
	pthread_t Photo_show_id;
	struct ts_xy xy_Photo;//
	char buf1[20], buf3[20];
	bzero(buf1, 20);
	sprintf(buf1, "/alot/photo/%s", head2->data);//////
	lcd_show_bmp(buf1);;//
	int Photo_dem_pattern = 0;

	unsigned int a;
	int i, X, x;

	while (1)
	{

		xy_Photo = read_touch();
		if (xy_Photo.x >= 0 && xy_Photo.x < 150)
		{
			head2 = head2->prev;
			bzero(buf1, 20);
			sprintf(buf1, "/alot/photo/%s", head2->data);
			lcd_show_bmp(buf1);
		}
		else if (xy_Photo.x > 650 && xy_Photo.x <= 800)
		{
			head2 = head2->next;
			bzero(buf1, 20);
			sprintf(buf1, "/alot/photo/%s", head2->data);
			lcd_show_bmp(buf1);
		}
		else if (xy_Photo.x > 250 && xy_Photo.x <= 550 && xy_Photo.y > 320 && xy_Photo.y <= 480)
			break;

	}

	return 0;
}
