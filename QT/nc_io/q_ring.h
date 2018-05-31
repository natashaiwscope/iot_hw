#ifndef __RING_Q_H
#define __RING_Q_H

#include <QThread>
#include <QMutex>
#include <QUrl>
#include <QWaitCondition>
#include <stdio.h>
#include <string.h>

#define   RING_LOCK_TYPE 1
#define NO_MORE_DATA 0x1000

typedef struct Ring_Q
{
    unsigned char           *buffer;
    volatile unsigned short Head;
    volatile unsigned short Tail;
    volatile unsigned short Len;
    volatile unsigned short tLen;
    QMutex                  Mutex;
} Ring_Q;

#define Q_LEN(X)    (X.Len)


static inline void init_sq_mutex(Ring_Q *sq, void *buffer, unsigned short tLen, char *mutName)
{
    sq->buffer = (unsigned char *) buffer;
    sq->tLen = tLen;

    sq->Head = 0;
    sq->Tail = 0;
    sq->Len  = 0;
}

static inline void init_sq(Ring_Q *sq, void *buffer, unsigned short tLen)
{
    sq->buffer = (unsigned char *) buffer;
    sq->tLen = tLen;

    sq->Head = 0;
    sq->Tail = 0;
    sq->Len  = 0;
}

static inline unsigned short push_sq(Ring_Q *sq, const void *buffer, unsigned short len)
{
    unsigned char *rwdata = (unsigned char *)buffer;

    QMutexLocker locker(&sq->Mutex);

    if ((len + sq->Len) > (sq->tLen))
    {
        //millisleep(1000);
    }


    if ((sq->tLen >= (sq->Len + len)))
    {
        if ((len + sq->Head) >= sq->tLen)
        {
            //printf("first case\n");
            memcpy(&sq->buffer[sq->Head], rwdata, sq->tLen - sq->Head);
            memcpy(&sq->buffer[0], (rwdata + (sq->tLen - sq->Head)), sq->Head + len - sq->tLen);
            sq->Len += len;
            sq->Head = (len + sq->Head) - sq->tLen;
        }
        else
        {
            //printf("second case\n");
            memcpy(&sq->buffer[sq->Head], rwdata, len);
            sq->Len += len;
            sq->Head = (len + sq->Head);
        }
        return len;
    }
    return 0;
}


static inline unsigned short pull_sq(Ring_Q *sq, void *buffer, unsigned short rlen)
{
    unsigned short len;
    unsigned char *rwdata = (unsigned char *)buffer;

    QMutexLocker locker(&sq->Mutex);

    len = rlen;

    if (sq->Len == 0)
    {
        return 0;
    }

    if (len >= sq->Len)
    {
        len = sq->Len;
    }

    /* if Q full then return 0 */
    if ((len + sq->Tail) >= sq->tLen)
    {
        //printf("first case\n");
        memcpy(rwdata, &sq->buffer[sq->Tail], sq->tLen - sq->Tail);
        memcpy((rwdata + (sq->tLen - sq->Tail)), &sq->buffer[0], sq->Tail + len - sq->tLen);
        sq->Len -= len;
        sq->Tail = (len + sq->Tail) - sq->tLen;
    }
    else
    {
        //printf("second case\n");
        memcpy(rwdata, &sq->buffer[sq->Tail], len);
        sq->Len -= len;
        sq->Tail = (len + sq->Tail);
    }

    return len;
}

#endif
