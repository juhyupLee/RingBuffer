#include <iostream>
#include "RingBuffer.h"
RingBuffer::RingBuffer()
    :
    m_Buffer{ 0, },
    m_Front(0),
    m_Rear(0),
    m_UsedSize(0),
    m_FreeSize(RING_BUFFER_SIZE-1)
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
    // �������� ���� ����� ���� ������� �۴ٸ� size�� ����������ιٲ�
    //-----------------------------------
    if (m_FreeSize < size)
    {
        size = m_FreeSize;
    }

    int diff = 0;
    int directSize = GetDirectEnqueueSize();
    //-----------------------------------
    // Enqueue ������ m_Rear�� �ܻ� �����ϰ�, Enqeue���Ѵ�.
    //-----------------------------------
    m_Rear = (m_Rear + 1) % RING_BUFFER_SIZE;

    //-----------------------------------
    // ���ڷ� ���� �����  ���ӵ� ������ ���� �� ũ�ٸ�, memcpy�� �������ؾ��Ѵ�
    //-----------------------------------
    if (directSize < size)
    {
        memcpy(m_Buffer + m_Rear, buffer, directSize);
        m_Rear = (m_Rear +directSize-1) % RING_BUFFER_SIZE;
        diff = size - directSize;

        m_Rear = (m_Rear + 1) % RING_BUFFER_SIZE;
        memcpy(m_Buffer + m_Rear, buffer + directSize, diff);
        m_Rear = (m_Rear+ diff-1) % RING_BUFFER_SIZE;
    }
    else
    {
        memcpy(m_Buffer + m_Rear, buffer, size);
        m_Rear = (m_Rear +size-1) % RING_BUFFER_SIZE;
    }

    m_UsedSize += size;
    m_FreeSize -= size;
    return size;
}

int RingBuffer::Dequeue(char* buffer, int32_t size)
{

    //-------------------------------------------
    //�����ۿ� ������� ����� ���ڷ� ���� ������� ������ ����
    //-------------------------------------------
    if (m_UsedSize < size)
    {
        size = m_UsedSize;
    }
    int directSize = GetDirectDequeueSize();

    //-----------------------------------
    // Enqueue ������ m_Rear�� �ܻ� �����ϰ�, Enqeue���Ѵ�.
    //-----------------------------------
    m_Front = (m_Front + 1) % RING_BUFFER_SIZE;
    //-------------------------------------------
    // �����Ҵ��Ҽ��ִ� ����� ���� ������� �۴ٸ� �ι������� ��ť�� �ؾ���.
    //-------------------------------------------
    if (directSize < size)
    {
        memcpy(buffer, m_Buffer + m_Front, directSize);
        m_Front = (m_Front + directSize -1) % RING_BUFFER_SIZE;;

        int diff = size - directSize;
        m_Front = (m_Front + 1) % RING_BUFFER_SIZE;
        char buffTemp[100];
        memcpy(buffer + directSize, m_Buffer + m_Front, diff);
        m_Front = (m_Front+diff -1 ) % RING_BUFFER_SIZE;
    }
    else
    {
        memcpy(buffer, m_Buffer + m_Front, size);
        m_Front = (m_Front+size -1 ) % RING_BUFFER_SIZE;
    }
    m_UsedSize -= size;
    m_FreeSize += size;
    return size;
}
//bool RingBuffer::Enqueue(char* buffer, int32_t size)
//{
//    //-----------------------------------
//    // �������� ���� ����� ���� ������� �۴ٸ� ����
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
//    // ���ڷ� ���� �����  ���ӵ� ������ ���� �� ũ�ٸ�, memcpy�� �������ؾ��Ѵ�
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
//    //�����ۿ� ������� ����� ���ڷ� ���� ������� ������ ����
//    //-------------------------------------------
//    if (m_UsedSize < size)
//    {
//        return false;
//    }
//    int directSize = GetDirectDequeueSize();
//
//    //-------------------------------------------
//    // �����Ҵ��Ҽ��ִ� ����� ���� ������� �۴ٸ� �ι������� ��ť�� �ؾ���.
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
    // �������� ����� ��á�ٸ�, Direct Size�� ���Ҽ� ����. 0��ȯ
    //-----------------------------------
    if ((m_Rear + 1) % RING_BUFFER_SIZE == m_Front)
    {
        return 0;
    }

    int directSize = 0;


    //-----------------------------------
    // mRear < mFront < �ε��� ��
    // �̷���  ���ӵ� �ε����� m_Front ��������.
    //-----------------------------------
    //if (m_Rear < m_Front && m_Front <= (RING_BUFFER_SIZE - 1))
    //{
    //    directSize = m_Front - m_Rear;
    //}

    if (m_Rear < m_Front)
    {
        directSize = m_Front - m_Rear-1;
    }
    //-----------------------------------
    // �װ��� �ƴ϶��, m_Rear ~ �ε����������� ���ӵ� �޸� �Ҵ������.
    //-----------------------------------
    else
    {
        directSize = (RING_BUFFER_SIZE - 1) - m_Rear;
    }
    
    return directSize;
}

int32_t RingBuffer::GetDirectDequeueSize() const
{
    //------------------------------------------
    // �����۰� ����ִٸ�, 0����ȯ
    //------------------------------------------
    if (m_Front == m_Rear)
    {
        return 0;
    }


    int32_t directSize = 0;
    //�Ϲ����� ���
    if (m_Front < m_Rear)
    {
        directSize = m_Rear - m_Front;
    }
    else
    {
        directSize = (RING_BUFFER_SIZE - 1) - m_Front;
    }

    //if (m_Front <= (RING_BUFFER_SIZE - 1) && 0 <= m_Rear)
    //{
    //    directSize = (RING_BUFFER_SIZE - 1) - m_Front;
    //}
    //else
    //{
    //    directSize = m_Rear- m_Front;
    //}

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