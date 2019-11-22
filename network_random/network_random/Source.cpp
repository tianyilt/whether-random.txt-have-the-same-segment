#include<stdio.h>  
#include<string.h>
#include<memory.h>  
#include<malloc.h>
#include<time.h>

/*
作者：周乐诚 10172969 信计170
参考：	1、https://blog.csdn.net/leeboy_wang/article/details/8980682
		2、https://blog.csdn.net/shuxiangxingkong/article/details/52579552
联系：10172969@mail.ecust.edu.cn
*/


/*
每个数分配2bit，00表示不存在，01表示出现一次，10表示多次，11无意义

用char数组存储2-Bitmap,不用考虑大小端内存的问题

映射关系如下：

|00 00 00 00| //映射|3 2 1 0|

|00 00 00 00| //映射|7 6 5 4|

……

|00 00 00 00|

*/
#define BIT_MAP_LEN 9000000
unsigned char flags[BIT_MAP_LEN]; //数组大小自定义，存放各个整数出现的次数1000*4=4000个整数
unsigned get_val(int idx)
{
	int i = idx / 4;  //确定数组位置
	int j = idx % 4;  //确定8bit内位置
	unsigned ret = (flags[i] & (0x3 << (2 * j))) >> (2 * j);    //0x3十六进制表示00 00 00 11可控制某个整数，获取某个整数的出现次数
	return ret;
}

struct LNode
{
	int data;
	LNode* next = NULL;
};

/*

已经过位运算测试

*/

unsigned set_val(int idx, unsigned int val)

{
	int i = idx / 4;
	int j = idx % 4;
	unsigned tmp = (flags[i] & ~((0x3 << (2 * j)) & 0xff)) | (((val % 4) << (2 * j)) & 0xff);  //或运算，前面保留其他位不变，后边重置所需修改的位
	flags[i] = tmp;
	return 0;
}

unsigned add_one(int idx)
{
	if (get_val(idx) >= 2) {

		return 1;
	}
	else {

		set_val(idx, get_val(idx) + 1);
		return 0;
	}
}

int Str2int(FILE* f)
{
	int i = 0;
	char ch = '?';
	int num_collect[10];
	while ((ch = fgetc(f)) != '\n')
	{
		if (ch == EOF)
		{
			return -1;
		}

		int num = ch - '0';
		num_collect[i++] = num;
	}
	int dec = 1, out = 0;
	for (int j = i - 1; j >= 0; --j)
	{
		out += num_collect[j] * dec;
		dec *= 10;
	}
	return out;
}
int main()
{
	time_t start, end;
	FILE* fptr = fopen("Random.txt", "rt");
	LNode* HEAD = (LNode*)malloc(sizeof(LNode));//头节点
	LNode* Lnodes = HEAD;
	int iter;
	start = clock();
	while ((iter = Str2int(fptr)) != -1)
	{
		LNode* newnode = (LNode*)malloc(sizeof(LNode));
		//printf("%d\n", iter);
		newnode->data = iter;
		Lnodes->next = newnode;
		Lnodes = newnode;
	}
	Lnodes->next = NULL;
	end = clock();
	printf(">>>数据读取完成,读数据花了%10.5fs\n", (float)(end - start) / CLOCKS_PER_SEC);
	LNode* Lnoder = HEAD->next;
	fclose(fptr);
	start = clock();
	while (Lnoder)
	{
		int data = Lnoder->data;
		add_one(data);
		Lnoder = Lnoder->next;
	}
	int key = 0;//指示是否找到了重复数值
	int i;
	for (i = 0; i < BIT_MAP_LEN; ++i)
	{
		//printf("%d", get_val(i));
		if (get_val(i) >= 2)
		{
			key = 1;
			break;
		}
	}
	end = clock();
	printf(">>>判断花了%5.3fs\n", (float)(end - start) / CLOCKS_PER_SEC);
	if (key)
	{
		printf("找到了重复的值，值为%ld", i);
	}
	else
	{
		printf(">>>没有找到重复的值");
	}


}