#include <iostream>
#include "RingBuffer.h"
#include <iostream>
#include <Windows.h>
#include "LogManager.h"

RingBuffer::RingBuffer()
    :
    m_Buffer{ 0, },
    m_Front(0),
    m_Rear(0)
{
}

RingBuffer::~RingBuffer()
{
}

int RingBuffer::Enqueue(char* buffer, int32_t size)
{
    int32_t copyFront = m_Front;
    int32_t copyRear = m_Rear;
    int32_t copyFreeSize = GetFreeSize(copyFront,copyRear);

    //-----------------------------------
    // �������� ���� ����� ���� ������� �۴ٸ� size�� ����������ιٲ�
    //-----------------------------------
    if (copyFreeSize < size)
    {
        size = copyFreeSize;
        if (size <= 0)
        {
            return 0;
        }
    }

    int32_t diff = 0;
    int32_t directSize = GetDirectEnqueueSize(copyFront, copyRear);

    //-----------------------------------
    // Enqueue ������ m_Rear�� �ܻ� �����ϰ�, Enqeue���Ѵ�.
    //-----------------------------------
    copyRear = (copyRear + 1) % RING_BUFFER_SIZE;

    //-----------------------------------
    // ���ڷ� ���� �����  ���ӵ� ������ ���� �� ũ�ٸ�, memcpy�� �������ؾ��Ѵ�
    //-----------------------------------
    if (directSize < size)
    {
        memcpy(m_Buffer + copyRear, buffer, directSize);
        copyRear = (copyRear +directSize-1) % RING_BUFFER_SIZE;
        diff = size - directSize;

        copyRear = (copyRear + 1) % RING_BUFFER_SIZE;
        memcpy(m_Buffer + copyRear, buffer + directSize, diff);
        copyRear = (copyRear + diff-1) % RING_BUFFER_SIZE;
    }
    else
    {
        memcpy(m_Buffer + copyRear, buffer, size);
        copyRear = (copyRear +size-1) % RING_BUFFER_SIZE;
    }

    m_Rear = copyRear;

    return size;
}

int RingBuffer::Dequeue(char* buffer, int32_t size)
{
    int copyRear = m_Rear;
    int copyFront = m_Front;
    int copyUsedSize = GetUsedSize(copyFront, copyRear);

    //-------------------------------------------
    //�����ۿ� ������� ����� ���ڷ� ���� ������� ������ ����
    //-------------------------------------------
    if (copyUsedSize < size)
    {
        size = copyUsedSize;
        if (size <= 0)
        {
            return 0;
        }
    }

    int directSize = GetDirectDequeueSize(copyFront,copyRear);

    //-----------------------------------
    // Enqueue ������ m_Rear�� �ܻ� �����ϰ�, Enqeue���Ѵ�.
    //-----------------------------------
    copyFront = (copyFront + 1) % RING_BUFFER_SIZE;
    //-------------------------------------------
    // �����Ҵ��Ҽ��ִ� ����� ���� ������� �۴ٸ� �ι������� ��ť�� �ؾ���.
    //-------------------------------------------
    if (directSize < size)
    {
        memcpy(buffer, m_Buffer + copyFront, directSize);
        copyFront = (copyFront + directSize -1) % RING_BUFFER_SIZE;;

        int diff = size - directSize;
        copyFront = (copyFront + 1) % RING_BUFFER_SIZE;
        memcpy(buffer + directSize, m_Buffer + copyFront, diff);
        copyFront = (copyFront +diff -1 ) % RING_BUFFER_SIZE;
    }
    else
    {
        memcpy(buffer, m_Buffer + copyFront, size);
        copyFront = (copyFront +size -1 ) % RING_BUFFER_SIZE;
    }

    m_Front = copyFront;
 
    return size;
}

