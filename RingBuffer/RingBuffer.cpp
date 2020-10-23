#include <iostream>
#include "RingBuffer.h"
RingBuffer::RingBuffer()
    :
    m_Buffer{ 0, },
    m_Front(0),
    m_Rear(0),
    m_UsedSize(0),
    m_FreeSize(RING_BUFFER_SIZE)
{
}

RingBuffer::~RingBuffer()
{
}


int g_Count = 0;
int g_EnqueueCount = 0;

int RingBuffer::Enqueue(char* buffer, int32_t size)
{
    //-----------------------------------
    // 링버퍼의 남은 사이즈가 들어온 사이즈보다 작다면 size를 프리사이즈로바꿈
    //-----------------------------------
    if (m_FreeSize < size)
    {
        size = m_FreeSize;
    }

    int diff = 0;
    int directSize = GetDirectEnqueueSize();

    //-----------------------------------
    // 인자로 들어온 사이즈가  연속된 사이즈 보다 더 크다면, memcpy를 나눠서해야한다
    //-----------------------------------
    if (directSize < size)
    {
        memcpy(m_Buffer + m_Rear, buffer, directSize);
        m_Rear = (m_Rear + directSize) % RING_BUFFER_SIZE;
        diff = size - directSize;

        memcpy(m_Buffer + m_Rear, buffer + directSize, diff);
        m_Rear = (m_Rear + diff) % RING_BUFFER_SIZE;
    }
    else
    {
        memcpy(m_Buffer + m_Rear, buffer, size);
        m_Rear = (m_Rear + size) % RING_BUFFER_SIZE;
    }

    m_UsedSize += size;
    m_FreeSize -= size;
    return size;
}

int RingBuffer::Dequeue(char* buffer, int32_t size)
{

    //-------------------------------------------
    //링버퍼에 사용중인 사이즈가 인자로 들어온 사이즈보다 작으면 실패
    //-------------------------------------------
    if (m_UsedSize < size)
    {
        size = m_UsedSize;
    }
    int directSize = GetDirectDequeueSize();

    //-------------------------------------------
    // 연속할당할수있는 사이즈가 들어온 사이즈보다 작다면 두번나눠서 디큐를 해야함.
    //-------------------------------------------
    if (directSize < size)
    {
        memcpy(buffer, m_Buffer + m_Front, directSize);
        m_Front = (m_Front + directSize) % RING_BUFFER_SIZE;;

        int diff = size - directSize;
        memcpy(buffer + directSize, m_Buffer + m_Front, diff);
        m_Front = (m_Front + diff) % RING_BUFFER_SIZE;
    }
    else
    {
        memcpy(buffer, m_Buffer + m_Front, size);
        m_Front = (m_Front + size) % RING_BUFFER_SIZE;
    }
    m_UsedSize -= size;
    m_FreeSize += size;
    return size;
}
//bool RingBuffer::Enqueue(char* buffer, int32_t size)
//{
//    //-----------------------------------
//    // 링버퍼의 남은 사이즈가 들어온 사이즈보다 작다면 실패
//    //-----------------------------------
//    if (m_FreeSize< size)
//    {
//        return false;
//    }
//
//    int diff = 0;
//    int directSize = GetDirectEnqueueSize();
//
//    //-----------------------------------
//    // 인자로 들어온 사이즈가  연속된 사이즈 보다 더 크다면, memcpy를 나눠서해야한다
//    //-----------------------------------
//    if (directSize < size)
//    {
//        memcpy(m_Buffer+m_Rear, buffer, directSize);
//        m_Rear = (m_Rear+directSize)%RING_BUFFER_SIZE;
//        diff = size - directSize;
//
//        memcpy(m_Buffer+ m_Rear, buffer+ directSize, diff);
//        m_Rear = (m_Rear + diff) % RING_BUFFER_SIZE;
//    }
//    else
//    {
//        memcpy(m_Buffer+ m_Rear, buffer,size);
//        m_Rear =(m_Rear+ size)% RING_BUFFER_SIZE;
//    }
//    
//    m_UsedSize += size;
//    m_FreeSize -= size;
//    return true;
//}
//
//bool RingBuffer::Dequeue(char* buffer, int32_t size)
//{
//
//    //-------------------------------------------
//    //링버퍼에 사용중인 사이즈가 인자로 들어온 사이즈보다 작으면 실패
//    //-------------------------------------------
//    if (m_UsedSize < size)
//    {
//        return false;
//    }
//    int directSize = GetDirectDequeueSize();
//
//    //-------------------------------------------
//    // 연속할당할수있는 사이즈가 들어온 사이즈보다 작다면 두번나눠서 디큐를 해야함.
//    //-------------------------------------------
//    if (directSize < size)
//    {
//        memcpy(buffer, m_Buffer + m_Front, directSize);
//        m_Front = (m_Front + directSize) % RING_BUFFER_SIZE;;
//
//        int diff = size - directSize;
//        memcpy(buffer+ directSize, m_Buffer + m_Front, diff);
//        m_Front = (m_Front + diff) % RING_BUFFER_SIZE;
//    }
//    else
//    {
//        memcpy(buffer, m_Buffer + m_Front, size);
//        m_Front = (m_Front + size) % RING_BUFFER_SIZE;
//    }
//    m_UsedSize -= size;
//    m_FreeSize += size;
//    return true;
//}

int32_t RingBuffer::GetFreeSize() const
{
    return int32_t();
}

int32_t RingBuffer::GetUsedSize() const
{
    return int32_t();
}

int32_t RingBuffer::GetDirectEnqueueSize() const
{
    //-----------------------------------
    // 링버퍼의 사이즈가 꽉찼다면, Direct Size를 구할수 없다. 0반환
    //-----------------------------------
    if ((m_Rear + 1) % RING_BUFFER_SIZE == m_Front)
    {
        return 0;
    }

    int directSize = 0;

    //-----------------------------------
    // mRear < mFront < 인덱스 끝
    // 이러면  연속된 인덱스는 m_Front 전까지다.
    //-----------------------------------
    if (m_Rear < m_Front && m_Front <= (RING_BUFFER_SIZE - 1))
    {
        directSize = m_Front - m_Rear;
    }
    //-----------------------------------
    // 그것이 아니라면, m_Rear ~ 인덱스끝까지가 연속된 메모리 할당범위다.
    //-----------------------------------
    else
    {
        directSize = ((RING_BUFFER_SIZE - 1) - m_Rear)+1;
    }
    
    return directSize;
}

int32_t RingBuffer::GetDirectDequeueSize() const
{
    //------------------------------------------
    // 링버퍼가 비어있다면, 0을반환
    //------------------------------------------
    if (m_Front == m_Rear)
    {
        return 0;
    }

    int32_t directSize = 0;

    if (m_Front <= (RING_BUFFER_SIZE - 1) && 0 <= m_Rear)
    {
        directSize = ((RING_BUFFER_SIZE - 1) - m_Front)+1;
    }
    else
    {
        directSize = m_Rear- m_Front;
    }

    return directSize;

}

void RingBuffer::ClearBuffer()
{
    m_FreeSize = RING_BUFFER_SIZE;
    m_UsedSize = 0;
    m_Front = 0;
    m_Rear = 0;

}

bool RingBuffer::Peek(char* data, int index)
{
    if (index > RING_BUFFER_SIZE - 1)
    {
        return false;
    }

    int tempFront = m_Front;

    tempFront = (tempFront + index) % RING_BUFFER_SIZE;

    *data = m_Buffer[tempFront];
    return true;
}