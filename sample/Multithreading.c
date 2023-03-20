//***********************************//
// Le Minh Tuan - Thang Long University //
// Multithreading.c
//***********************************//

#include <pthread.h>
#include <stdio.h>


typedef struct{
	int numOfEle;
	int *listInt;
} list;

int sum=0;

void * sumation (void *param);
void * max (void *param);
void * min (void *param);

int main(int argc, char *argv[])
{
	
	pthread_t tid;
	pthread_attr_t attr;
	list param;
	int temp[] = {3,6,4,7, 8};
	param.numOfEle = 5;
	param.listInt = temp;
	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, sumation, &param);
	pthread_join(tid, NULL);
	printf ("Sum %d\n", sum);
	printf("Program finished.\n");
	return 0;
}
void * sumation (void *param)
{
	list* temp = (list*)param;
	int num = temp->numOfEle;
	int *listTemp = temp->listInt;
	int i;
	for (int i=0; i < num; i++)
	{
		sum += listTemp[i];	
	}
	pthread_exit(0);
}


//**************************************//
// Implement others thread function here