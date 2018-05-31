#include<stdio.h>

void main()
{
    int i;
    for (i = 0; i < 10; i++)
    {
        writeUART(0, "Vijayandra", 10);
        osDelay(100);
        writeUART(1, "Vijayandra", 10);
        osDelay(100);
        writeUART(2, "Vijayandra", 10);
        osDelay(100);
    }
}
