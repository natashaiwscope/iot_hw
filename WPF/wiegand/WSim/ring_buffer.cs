using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;

namespace System.Collections
{
/// <summary>
/// Defines a class that represents a resizable circular byte queue.
/// </summary>
public sealed class SerialQueue
{
    private int    TSize;

    private int    RxHead;
    private int    RxTail;
    private int    RxSize;

    private int    TxHead;
    private int    TxTail;
    private int    TxSize;

    private byte[] RxBuffer;
    private byte[] TxBuffer;

    public SerialQueue()
    {
        RxBuffer = new byte[2048];
        TxBuffer = new byte[2048];

        RxHead = 0;
        RxTail = 0;
        RxSize = 0;

        TxHead = 0;
        TxTail = 0;
        TxSize = 0;
        TSize  = 2048;
    }


    internal void PushRxByte(byte pbyte)
    {
        lock (this)
        {
            RxBuffer[RxHead]=pbyte;
            RxSize++;
            RxHead++;
            if(RxHead>=TSize) RxHead=0;
        }
    }

    internal int PullRxByte()
    {
        int j=0x1000;
        if(RxSize>0)
        {
            lock (this)
            {
                j = RxBuffer[RxTail];
                RxSize--;
                RxTail++;
                if(RxTail>=TSize) RxTail=0;
            }
        }
        return j;
    }

    internal void PushTxByte(byte pbyte)
    {
        lock (this)
        {
            TxBuffer[TxHead]=pbyte;
            TxSize++;
            TxHead++;
            if(TxHead>=TSize) TxHead=0;
        }
    }

    internal int PullTxByte()
    {
        int j=0x1000;
        if(TxSize>0)
        {
            lock (this)
            {
                j = TxBuffer[TxTail];
                TxSize--;
                TxTail++;
                if(TxTail>=TSize) TxTail=0;
            }
        }
        return j;
    }

}
}
