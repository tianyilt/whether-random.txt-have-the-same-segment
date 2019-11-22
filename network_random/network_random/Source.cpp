#include<stdio.h>  
#include<string.h>
#include<memory.h>  
#include<malloc.h>
#include<time.h>

/*
���ߣ����ֳ� 10172969 �ż�170
�ο���	1��https://blog.csdn.net/leeboy_wang/article/details/8980682
		2��https://blog.csdn.net/shuxiangxingkong/article/details/52579552
��ϵ��10172969@mail.ecust.edu.cn
*/


/*
ÿ��������2bit��00��ʾ�����ڣ�01��ʾ����һ�Σ�10��ʾ��Σ�11������

��char����洢2-Bitmap,���ÿ��Ǵ�С���ڴ������

ӳ���ϵ���£�

|00 00 00 00| //ӳ��|3 2 1 0|

|00 00 00 00| //ӳ��|7 6 5 4|

����

|00 00 00 00|

*/
#define BIT_MAP_LEN 9000000
unsigned char flags[BIT_MAP_LEN]; //�����С�Զ��壬��Ÿ����������ֵĴ���1000*4=4000������
unsigned get_val(int idx)
{
	int i = idx / 4;  //ȷ������λ��
	int j = idx % 4;  //ȷ��8bit��λ��
	unsigned ret = (flags[i] & (0x3 << (2 * j))) >> (2 * j);    //0x3ʮ�����Ʊ�ʾ00 00 00 11�ɿ���ĳ����������ȡĳ�������ĳ��ִ���
	return ret;
}

struct LNode
{
	int data;
	LNode* next = NULL;
};

/*

�Ѿ���λ�������

*/

unsigned set_val(int idx, unsigned int val)

{
	int i = idx / 4;
	int j = idx % 4;
	unsigned tmp = (flags[i] & ~((0x3 << (2 * j)) & 0xff)) | (((val % 4) << (2 * j)) & 0xff);  //�����㣬ǰ�汣������λ���䣬������������޸ĵ�λ
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
	LNode* HEAD = (LNode*)malloc(sizeof(LNode));//ͷ�ڵ�
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
	printf(">>>���ݶ�ȡ���,�����ݻ���%10.5fs\n", (float)(end - start) / CLOCKS_PER_SEC);
	LNode* Lnoder = HEAD->next;
	fclose(fptr);
	start = clock();
	while (Lnoder)
	{
		int data = Lnoder->data;
		add_one(data);
		Lnoder = Lnoder->next;
	}
	int key = 0;//ָʾ�Ƿ��ҵ����ظ���ֵ
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
	printf(">>>�жϻ���%5.3fs\n", (float)(end - start) / CLOCKS_PER_SEC);
	if (key)
	{
		printf("�ҵ����ظ���ֵ��ֵΪ%ld", i);
	}
	else
	{
		printf(">>>û���ҵ��ظ���ֵ");
	}


}