int32_t RingBuffer::GetFreeSize() const
{
    int freeSize = 0;

    if ((m_Rear + 1) % RING_BUFFER_SIZE == m_Front)
    {
        return 0;
    }
    if (m_Front > m_Rear)
    {
        freeSize = m_Front - m_Rear-1;
    }
    else
    {
        freeSize = m_Front + (RING_BUFFER_SIZE - 1) - m_Rear;
    }
    return freeSize;
}

int32_t RingBuffer::GetUsedSize() const
{
    int32_t usedSize = 0;

    if (m_Rear == m_Front)
    {
        return 0;
    }
    if (m_Front < m_Rear)
    {
        usedSize = m_Rear - m_Front;
    }
    else
    {
        usedSize = m_Rear + (RING_BUFFER_SIZE - 1) - m_Front + 1;
    }
    return usedSize;
}

int32_t RingBuffer::GetFreeSize(int32_t front, int32_t rear) const
{
    int freeSize = 0;
    if ((rear + 1) % RING_BUFFER_SIZE == front)
    {
        return 0;
    }
    if (front > rear)
    {
        freeSize = front - rear-1;
    }
    else
    {
        freeSize = front + (RING_BUFFER_SIZE - 1) - rear;
    }

    return freeSize;
}

int32_t RingBuffer::GetUsedSize(int32_t front, int32_t rear) const
{
    int32_t usedSize = 0;

    if (rear == front)
    {
        return 0;
    }
    if (front < rear)
    {
        usedSize = rear - front ;
    }
    else
    {
        usedSize = rear+ (RING_BUFFER_SIZE - 1) - front + 1;
    }

    return usedSize;
}

int32_t RingBuffer::GetDirectEnqueueSize() const
{
     //-----------------------------------
    // �������� ����� ��á�ٸ�, Direct Size�� ���Ҽ� ����. 0��ȯ
    //-----------------------------------
    int copyFront = m_Front;
    int copyRear = m_Rear;

    if ((copyRear + 1) % RING_BUFFER_SIZE == copyFront)
    {
        return 0;
    }

    int directSize = 0;
    int rearNext = (copyRear + 1) % RING_BUFFER_SIZE;

    //-----------------------------------
    // mRear < mFront < �ε��� ��
    // �̷���  ���ӵ� �ε����� m_Front ��������.
    //-----------------------------------

    if (rearNext < copyFront)
    {
        directSize = copyFront - rearNext;
    }
    //-----------------------------------
    // �װ��� �ƴ϶��, m_Rear ~ �ε����������� ���ӵ� �޸� �Ҵ������.
    //-----------------------------------
    else
    {
        directSize = (RING_BUFFER_SIZE - 1) - rearNext + 1;
    }

    return directSize;
}

int32_t RingBuffer::GetDirectDequeueSize() const
{
    int32_t copyRear = m_Rear;
    int32_t copyFront = m_Front;
    
    //------------------------------------------
    // �����۰� ����ִٸ�, 0����ȯ
    //------------------------------------------
    if (copyFront == copyRear)
    {
        return 0;
    }

    int32_t nextFront = (copyFront + 1) % RING_BUFFER_SIZE;

    int32_t directSize = 0;
    //�Ϲ����� ���
    if (nextFront < copyRear)
    {
        directSize = copyRear - nextFront+1;
    }
    else
    {
        directSize = (RING_BUFFER_SIZE - 1) - nextFront+1;
    }

    return directSize;

}

void RingBuffer::ClearBuffer()
{
    m_Front = 0;
    m_Rear = 0;
}

void RingBuffer::MoveRear(int size)
{
    if (size < 0)
    {
        return;
    }
    m_Rear = (m_Rear + size) % RING_BUFFER_SIZE;
}

void RingBuffer::MoveFront(int size)
{
    if (size < 0)
    {
        return;
    }
    m_Front = (m_Front + size ) % RING_BUFFER_SIZE;
}

char* RingBuffer::GetFrontBufferPtr(void)
{
    //--------------------------------------
    // FrontBufferPtr�� �������ϴ°��� ����
    // dequeue���Ϸ��� ��Ȳ�ε�, ť�� ����ִٸ�
    // null����ȯ
    //--------------------------------------
    if (m_Front == m_Rear)
    {
        return nullptr;
    }

    return m_Buffer + ((m_Front+1)% RING_BUFFER_SIZE);
}

