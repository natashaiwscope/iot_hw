#include <stdio.h>
int main(void)
{
    int i, j;
    char buffer[100];
    for (j = 0; j < 5; j++)
    for (i = 1; i <= 9; i++)
    {
        led_on(i);
        osDelay(29);
        led_off(i);
        osDelay(21);
    }

    return 0;
}
