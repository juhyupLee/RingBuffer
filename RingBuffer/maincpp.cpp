#include <iostream>
#include "RingBuffer.h"
#include <Windows.h>
#include <cassert>
#include "maincpp.h"
#include <process.h>
using namespace std;

char g_Buffer[] = { "1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwxyz 12345" };

int g_MainCount = 0;
RingBuffer g_RingBuffer;


HANDLE g_DeQThread;
HANDLE g_EnQThread;


unsigned int WINAPI DeQThread(LPVOID lpParam);
unsigned int WINAPI EnQThread(LPVOID lpParam);
int main()
{

	int arr[4];
	arr[0] = 4;
	arr[1] = 3;
	arr[2] = 2;
	arr[3] = 1;


	g_EnQThread = (HANDLE)_beginthreadex(NULL, 0, DeQThread, NULL, 0, NULL);
	g_DeQThread = (HANDLE)_beginthreadex(NULL, 0, EnQThread, NULL, 0, NULL);




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

	//EnqueueDequeueTest1();


	while (true)
	{
		int a = 10;
	}


}

void EnqueueDequeueTest1()
{

	char dequeueBuffer[100];
	char peekBuffer[100];

	int idx = 0;
	int len = 0;
	int max = 81;

	len = (rand() % 81) + 1; //1~ 81

	while (true)
	{
		int enqueRtn = g_RingBuffer.Enqueue(g_Buffer + idx, len);
		//----------------------------------------------------------
		// dequeue 전, peek을 하고, dqeuue 후의 결과가 같은지 확인.
		//----------------------------------------------------------
		int dequeueRandNum = (rand() % 81) + 1;

		memset(peekBuffer, 0, sizeof(peekBuffer));
		int peekRtn = g_RingBuffer.Peek(peekBuffer, dequeueRandNum);

		memset(dequeueBuffer, 0, sizeof(dequeueBuffer));
		int dequeueRtn = g_RingBuffer.Dequeue(dequeueBuffer, dequeueRandNum);

		assert(memcmp(peekBuffer, dequeueBuffer, sizeof(dequeueBuffer)) == 0);

		printf("%s", dequeueBuffer);

		idx += enqueRtn;
		max = (max - enqueRtn);

		if (max == 0)
		{
			max = 81;
			len = (rand() % max) + 1; //1~ 81
			idx = idx - 81;
			//idx = 0;
		}
		else
		{
			len = (rand() % max) + 1; //1~ 81
		}
	}


}

unsigned int __stdcall EnQThread(LPVOID lpParam)
{
	int idx = 0;
	int len = 0;
	int max = 81;

	len = (rand() % 81) + 1; //1~ 81

	while (true)
	{
		int enqueRtn = g_RingBuffer.Enqueue(g_Buffer + idx, len);

		if (enqueRtn > 81)
		{
			int a = 10;
		}
		idx += enqueRtn;
		if (idx > 81)
		{
			int a = 10;
		}
		max = (max - enqueRtn);

		if (max == 0)
		{
			max = 81;
			len = (rand() % max) + 1; //1~ 81
			idx = idx - 81;
		}
		else
		{
			len = (rand() % max) + 1; //1~ 81
		}
	}
	return 0;
}

unsigned int __stdcall DeQThread(LPVOID lpParam)
{
	//SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
	char dequeueBuffer[100];
	char peekBuffer[100];
	while (true)
	{
		//----------------------------------------------------------
		// dequeue 전, peek을 하고, dqeuue 후의 결과가 같은지 확인.
		//----------------------------------------------------------
		int dequeueRandNum = (rand() % 81) + 1;

		//memset(peekBuffer, 0, sizeof(peekBuffer));
		//int peekRtn = g_RingBuffer.Peek(peekBuffer, dequeueRandNum);

		memset(dequeueBuffer, 0, sizeof(dequeueBuffer));
		int dequeueRtn = g_RingBuffer.Dequeue(dequeueBuffer, dequeueRandNum);
	
		//assert(memcmp(peekBuffer, dequeueBuffer, sizeof(dequeueBuffer)) == 0);

		printf("%s", dequeueBuffer);
	}
	return 0;
	
}