char* RingBuffer::GetRearBufferPtr(void)
{
    //--------------------------------------
    // Enqueue���Ϸ��� ��Ȳ�ε�, ť�� �����ִٸ�,
    // null����ȯ
    //--------------------------------------
    if ((m_Rear+1)%RING_BUFFER_SIZE == m_Front)
    {
        return nullptr;
    }

    return m_Buffer + ((m_Rear + 1) % RING_BUFFER_SIZE); 
}

int RingBuffer::Peek(char* dest, int size)
{
    //-------------------------------------------
   //�����ۿ� ������� ����� ���ڷ� ���� ������� ������ ���� ������� ������ιٲ�.
   // Peek�̱⶧���� ����Ʈ �纻 ����.
   //-------------------------------------------
    int32_t copyRear = m_Rear;
    int32_t copyFront = m_Front;
    int tempUsedSize = GetUsedSize(copyFront,copyRear);

    if (tempUsedSize < size)
    {
        size = tempUsedSize;
        if (size <= 0)
        {
            return 0;
        }
    }

    int directSize = GetDirectDequeueSize();


    //-----------------------------------
    // Peek ������ �纻 front �� �ܻ� �����ϰ�, dequeue���Ѵ�.
    //-----------------------------------
    copyFront = (copyFront + 1) % RING_BUFFER_SIZE;

    //-------------------------------------------
    // �����Ҵ��Ҽ��ִ� ����� ���� ������� �۴ٸ� �ι������� ��ť�� �ؾ���.
    //-------------------------------------------
    if (directSize < size)
    {
        memcpy(dest, m_Buffer + copyFront, directSize);
        copyFront = (copyFront + directSize - 1) % RING_BUFFER_SIZE;;

        int diff = size - directSize;
        copyFront = (copyFront + 1) % RING_BUFFER_SIZE;
        memcpy(dest + directSize, m_Buffer + copyFront, diff);
    }
    else
    {
        memcpy(dest, m_Buffer + copyFront, size);
    }

    return size;
}

int32_t RingBuffer::GetDirectEnqueueSize(int32_t copyFront, int32_t copyRear) const
{
    //-----------------------------------
    // �������� ����� ��á�ٸ�, Direct Size�� ���Ҽ� ����. 0��ȯ
    //-----------------------------------

    if ((copyRear + 1) % RING_BUFFER_SIZE == copyFront)
    {
        return 0;
    }

    int directSize = 0;
    int rearNext = (copyRear + 1) % RING_BUFFER_SIZE;

    //-----------------------------------
    // mRear < mFront < �ε��� ��
    // �̷���  ���ӵ� �ε����� m_Front ��������.
    //-----------------------------------

    if (rearNext < copyFront)
    {
        directSize = copyFront - rearNext;
    }
    //-----------------------------------
    // �װ��� �ƴ϶��, m_Rear ~ �ε����������� ���ӵ� �޸� �Ҵ������.
    //-----------------------------------
    else
    {
        directSize = (RING_BUFFER_SIZE - 1) - rearNext + 1;
    }

    return directSize;
}

int32_t RingBuffer::GetDirectDequeueSize(int32_t copyFront, int32_t copyRear) const
{

    //------------------------------------------
    // �����۰� ����ִٸ�, 0����ȯ
    //------------------------------------------
    if (copyFront == copyRear)
    {
        return 0;
    }

    int nextFront = (copyFront + 1) % RING_BUFFER_SIZE;

    int32_t directSize = 0;
    //�Ϲ����� ���
    if (nextFront < copyRear)
    {
        directSize = copyRear - nextFront + 1;
    }
    else
    {
        directSize = (RING_BUFFER_SIZE - 1) - nextFront + 1;
    }

    return directSize;
}
