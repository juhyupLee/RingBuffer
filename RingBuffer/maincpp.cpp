#include <iostream>
#include "RingBuffer.h"
#include <Windows.h>

using namespace std;

char g_Buffer[] = { "1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwxyz 12345" };

int g_MainCount = 0;

int main()
{
	RingBuffer ringBuffer;

	int arr[4];
	arr[0] = 4;
	arr[1] = 3;
	arr[2] = 2;
	arr[3] = 1;

	srand(10);

	//----------------------------------------
	// 테스트 첫번째
	//----------------------------------------
	//ringBuffer.Enqueue((char*)&arr, 16);
	//ringBuffer.Enqueue((char*)&arr, 16);
	//ringBuffer.Enqueue((char*)&arr, 16);
	//ringBuffer.Enqueue((char*)&arr, 16);
	//ringBuffer.Enqueue((char*)&arr, 16);
	//ringBuffer.Enqueue((char*)&arr, 16);


	char Buffer[100];
	//ringBuffer.Dequeue(Buffer, 16);

	//arr[0] = 8;
	//arr[1] = 7;
	//arr[2] = 6;
	//arr[3] = 5;
	//ringBuffer.Enqueue((char*)&arr, 16);
	//ringBuffer.Enqueue((char*)&arr, 4);

	//////----------------------------------------
	////// 테스트 두번째 
	//////----------------------------------------
	//ringBuffer.ClearBuffer();

	//ringBuffer.Enqueue((char*)&arr, 16);
	//ringBuffer.Enqueue((char*)&arr, 16);
	//ringBuffer.Enqueue((char*)&arr, 16);
	//ringBuffer.Enqueue((char*)&arr, 16);
	//ringBuffer.Enqueue((char*)&arr, 16);
	//ringBuffer.Dequeue(Buffer, 16);
	//ringBuffer.Dequeue(Buffer, 16);
	//ringBuffer.Dequeue(Buffer, 16);
	//ringBuffer.Dequeue(Buffer, 16);
	//ringBuffer.Dequeue(Buffer, 16);

	//ringBuffer.Enqueue((char*)&arr, 16);
	//ringBuffer.Enqueue((char*)&arr, 16);

	//----------------------------------------
	// 테스트 세번째 
	//----------------------------------------
	//ringBuffer.ClearBuffer();
	char tempBuffer[100];


	int idx = 0;
	int len = 0;
	int max = 81;

	len = (rand() % 81) + 1; //1~ 81

	while (true)
	{
		//-----------------------------------------------------
		// bool type 테스트
		//-----------------------------------------------------
		//ringBuffer.Enqueue(g_Buffer+ idx, len);

		//if (ringBuffer.Dequeue(tempBuffer, len))
		//{
		//	tempBuffer[len] = '\0';
		//	printf("%s", tempBuffer);

		//}
		//idx += len;
		//max = (max - len);
		//if (max == 0)
		//{
		//	max = 81;
		//	len  = (rand() % max) + 1; //1~ 81
		//	idx = 0;
		//}
		//else
		//{
		//	len = (rand() % max) + 1; //1~ 81
		//}
		//Sleep(200);

		////-----------------------------------------------------
		//// bool type 테스트
		////-----------------------------------------------------

		if (g_MainCount == 6)
			int a = 10;


		int enqueRtn = ringBuffer.Enqueue(g_Buffer+ idx, len);
		memset(tempBuffer, 0, sizeof(tempBuffer));
		int dequeueRtn = ringBuffer.Dequeue(tempBuffer, (rand() % 81) + 1);
		
		printf("%s", tempBuffer);
		//printf("equeueRtn:%d  dequeueRtn:%d idx:%d\n", enqueRtn,dequeueRtn,idx);

		idx += enqueRtn;
		max = (max - enqueRtn);

		if (max == 0)
		{
			max = 81;
			len  = (rand() % max) + 1; //1~ 81
			idx = idx - 81;
			//idx = 0;
		}
		else
		{
			len = (rand() % max) + 1; //1~ 81
		}
		++g_MainCount;
		//Sleep(200);
	}




}